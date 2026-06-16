#include "CharacterMove.h"
#include "CharacterState.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Wall.h"
#include "Player3D.h"
#include "CapsuleCollider.h"
#include <cmath>
#include "ServiceLocator.h"
#include "Scene.h"
#include "ColliderManager.h"

// ==============================================================================
// コンストラクタ
// キャラクターの初期化を行います。生成時に一度だけ呼ばれます。
// ==============================================================================
CharacterMove::CharacterMove(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mCurrentState(STATE_IDLE)
	, mpCurrentState(new StateIdle()) // 初期状態は「待機」
	, mActionTimer(60)
	, mfSpeed(10.0f)
	, mfTargetAngle(0.0f)
	, mfAngle(0.0f)
	, moveVec(VGet(0.0f, 0.0f, 0.0f))
	, oldmoveVec(VGet(0.0f, 0.0f, 0.0f))
	, hitPos(VGet(0.0f, 0.0f, 0.0f))
	, mVacuumTimer(0)
	, mDeleteFlag(false)
	, mfdeathTime(1000.0f)
	, mfScore(0.0f)
	, mfXp(0.0f)
	, mbBaitFlag(false)
	, mbIsVisible(true)
{
	// 3Dモデルの読み込み（非同期読み込みはfalse）
	mpModel = new Model(filename, initPos, false);

	// キャラクターが全て同じ方向を向かないよう、初期のY軸回転（向き）をランダムに設定
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	mpModel->SetRotation(mvRotation);
}

// ==============================================================================
// デストラクタ
// キャラクター破棄時のメモリ解放を行います。
// ==============================================================================
CharacterMove::~CharacterMove()
{
	// 動的に確保したモデルのメモリを解放
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
	// State（状態）インスタンスのメモリを解放
	if (mpCurrentState != nullptr)
	{
		delete mpCurrentState;
		mpCurrentState = nullptr;
	}
}

// ==============================================================================
// Reset (オブジェクトプール用の再初期化)
// インスタンスを破棄せず、使い回すために初期状態に戻して再配置します。
// ==============================================================================
void CharacterMove::Reset(VECTOR pos)
{
	mvPosition = pos;
	mCurrentState = STATE_IDLE;

	// 現在のStateを一度破棄し、新たに待機状態を作成
	if (mpCurrentState != nullptr)
	{
		delete mpCurrentState;
	}
	mpCurrentState = new StateIdle();

	mActionTimer = 60;
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	oldmoveVec = VGet(0.0f, 0.0f, 0.0f);
	mVacuumTimer = 0;
	mDeleteFlag = false;
	mbBaitFlag = false;
	mbIsVisible = true;
	SetDrawFlag(true); // 描画を有効化

	// 管理クラス(ObjectManager)に自身を再度登録
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	if (mpModel != nullptr)
	{
		mpModel->SetPosition(pos);
		mpModel->SetRotation(mvRotation);
	}

	// 当たり判定（コライダー）を再度有効化して登録
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(false);
		ColliderManager::GetInstance()->AddCollider(mpCapsuleCollider);
	}
}

// ==============================================================================
// Deactivate (オブジェクトの無効化)
// 死亡時や画面外に出た時など、描画と判定をオフにして処理対象から外します。
// ==============================================================================
void CharacterMove::Deactivate()
{
	mbIsVisible = false;
	SetDrawFlag(false); // 描画を無効化

	// 当たり判定を無効化
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
	}

	// deleteはせず、Updateの更新対象リストからのみ外す（再利用のため）
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->RemoveObjectNoDelete(this);
}

// ==============================================================================
// Update (毎フレームの更新処理)
// ==============================================================================
void CharacterMove::Update()
{
	MoveCharacter();   // 移動と壁判定
	CharacterDied();   // 死亡演出などの更新

	// 吸い込まれている最中は独自の回転処理になるため、通常の向き補正を行わない
	if (mCurrentState != STATE_VACUUM)
	{
		RotationCharacter(); // 移動方向に体を向ける
	}

	ColliderMove();    // コライダーをキャラの位置に追従させる
	mpModel->Update(); // 3Dモデル（アニメーション等）の更新
}

// ==============================================================================
// Draw (描画処理)
// ==============================================================================
void CharacterMove::Draw()
{
	mpModel->Draw();
}

// ==============================================================================
// MoveCharacter (移動と座標の確定)
// ==============================================================================
void CharacterMove::MoveCharacter()
{
	// 吸い込み中（UFOに引っ張られている等）は、このクラスの自律移動は行わない
	if (mCurrentState == STATE_VACUUM) return;

	// 壁衝突で押し戻す時のために、移動前の座標を保存
	mvOldPosition = mvPosition;

	UpdateWanderAI();     // Stateパターンによる移動ベクトルの算出
	CheckWallCollision(); // 移動ベクトルに対する壁との当たり判定と押し出し計算

	// 最終的な座標をモデルに適用
	mpModel->SetPosition(mvPosition);
}

