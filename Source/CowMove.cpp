#include "CowMove.h"
#include "Master.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Wall.h"
#include "stage.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include <cmath> 
#include "Player3D.h"
#include "Bait.h"
#include "TutorialScene.h"

/*
 * @brief コンストラクタ
 * @param filename モデルファイルのパス
 * @param initPos 初期配置座標
 */
CowMove::CowMove(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(10.0f)
	, mnActionTimer(0)
	, mActionTimer(60)
	, mCurrentState(STATE_IDLE)
	, mfdeathTime(1900.0f) // 通常ステージでの死亡（吸い切られる）高度
	, mfScore(10)
	, mfXp(10)
	, mbBaitFlag(false)
	, mbIsJudged(false)
	, mCowtDelete(false)
{
	mbIsVisible = true;
	mEffectTimer = 0;
	SetTag(Object3D::Tag3D_Cow);

	// 3Dモデルの生成
	mpModel = new Model(filename, initPos, false);

	// チュートリアルシーンの場合は、早く吸い切れるように限界高度を低く設定
	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
	{
		mfdeathTime = 700.0f;
	}

	// 初期向き（Y軸回転）をランダムに決定
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	mpModel->SetRotation(mvRotation);

	// エフェクトの生成（Effekseer）
	mpCowVm = new EffekseerEffect("Resource/3D/EFK/NowVm.efk", mvPosition, 180.0f);
}

/*
 * @brief デストラクタ
 */
CowMove::~CowMove()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}

	if (mpCowVm != nullptr)
	{
		delete mpCowVm;
		mpCowVm = nullptr;
	}
}

/*
 * @brief 更新処理
 */
void CowMove::Update()
{
	MoveCow();   // 移動・AI関連の処理
	CowDied();    // 吸引・死亡判定処理

	// 吸引状態でなければ、移動方向を向く
	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCow();
	}

	// エフェクトの更新
	if (mpCowVm != nullptr)
	{
		mpCowVm->Update();
	}

	ColliderMove();     // 当たり判定カプセルの同期
	mpModel->Update();  // モデルの行列更新
}

/*
 * @brief 描画処理
 */
void CowMove::Draw()
{
	if (mbIsVisible)
	{
		mpModel->Draw();
	}
}

/*
 * @brief 吸引状態の更新および死亡（回収）処理
 */
void CowMove::CowDied()
{
	// プレイヤーオブジェクトの取得とキャスト
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);

	// --- 1. 吸引モード（STATE_VACUUM）時の処理 ---
	if (mCurrentState == STATE_VACUUM)
	{
		// すでに死亡（回収完了）している場合、消滅エフェクトのタイマーを落とす
		if (mIsDead)
		{
			mEffectTimer--;
			if (mEffectTimer <= 0)
			{
				SetDeleteFlag(true);              // オブジェクトマネージャーからの削除フラグ
				mpCapsuleCollider->SetDeleteFlag(true); // コライダーの削除フラグ
				mCowtDelete = true;
			}
			return;
		}

		// 吸い上げ中の回転演出
		CowRotate();

		// 上昇処理（プレイヤーの攻撃ステータスに応じて上昇速度が変化）
		if (player != nullptr)
		{
			mvPosition.y += player->Status(Player3D::Status_AttackS);

			// フィーバータイム中はプレイヤーの水平位置に引き寄せられる（追従）
			if (Master::FeverFlag)
			{
				float followSpeed = 0.15f;
				VECTOR playerPos = player->GetPosition();
				mvPosition.x += (playerPos.x - mvPosition.x) * followSpeed;
				mvPosition.z += (playerPos.z - mvPosition.z) * followSpeed;
			}
		}
		else
		{
			mvPosition.y += 6.0f; // プレイヤーが見つからない場合のデフォルト上昇値
		}

		// エフェクトの位置を牛に同期
		if (mpCowVm != nullptr)
		{
			mpCowVm->SetPosition(mvPosition);
		}

		// 限界高度を超えたら回収完了（死亡フラグON）
		if (mvPosition.y > mfdeathTime && !mIsDead)
		{
			mIsDead = true;
			Master::mnCaughtCowCount++; // 総回収数のカウントアップ
			Master::mpSoundManager->PlaySE(SoundManager::SE_COW); // 鳴き声SE
			mbIsVisible = false;       // モデルを非表示に
			mEffectTimer = 60;         // 消滅までの猶予タイマー設定

			if (mpCowVm != nullptr)
			{
				mpCowVm->Play(); // 回収エフェクト再生
			}

			// プレイヤーの各種報酬処理（経験値、コンボ、スコア）
			if (player != nullptr)
			{
				player->mpLevel->AddXp(mfXp);
				player->mpCombo->AddHit();
				player->mpScore->AddScore(mfScore + player->mpCombo->GetMultiplier()); // コンボ倍率を上乗せ
			}

			// チュートリアル用のフラグ制御
			if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
			{
				Master::TutrialVacumFlag = true;
			}
		}

		mpModel->SetPosition(mvPosition);
		return;
	}

	// --- 2. 吸引モード以外で死亡フラグが立っている場合のクリーンアップ ---
	if (mIsDead)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
		SetDeleteFlag(true);
	}
}

