#include "CowMove.h"
#include "GameConstants.h"
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
#include "ServiceLocator.h"

namespace {
	// 暫定対応: コレクション要素廃止の仕様変更に伴い、これらの同種連続回収管理変数は次回更新で削除（期限: 今月末）
	int s_mnTagCountCow = 0;
	CowMove::TagCow s_tag1Cow = CowMove::kNone;
	CowMove::TagCow s_tag2Cow = CowMove::kNone;
	CowMove::TagCow s_tag3Cow = CowMove::kNone;
}

/*
 * 入力: filename (モデル), initPos (初期座標)
 * 出力: なし
 * 副作用: 牛固有のパラメータ（スコア・XP・生存時間）の設定とエフェクトのロード
 */
CowMove::CowMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	death_timer_ = GameConstants::kCowDefault.death_time_height;
	mfScore = GameConstants::kCowDefault.score;
	mfXp = GameConstants::kCowDefault.xp;
	mbBaitFlag = false;

	mbIsVisible = true;
	effect_timer_ = 0;
	SetTag(Object3D::kTag3dCow);

	if (Master::mpSceneManager->GetSceneType() == SceneManager::kSceneTutorial)
	{
		death_timer_ = GameConstants::kCowTutorial.death_time_height;
	}

	cow_vm_ = new EffekseerEffect("Resource/3D/EFK/MowVm.efk", position_, 50.0f);
}

CowMove::~CowMove()
{
}

/*
 * 入力: pos (再配置する座標)
 * 出力: なし
 * 副作用: オブジェクトプーリング再利用時の状態初期化
 */
void CowMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	// プールから再利用した際、以前の死亡判定やエフェクト進行度が引き継がれるバグを防ぐため初期化
	mDeleteFlag = false;
	effect_timer_ = 0;
	if (cow_vm_ != nullptr)
	{
		cow_vm_->SetPosition(pos);
		cow_vm_->Stop();
	}
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
	}
}

void CowMove::Update()
{
	CharacterMove::Update();

	if (cow_vm_ != nullptr && mbIsVisible == false)
	{
		cow_vm_->Update();
	}
}

void CowMove::Draw()
{
	if (mbIsVisible)
	{
		CharacterMove::Draw();
	}
	if (cow_vm_ != nullptr && mbIsVisible == false)
	{
		cow_vm_->Draw();
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: コライダーの座標と高さ(position2_)を現在のモデル座標に追従させる
 */
void CowMove::ColliderMove()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		capsule_collider_->radius_ = collider_radius_;
	}
}

void CowMove::MoveCharacter()
{
	AvoidOtherCows();

	if (SeekBait())
	{
		return;
	}

	CharacterMove::UpdateWanderAI();
	CharacterMove::CheckWallCollision();

	model_->SetPosition(position_);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 他の牛との距離計算および座標の押し出し補正
 */
void CowMove::AvoidOtherCows()
{
	const auto& cows = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dCow);
	for (auto& obj : cows)
	{
		CowMove* otherCow = dynamic_cast<CowMove*>(obj);
		if (otherCow != nullptr && otherCow != this)
		{
			VECTOR otherPos = otherCow->GetPosition();
			VECTOR diff = VSub(position_, otherPos);
			diff.y = 0.0f;

			float distSq = VSquareSize(diff);
			float minDist = 50.0f;

			// 多数の牛が完全に重なり、Zファイティング(描画のチラつき)や不自然な密集が発生するのを防ぐ
			if (distSq < minDist * minDist)
			{
				VECTOR dir_ = diff;
				// 座標が完全に一致した場合、ゼロ除算や押し出し方向の消失を防ぐため微小な乱数ベクトルを与える
				if (distSq < 0.001f)
				{
					dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
					if (VSquareSize(dir_) < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
				}
				dir_ = VNorm(dir_);
				position_ = VAdd(position_, VScale(dir_, 15.0f * Master::GetDeltaTimeScaler()));
			}
		}
	}
	model_->SetPosition(position_);
}

/*
 * 入力: なし
 * 出力: 餌を検知し追従状態に入った場合はtrue
 * 副作用: 餌に向かう座標更新
 */
bool CowMove::SeekBait()
{
	// アクション競合によるスタックを防ぐため、チュートリアル中およびUFO吸引中は餌の追従処理を無効化する
	if (Master::mpSceneManager->GetSceneType() == SceneManager::kSceneTutorial) return false;
	if (mCurrentState == STATE_VACUUM) return false;

	old_position_ = position_;

	if (mbBaitFlag)
	{
		const auto& b = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dBait);
		if (!b.empty())
		{
			Bait* bait = dynamic_cast<Bait*>(b.at(0));
			if (bait)
			{
				VECTOR diff = VSub(bait->GetPosition(), position_);
				float distSq = VSquareSize(diff);

				if (distSq > 100.0f)
				{
					position_ = VAdd(position_, VScale(VNorm(diff), 25.0f * Master::GetDeltaTimeScaler()));
				}
				else
				{
					position_ = bait->GetPosition();
				}
			}
		}
		model_->SetPosition(position_);
		return true;
	}
	return false;
}

void CowMove::UpdateWanderAI()
{
	CharacterMove::UpdateWanderAI();
}

void CowMove::CheckWallCollision()
{
	CharacterMove::CheckWallCollision();
}

void CowMove::RotationCharacter()
{
	CharacterMove::RotationCharacter();
}

void CowMove::CharacterRotate()
{
	CharacterMove::CharacterRotate();
}

void CowMove::AddAnimation(AnimationState state, std::string filename)
{
}

