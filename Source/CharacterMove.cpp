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
#include "ColliderManager.h"
#include "Utility.h"
/*
 * キャラクターの初期化
 * [入力] filename: モデルのファイルパス, initPos: 初期座標
 * [出力] なし
 * [副作用] 各種メンバ変数の初期化、モデルのロード、初期状態(待機)の作成を行う
 */
CharacterMove::CharacterMove(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mCurrentState(STATE_IDLE)
	, current_state_(new StateIdle())
	, mActionTimer(60)
	, speed_(10.0f)
	, target_angle_(0.0f)
	, angle_(0.0f)
	, moveVec(VGet(0.0f, 0.0f, 0.0f))
	, oldmoveVec(VGet(0.0f, 0.0f, 0.0f))
	, hitPos(VGet(0.0f, 0.0f, 0.0f))
	, mVacuumTimer(0)
	, mDeleteFlag(false)
	, death_timer_(1000.0f)
	, score_(0.0f)
	, xp_(0.0f)
	, bait_flag_(false)
	, is_visible_(true)
{
	model_ = new Model(filename, initPos, false);
	// 自然な群れを表現するため、初期向きをランダムに分散させる。
	rotation_.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	model_->SetRotation(rotation_);
}
/*
 * キャラクターの破棄
 * [入力] なし
 * [出力] なし
 * [副作用] モデルおよび現在の状態オブジェクトのメモリを解放する
 */
CharacterMove::~CharacterMove()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
	if (current_state_ != nullptr)
	{
		delete current_state_;
		current_state_ = nullptr;
	}
}
/*
 * キャラクターの再初期化
 * [入力] pos: 配置する座標
 * [出力] なし
 * [副作用] 状態を待機にリセットし、パラメータやフラグを初期化。マネージャーへ再登録する
 */
void CharacterMove::Reset(VECTOR pos)
{
	position_ = pos;
	mCurrentState = STATE_IDLE;
	target_player_ = nullptr;
	if (current_state_ != nullptr)
	{
		delete current_state_;
	}
	current_state_ = new StateIdle();
	mActionTimer = 60;
	rotation_.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	oldmoveVec = VGet(0.0f, 0.0f, 0.0f);
	mVacuumTimer = 0;
	mDeleteFlag = false;
	bait_flag_ = false;
	is_visible_ = true;
	SetDrawFlag(true);
	if (auto scene = Master::scene_manager_->GetCurrentScene())
	{
		if (auto objMgr = scene->GetObjectManager())
		{
			objMgr->AddObject(this);
		}
	}
	if (model_ != nullptr)
	{
		model_->SetPosition(pos);
		model_->SetRotation(rotation_);
	}
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->SetDeleteFlag(false);
		ColliderManager::GetInstance()->AddCollider(capsule_collider_);
	}
}
/*
 * オブジェクトの無効化処理
 * [入力] なし
 * [出力] なし
 * [副作用] is_visible_, 描画フラグ, コライダーの削除フラグを変更。ObjectManagerのリストから除外。
 */
void CharacterMove::Deactivate()
{
	is_visible_ = false;
	SetDrawFlag(false);
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->SetDeleteFlag(true);
	}
	// 再利用時の負荷を軽減するため、メモリ破棄ではなく更新対象から外す。
	if (auto scene = Master::scene_manager_->GetCurrentScene())
	{
		if (auto objMgr = scene->GetObjectManager())
		{
			objMgr->RemoveObjectNoDelete(this);
		}
	}
}
/*
 * 毎フレームの更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] 座標移動、回転、コライダー追従、死亡判定、モデルの更新を行う
 */
void CharacterMove::Update()
{
	MoveCharacter();
	CharacterDied();
	if (mCurrentState != STATE_VACUUM)
	{
		RotationCharacter();
	}
	ColliderMove();
	model_->Update();
}
/*
 * 描画処理
 * [入力] なし
 * [出力] なし
 * [副作用] モデルを描画する
 */
void CharacterMove::Draw()
{
	model_->Draw();
}

void CharacterMove::DrawShadowCaster()
{
	if (model_ != nullptr && is_visible_)
	{
		model_->Draw();
	}
}
/*
 * キャラクターの移動処理
 * [入力] なし
 * [出力] なし
 * [副作用] AIに基づく移動量を計算し、壁判定を経て最終的な座標を決定する
 */