/*
 * @brief エサによって即死（強制回収）されたときの処理
 */
void CowMove::KilledByBait()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);

	if (player != nullptr)
	{
		Master::mnCaughtCowCount++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_COW);
		player->mpLevel->AddXp(mfXp);
		player->mpCombo->AddHit();
		player->mpScore->AddScore(mfScore + player->mpCombo->GetMultiplier());

		if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
		{
			Master::TutrialVacumFlag = true;
		}
	}

	mIsDead = true;
	mpCapsuleCollider->SetDeleteFlag(true);
	SetDeleteFlag(true);
}

/*
 * @brief 当たり判定用カプセルコライダーの位置・サイズ更新
 */
void CowMove::ColliderMove()
{
	mpCapsuleCollider->mvPosition = mvPosition; // 底面座標
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)); // 天面座標（高さ150）
	mpCapsuleCollider->mfRadius = mColliderRadius;
}

/*
 * @brief 移動メイン処理
 */
void CowMove::MoveCow()
{
	// 他の牛との密集を避ける
	AvoidOtherCows();

	// エサに向かって進んでいる場合は、通常の徘徊AIや壁判定をスキップ
	if (SeekBait())
	{
		return;
	}

	// 通常の徘徊（Wander）AIの更新
	UpdateWanderAI();

	// 壁との衝突判定とすべり処理
	CheckWallCollision();

	mpModel->SetPosition(mvPosition);
}

/*
 * @brief 他の牛との重複・密集を回避する処理（ソーシャルディスタンス）
 */
void CowMove::AvoidOtherCows()
{
	if (mCurrentState == STATE_VACUUM) return;

	// ステージ上の全「牛」のリストを取得
	const auto& cList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Cow);
	for (auto obj : cList)
	{
		if (obj == this) continue; // 自分自身はスキップ

		CowMove* otherCow = dynamic_cast<CowMove*>(obj);
		if (otherCow && otherCow->GetCurrentState() == STATE_VACUUM) continue; // 吸引中の牛は対象外

		VECTOR otherPos = obj->GetPosition();
		VECTOR dir = VSub(mvPosition, otherPos); // 相手から離れる方向ベクトル
		dir.y = 0.0f; // 水平方向のみで計算

		float lengthSq = VSquareSize(dir);
		// 一定距離（半径400）以内にいる場合、押し出し処理を行う
		if (lengthSq < 400.0f * 400.0f)
		{
			// 座標が完全に重なっている（距離がほぼゼロ）場合は、ランダムな方向に逃がす
			if (lengthSq < 0.001f)
			{
				dir = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				if (VSquareSize(dir) < 0.001f) dir = VGet(1.0f, 0.0f, 0.0f);
			}
			dir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(dir, 15.0f)); // 15の力で押し出す
		}
	}
	mpModel->SetPosition(mvPosition);
}

/*
 * @brief エサを感知して追従する処理
 * @return bool エサに向かって移動中か否か
 */
bool CowMove::SeekBait()
{
	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL) return false;
	if (mCurrentState == STATE_VACUUM) return false;

	mvOldPosition = mvPosition; // 衝突前の位置を保存

	if (mbBaitFlag)
	{
		// ステージ上のエサリストを取得
		const auto& b = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Bait);
		if (!b.empty())
		{
			Bait* bait = dynamic_cast<Bait*>(b.at(0)); // 最初に見つかったエサを対象にする
			if (bait)
			{
				VECTOR diff = VSub(bait->GetPosition(), mvPosition);
				float distSq = VSquareSize(diff);

				if (distSq > 100.0f)
				{
					// エサに向かって高速移動（速度25.0）
					mvPosition = VAdd(mvPosition, VScale(VNorm(diff), 25.0f));
				}
				else
				{
					// 密着距離なら直接エサの位置に固定
					mvPosition = bait->GetPosition();
				}
			}
		}
		mpModel->SetPosition(mvPosition);
		return true; // エサ追従中
	}
	return false;
}

/*
 * @brief 徘徊AIの更新（一定時間ごとに歩く・止まるを切り替える）
 */