// ==============================================================================
// UpdateWanderAI (AIの更新)
// 現在のState（待機や歩行など）のUpdate関数に処理を委譲します。
// ==============================================================================
void CharacterMove::UpdateWanderAI()
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Update(this);
	}
}

// ==============================================================================
// ChangeState (状態の切り替え)
// 古い状態の終了処理(Exit)を行い、新しい状態の開始処理(Enter)を呼び出します。
// ==============================================================================
void CharacterMove::ChangeState(CharacterState* newState)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Exit(this); // 古いStateの後片付け
		delete mpCurrentState;      // メモリ解放
	}
	mpCurrentState = newState;
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Enter(this); // 新しいStateの初期化処理
	}
}

// ==============================================================================
// ChangeStateToVacuum (吸い込み状態への移行)
// UFOに吸い上げられる際の専用処理。
// ==============================================================================
void CharacterMove::ChangeStateToVacuum()
{
	mCurrentState = STATE_VACUUM; // 既存の分岐処理の互換性のため列挙型も更新
	ChangeState(new StateVacuum());
}

// ==============================================================================
// CheckWallCollision (壁との当たり判定と壁ずり移動)
// ==============================================================================
void CharacterMove::CheckWallCollision()
{
	bool hitwall = false;
	bool hitwalls = false;

	// ステージ上の全壁オブジェクトを取得
	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall);
	if (!walls.empty())
	{
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex(); // 壁のポリゴン頂点を取得

				// カプセル（自キャラ）と三角形ポリゴン（壁）の交差判定
				// 壁は四角形（三角形2枚）で構成されているため、2回判定を行う
				if (HitCheck_Capsule_Triangle(
					mvPosition,
					VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)), // カプセルの上端
					80.0f,                                      // カプセルの半径
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition,
						VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);

					// 【壁ずりの数学計算】
					// 内積(VDot)を用いて、壁の法線(norm)に対する移動ベクトルのめり込み具合を計算し、
					// めり込んだ分だけ法線方向に押し返すことで、壁に沿って滑る(スライドする)ベクトルを作る。
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					// 1枚目の壁に当たった場合の処理
					if (hitwall && !hitwalls)
					{
						mvPosition = mvOldPosition; // 一旦移動前の位置に戻す
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed)); // スライドする方向に再移動
						hitwalls = true;
					}
					// 複数の壁（コーナーなど）に同時に当たった場合は、移動を完全にキャンセルして停止させる
					else if (hitwalls)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}
	}
}

// ==============================================================================
// ColliderMove (当たり判定の追従)
// キャラクターの見た目の位置（mvPosition）に、実際の当たり判定領域を合わせます。
// ==============================================================================
void CharacterMove::ColliderMove()
{
	if (mpCapsuleCollider != nullptr)
	{
		// カプセルの底面と天面を設定（高さ150、半径50の円柱状の判定）
		mpCapsuleCollider->mvPosition = mvPosition;
		mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		mpCapsuleCollider->mfRadius = 50.0f;
	}
}

// ==============================================================================
// RotationCharacter (移動方向への旋回)
// 移動している方向にキャラクターの3Dモデルを向けます。
// ==============================================================================
void CharacterMove::RotationCharacter()
{
	// X軸（横）またはZ軸（奥）への移動入力がある場合
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		// atan2f を使って、X成分とZ成分から向かうべき角度（ラジアン）を算出
		float targetAngle = atan2f(moveVec.x, moveVec.z);

		// 3Dモデルの正面方向の仕様に合わせて180度(DX_PI_F)反転させる
		mvRotation.y = targetAngle + DX_PI_F;
		mpModel->SetRotation(mvRotation);
	}
}

// ==============================================================================
// CharacterRotate (継続的な回転)
// アニメーションとは別に、モデルそのものを常に回転させ続ける処理。
// （例：竜巻に巻き込まれてキリモミ回転している時などに使用）
// ==============================================================================
void CharacterMove::CharacterRotate()
{
	mvRotation.y += 0.1f;

	// 角度が360度(2π)を超えたら、0度に戻してオーバーフローを防ぐ
	if (mvRotation.y > DX_PI_F * 2.0f)
	{
		mvRotation.y -= DX_PI_F * 2.0f;
	}
	mpModel->SetRotation(mvRotation);
}

// ==============================================================================
// SetScale (スケールの適用)
// 3Dモデルの大きさを変更します。
// ==============================================================================
void CharacterMove::SetScale(float scale)
{
	if (mpModel != nullptr)
	{
		mpModel->SetScale(scale);
	}
}

// ==============================================================================
// CharacterDied (死亡時の更新処理) - ※未実装
// 死亡フラグが立った後のエフェクトの再生や、一定時間後のDeactivate呼び出しなどを行います。
// ==============================================================================
void CharacterMove::CharacterDied()
{
	//各キャラに書き込む
}

// ==============================================================================
// Die (死亡状態のトリガー) - ※未実装
// ダメージを受けたり吸い込まれたりしてHPが尽きた瞬間に呼ばれます。
// 引数(reason)によって、死因に合わせた専用の演出へ分岐させます。
// ==============================================================================
void CharacterMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	switch (reason)
	{
	}
}