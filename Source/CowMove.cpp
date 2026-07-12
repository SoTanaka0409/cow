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
	int s_mnTagCountCow = 0;
	CowMove::TagCow s_tag1Cow = CowMove::kNone;
	CowMove::TagCow s_tag2Cow = CowMove::kNone;
	CowMove::TagCow s_tag3Cow = CowMove::kNone;
}

CowMove::CowMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	mfdeathTime = GameConstants::kCowDefault.death_time_height;
	mfScore = GameConstants::kCowDefault.score;
	mfXp = GameConstants::kCowDefault.xp;
	mbBaitFlag = false;

	mbIsVisible = true;
	effect_timer_ = 0;
	SetTag(Object3D::kTag3dCow);

	if (Master::mpSceneManager->GetSceneType() == SceneManager::kSceneTutorial)
	{
		mfdeathTime = GameConstants::kCowTutorial.death_time_height;
	}

	cow_vm_ = new EffekseerEffect("Resource/3D/EFK/MowVm.efk", position_, 50.0f);
}

CowMove::~CowMove()
{
}

void CowMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

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

void CowMove::AvoidOtherCows()
{
	const auto& cows = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dCow);
	for (int i = 0; i < cows.size(); ++i)
	{
		CowMove* otherCow = dynamic_cast<CowMove*>(cows.at(i));
		if (otherCow != nullptr && otherCow != this)
		{
			VECTOR otherPos = otherCow->GetPosition();
			VECTOR diff = VSub(position_, otherPos);
			diff.y = 0.0f;

			float distSq = VSquareSize(diff);
			float minDist = 50.0f;

			if (distSq < minDist * minDist)
			{
				VECTOR dir_ = diff;
				if (distSq < 0.001f)
				{
					dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
					if (VSquareSize(dir_) < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
				}
				dir_ = VNorm(dir_);
				position_ = VAdd(position_, VScale(dir_, 15.0f));
			}
		}
	}
	model_->SetPosition(position_);
}

bool CowMove::SeekBait()
{
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
					position_ = VAdd(position_, VScale(VNorm(diff), 25.0f));
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
			position_ = VAdd(position_, VScale(dir_, 3.0f));
		}
	}
}

void CowMove::OnTrigger(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dCow)
		{
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
			position_ = VAdd(position_, VScale(dir_, 3.0f));
		}
	}
}

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

void CowMove::CharacterDied()
{
	if (mCurrentState == STATE_VACUUM)
	{
		Player3D* player = mpTargetPlayer;

		CharacterRotate();
		if (player != nullptr)
		{
			position_.y += player->Status(Player3D::Status_AttackS);

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
			if (position_.y > mfdeathTime)
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
			if (position_.y > mfdeathTime)
			{
				Die(DEATH_VACUUM);
			}
		}
	}
}

void CowMove::KilledByBait()
{
	mbIsVisible = false;
	Die(DEATH_BAIT);
	mDeleteFlag = true;
}

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

			// 蜷檎ｨ�E�騾�E�邯壹く繝ｫ縺�E�繧医�E�繝懊・繝翫せ繧�E�繧�E�繧�E�險育�E�・
			if (tag_cow_ == CowMove::TagCow::kCowT)
			{
				Master::mnTutorialcount++;
			}

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