void CowMove::UpdateWanderAI()
{
	if (mCurrentState == STATE_VACUUM) return;

	mActionTimer--;
	if (mActionTimer <= 0)
	{
		// 50%の確率で歩行状態へ移行
		if (GetRand(100) < 50)
		{
			mCurrentState = STATE_WALK;
			float angle = GetRand(359) * DX_PI_F / 180.0f; // ランダムな方角
			moveVec.x = sinf(angle);
			moveVec.z = cosf(angle);
			mActionTimer = 60 + GetRand(120); // 歩行時間をランダム設定
		}
		// 50%の確率で立ち止まり状態へ移行
		else
		{
			mCurrentState = STATE_IDLE;
			mActionTimer = 60 + GetRand(60);  // 停止時間をランダム設定
		}
	}

	// --- 各状態における移動処理と簡易的な接地（重力）処理 ---
	if (mCurrentState == STATE_WALK)
	{
		mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
		mvPosition.y -= 7.0f; // 常に下方向に力をかける（重力の代わり）
		if (mvPosition.y <= 0)
		{
			mvPosition.y = 0; // 地面（Y=0）で止める
		}
	}
	else if (mCurrentState == STATE_IDLE)
	{
		mvPosition.y -= 4.0f; // 停止時も接地を維持
		if (mvPosition.y <= 0)
		{
			mvPosition.y = 0;
		}
	}
}

/*
 * @brief ポリゴン壁（Wall）との衝突判定およびスライド（壁ずり）計算
 */
void CowMove::CheckWallCollision()
{
	bool hitwall = false;
	bool hitwalls = false;

	// ステージ上の全壁オブジェクトを取得
	const auto& walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall);
	if (!walls.empty())
	{
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// 壁を構成する2つの三角形（四角形ポリゴン）に対して、カプセルとの衝突チェック
				if (HitCheck_Capsule_Triangle(
					mvPosition, VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition, VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);

					// 法線ベクトルを用いて、壁に沿って進むためのスライドベクトル（すべりベクトル）を計算
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					// 最初の1枚目の壁に当たった場合、位置を巻き戻してからスライド方向に移動させる
					if (hitwall && !hitwalls)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 複数の壁に同時に挟まれた場合は、完全に移動を制限する（ハメコミ防止）
					else if (hitwalls)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}
	}
}

/*
 * @brief 通常移動時の方向ベクトルに合わせてモデルの向き（Y軸回転）を同期
 */
void CowMove::RotationCow()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);
		mvRotation.y = targetAngle + DX_PI_F; // モデルの初期方向に応じて180度（DX_PI_F）補正
		mpModel->SetRotation(mvRotation);
	}
}

/*
 * @brief モデルのスケール設定
 */
void CowMove::SetScale(float scale)
{
	mpModel->SetScale(scale);
}

/*
 * @brief 吸引中の回転演出（Y軸を一定速度でクルクル回す）
 */
void CowMove::CowRotate()
{
	float rotateSpeed = 0.1f;
	mvRotation.y += rotateSpeed;
	if (mvRotation.y > DX_PI_F * 2.0f)
	{
		mvRotation.y -= DX_PI_F * 2.0f; // 360度を超えたらラップアラウンド
	}
	mpModel->SetRotation(mvRotation);
}

/*
 * @brief アニメーション追加用（現在未使用のスタブ）
 */
void CowMove::AddAnimation(AnimationState state, std::string filename)
{
}

/*
 * @brief コライダーの接触開始コールバック
 */
void CowMove::OnEnter(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		// エサの検知範囲に入ったらフラグON
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = true;
		}

		// 他の牛と接触した瞬間、少しだけ押し戻し処理を行う
		if (check->mpParentObject->GetTag() == Tag3D_Cow)
		{
			VECTOR otherPos = check->mpParentObject->GetPosition();
			VECTOR dir = VSub(mvPosition, otherPos);
			dir.y = 0.0f;

			float lengthSq = VSquareSize(dir);
			if (lengthSq < 0.001f)
			{
				dir = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				lengthSq = VSquareSize(dir);
				if (lengthSq < 0.001f) dir = VGet(1.0f, 0.0f, 0.0f);
			}

			dir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(dir, 3.0f));
		}
	}
}

/*
 * @brief コライダーの接触中コールバック
 */
void CowMove::OnTrigger(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		// 接触し続けている間も、重なりを防ぐために微小な押し出しを行う
		if (check->mpParentObject->GetTag() == Tag3D_Cow)
		{
			VECTOR otherPos = check->mpParentObject->GetPosition();
			VECTOR dir = VSub(mvPosition, otherPos);
			dir.y = 0.0f;

			float lengthSq = VSquareSize(dir);
			if (lengthSq < 0.001f)
			{
				dir = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				lengthSq = VSquareSize(dir);
				if (lengthSq < 0.001f) dir = VGet(1.0f, 0.0f, 0.0f);
			}

			dir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(dir, 3.0f));
		}
	}
}

/*
 * @brief コライダーの接触終了コールバック
 */
void CowMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		// エサの範囲から出たらフラグOFF
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = false;
		}
	}
}