void CharacterMove::MoveCharacter()
{
	if (mCurrentState == STATE_VACUUM) return;
	old_position_ = position_;
	UpdateWanderAI();
	CheckWallCollision();

	if (position_.x > Utility::StageSize.x) position_.x = Utility::StageSize.x;
	if (position_.x < -Utility::StageSize.x) position_.x = -Utility::StageSize.x;
	if (position_.z > Utility::StageSize.z) position_.z = Utility::StageSize.z;
	if (position_.z < -Utility::StageSize.z) position_.z = -Utility::StageSize.z;

	model_->SetPosition(position_);
}
/*
 * AIの更新
 * [入力] なし
 * [出力] なし
 * [副作用] 現在のStateオブジェクトのUpdateを呼び出す
 */
void CharacterMove::UpdateWanderAI()
{
	if (current_state_ != nullptr)
	{
		current_state_->Update(this);
	}
}
/*
 * 状態の変更
 * [入力] newState: 新しい状態クラスのポインタ
 * [出力] なし
 * [副作用] 現在の状態を終了・破棄し、新しい状態へ移行する
 */
void CharacterMove::ChangeState(CharacterState* newState)
{
	if (current_state_ != nullptr)
	{
		current_state_->Exit(this);
		delete current_state_;
	}
	current_state_ = newState;
	if (current_state_ != nullptr)
	{
		current_state_->Enter(this);
	}
}
/*
 * 吸い込み状態への変更
 * [入力] なし
 * [出力] なし
 * [副作用] 現在のAI状態をSTATE_VACUUMに変更し、StateVacuumへ移行する
 */
void CharacterMove::ChangeStateToVacuum()
{
	mCurrentState = STATE_VACUUM;
	ChangeState(new StateVacuum());
}
/*
 * 壁との衝突判定と補正
 * [入力] なし
 * [出力] なし
 * [副作用] 壁に衝突した場合、進行を阻害する方向に座標を押し戻す
 */
void CharacterMove::CheckWallCollision()
{
	bool hitwall = false;
	bool hitwalls = false;
	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dWall);
	if (!walls.empty())
	{
		for (auto& w : walls)
		{
			Wall* wall = dynamic_cast<Wall*>(w);
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();
				if (HitCheck_Capsule_Triangle(
					position_,
					VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						position_,
						VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));
					if (hitwall && !hitwalls)
					{
						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, speed_ * Master::GetDeltaTimeScaler()));
						hitwalls = true;
					}
					// 複数壁への連続衝突によるめり込みを防ぐため。
					else if (hitwalls)
					{
						position_ = old_position_;
					}
				}
			}
		}
	}
}
/*
 * コライダーの追従
 * [入力] なし
 * [出力] なし
 * [副作用] キャラクターの座標に合わせてコライダーの位置と形状を更新する
 */
void CharacterMove::ColliderMove()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		capsule_collider_->radius_ = 50.0f;
	}
}
/*
 * 移動方向への回転
 * [入力] なし
 * [出力] なし
 * [副作用] 現在の移動ベクトルに基づいてモデルのY軸回転角度を更新する
 */
void CharacterMove::RotationCharacter()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);
		// モデルの正面ベクトルが仕様上逆を向いているため、180度補正する。
		rotation_.y = targetAngle + DX_PI_F;
		model_->SetRotation(rotation_);
	}
}
/*
 * 強制的な回転
 * [入力] なし
 * [出力] なし
 * [副作用] モデルをY軸に対して一定速度で回転させ続ける
 */
void CharacterMove::CharacterRotate()
{
	rotation_.y += 0.1f;
	if (rotation_.y > DX_PI_F * 2.0f)
	{
		rotation_.y -= DX_PI_F * 2.0f;
	}
	model_->SetRotation(rotation_);
}
/*
 * モデルのスケール設定
 * [入力] scale: 設定する倍率
 * [出力] なし
 * [副作用] モデルの表示サイズを変更する
 */
void CharacterMove::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}
/*
 * 死亡判定
 * [入力] なし
 * [出力] なし
 * [副作用] 現在は未実装
 */
void CharacterMove::CharacterDied()
{
}
/*
 * キャラクターの死亡処理
 * [入力] reason: 死亡理由を示す列挙値
 * [出力] なし
 * [副作用] 削除フラグの判定や各種死亡に応じた演出・処理を行う
 */
void CharacterMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;
}