/*
 * 入力: collider (自身のコライダー), check (相手のコライダー)
 * 出力: なし
 * 副作用: 餌検知フラグの有効化、または他牛との物理的な押し出し処理
 */
void CowMove::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			mbBaitFlag = true;
		}

		if (check->parent_object_->GetTag() == kTag3dCow)
		{
			// 物理エンジンの演算遅れでオブジェクト同士が深くめり込んだ際、強引に引き剥がすための補正処理
			VECTOR otherPos = check->parent_object_->GetPosition();
			VECTOR dir_ = VSub(position_, otherPos);
			dir_.y = 0.0f;

			float lengthSq = VSquareSize(dir_);
			if (lengthSq < 0.001f)
			{
				dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				lengthSq = VSquareSize(dir_);
				if (lengthSq < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
			}

			dir_ = VNorm(dir_);
			position_ = VAdd(position_, VScale(dir_, 3.0f * Master::GetDeltaTimeScaler()));
		}
	}
}

/*
 * 入力: collider (自身のコライダー), check (相手のコライダー)
 * 出力: なし
 * 副作用: 他牛との継続的な重なりに対する押し出し処理
 */
void CowMove::OnTrigger(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dCow)
		{
			// OnEnterと同様の理由によるスタック防止策
			VECTOR otherPos = check->parent_object_->GetPosition();
			VECTOR dir_ = VSub(position_, otherPos);
			dir_.y = 0.0f;

			float lengthSq = VSquareSize(dir_);
			if (lengthSq < 0.001f)
			{
				dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				lengthSq = VSquareSize(dir_);
				if (lengthSq < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
			}

			dir_ = VNorm(dir_);
			position_ = VAdd(position_, VScale(dir_, 3.0f * Master::GetDeltaTimeScaler()));
		}
	}
}

/*
 * 入力: collider (自身のコライダー), check (相手のコライダー)
 * 出力: なし
 * 副作用: 餌検知フラグの無効化
 */
void CowMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			mbBaitFlag = false;
		}
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: UFO吸引中の座標追従、エフェクト再生、および一定高度到達による死亡処理
 */
void CowMove::CharacterDied()
{
	if (mCurrentState == STATE_VACUUM)
	{
		Player3D* player = mpTargetPlayer;

		CharacterRotate();
		if (player != nullptr)
		{
			position_.y += player->Status(Player3D::Status_AttackS) * Master::GetDeltaTimeScaler();

			// フィーバー中はプレイヤーの移動速度が上がるため、吸引漏れを防ぐ目的で牛をUFOの真下へ強制的に吸い寄せる
			if (Master::FeverFlag)
			{
				float followSpeed = 0.15f;
				VECTOR playerPos = player->GetPosition();
				position_.x += (playerPos.x - position_.x) * followSpeed;
				position_.z += (playerPos.z - position_.z) * followSpeed;
			}
		}
		model_->SetPosition(position_);

		if (cow_vm_ != nullptr)
		{
			if (position_.y > death_timer_)
			{
				if (effect_timer_ <= 0 && mbIsVisible == true)
				{
					cow_vm_->Play();
					effect_timer_ = 60;
					mbIsVisible = false;
					capsule_collider_->SetDeleteFlag(true);
				}

				if (!mbIsVisible)
				{
					effect_timer_--;
				}

				if (effect_timer_ <= 0 && !mbIsVisible)
				{
					Die(DEATH_VACUUM);
				}
			}
		}
		else
		{
			if (position_.y > death_timer_)
			{
				Die(DEATH_VACUUM);
			}
		}
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 餌による死亡(消失)処理および削除フラグ付与
 */
void CowMove::KilledByBait()
{
	mbIsVisible = false;
	Die(DEATH_BAIT);
	mDeleteFlag = true;
}

/*
 * 入力: reason (死亡理由)
 * 出力: なし
 * 副作用: プレイヤーへのスコア・XP・コンボ加算、および自身への削除フラグ付与
 */
void CowMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	Player3D* player = mpTargetPlayer;

	switch (reason)
	{
	case DEATH_VACUUM:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->combo_->AddHit();
			player->mpScore->AddScore(mfScore * player->combo_->GetMultiplier());

			if (tag_cow_ == CowMove::TagCow::kCowT)
			{
				Master::mnTutorialcount++;
			}

			// 暫定対応: コレクション要素を廃止してアクションに特化する仕様変更に伴い、
			// 以下の同種連続回収ボーナス処理は非推奨コードとする。次回リファクタリング時に削除する（期限：今月末）
			s_mnTagCountCow++;
			if (s_mnTagCountCow == 1)
			{
				s_tag1Cow = tag_cow_;
			}
			else if (s_mnTagCountCow == 2 && s_tag1Cow == tag_cow_)
			{
				s_tag2Cow = tag_cow_;
			}
			else if (s_mnTagCountCow == 3 && s_tag2Cow == tag_cow_)
			{
				s_tag3Cow = tag_cow_;
				if (s_tag3Cow == CowMove::kCow1) player->mpScore->AddScore(300);
				if (s_tag2Cow == CowMove::kCow2) player->mpScore->AddScore(600);
			}
			else
			{
				s_mnTagCountCow = 0;
				s_tag1Cow = CowMove::kNone;
				s_tag2Cow = CowMove::kNone;
				s_tag3Cow = CowMove::kNone;
			}
		}
		mDeleteFlag = true;
		break;

	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->combo_->AddHit();
			player->mpScore->AddScore(mfScore * player->combo_->GetMultiplier());
		}
		mDeleteFlag = true;
		break;

	case DEATH_LIMIT:
		mDeleteFlag = true;
		break;
	}
}