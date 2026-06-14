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
	CowMove::Tag_cow s_tag1Cow = CowMove::none;
	CowMove::Tag_cow s_tag2Cow = CowMove::none;
	CowMove::Tag_cow s_tag3Cow = CowMove::none;
}

CowMove::CowMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
	, mCowtDelete(false)
{
	mfdeathTime = GameConstants::COW_DEFAULT.deathTimeHeight;
	mfScore = GameConstants::COW_DEFAULT.score;
	mfXp = GameConstants::COW_DEFAULT.xp;
	mbBaitFlag = false;

	mbIsVisible = true;
	mEffectTimer = 0;
	SetTag(Object3D::Tag3D_Cow);

	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
	{
		mfdeathTime = GameConstants::COW_TUTORIAL.deathTimeHeight;
	}

	mpCowVm = new EffekseerEffect("Resource/3D/EFK/MowVm.efk", mvPosition, 50.0f);
}

CowMove::~CowMove()
{
}

void CowMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	mCowtDelete = false;
	mEffectTimer = 0;
	if (mpCowVm != nullptr)
	{
		mpCowVm->SetPosition(pos);
		mpCowVm->Stop();
	}
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = pos;
	}
}

void CowMove::Update()
{
	CharacterMove::Update();

	if (mpCowVm != nullptr && mbIsVisible == false)
	{
		mpCowVm->Update();
	}
}

void CowMove::Draw()
{
	if (mbIsVisible)
	{
		CharacterMove::Draw();
	}
	if (mpCowVm != nullptr && mbIsVisible == false)
	{
		mpCowVm->Draw();
	}
}

void CowMove::ColliderMove()
{
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = mvPosition;
		mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		mpCapsuleCollider->mfRadius = mColliderRadius;
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

	mpModel->SetPosition(mvPosition);
}

void CowMove::AvoidOtherCows()
{
	const auto& cows = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Cow);
	for (int i = 0; i < cows.size(); ++i)
	{
		CowMove* otherCow = dynamic_cast<CowMove*>(cows.at(i));
		if (otherCow != nullptr && otherCow != this)
		{
			VECTOR otherPos = otherCow->GetPosition();
			VECTOR diff = VSub(mvPosition, otherPos);
			diff.y = 0.0f;

			float distSq = VSquareSize(diff);
			float minDist = 50.0f;

			if (distSq < minDist * minDist)
			{
				VECTOR dir = diff;
				if (distSq < 0.001f)
				{
					dir = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
					if (VSquareSize(dir) < 0.001f) dir = VGet(1.0f, 0.0f, 0.0f);
				}
				dir = VNorm(dir);
				mvPosition = VAdd(mvPosition, VScale(dir, 15.0f));
			}
		}
	}
	mpModel->SetPosition(mvPosition);
}

bool CowMove::SeekBait()
{
	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL) return false;
	if (mCurrentState == STATE_VACUUM) return false;

	mvOldPosition = mvPosition;

	if (mbBaitFlag)
	{
		const auto& b = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Bait);
		if (!b.empty())
		{
			Bait* bait = dynamic_cast<Bait*>(b.at(0));
			if (bait)
			{
				VECTOR diff = VSub(bait->GetPosition(), mvPosition);
				float distSq = VSquareSize(diff);

				if (distSq > 100.0f)
				{
					mvPosition = VAdd(mvPosition, VScale(VNorm(diff), 25.0f));
				}
				else
				{
					mvPosition = bait->GetPosition();
				}
			}
		}
		mpModel->SetPosition(mvPosition);
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
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = true;
		}

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

void CowMove::OnTrigger(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
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

void CowMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = false;
		}
	}
}

void CowMove::CharacterDied()
{
	if (mCurrentState == STATE_VACUUM)
	{
		auto p = ServiceLocator::GetPlayer();
		Player3D* player = dynamic_cast<Player3D*>(p);

		CharacterRotate();
		if (player != nullptr)
		{
			mvPosition.y += player->Status(Player3D::Status_AttackS);

			if (Master::FeverFlag)
			{
				float followSpeed = 0.15f;
				VECTOR playerPos = player->GetPosition();
				mvPosition.x += (playerPos.x - mvPosition.x) * followSpeed;
				mvPosition.z += (playerPos.z - mvPosition.z) * followSpeed;
			}
		}
		mpModel->SetPosition(mvPosition);

		if (mpCowVm != nullptr)
		{
			if (mvPosition.y > mfdeathTime)
			{
				if (mEffectTimer <= 0 && mbIsVisible == true)
				{
					mpCowVm->Play();
					mEffectTimer = 60;
					mbIsVisible = false;
					mpCapsuleCollider->SetDeleteFlag(true);
				}

				if (!mbIsVisible)
				{
					mEffectTimer--;
				}

				if (mEffectTimer <= 0 && !mbIsVisible)
				{
					Die(DEATH_VACUUM);
				}
			}
		}
		else
		{
			if (mvPosition.y > mfdeathTime)
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
	mCowtDelete = true;
}

void CowMove::Die(DeathReason reason)
{
	if (mDeleteFlag || mCowtDelete) return;

	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);

	switch (reason)
	{
	case DEATH_VACUUM:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->mpCombo->AddHit();
			player->mpScore->AddScore(mfScore * player->mpCombo->GetMultiplier());

			// コンボロジック
			if (mntag_cow == CowMove::Tag_cow::Cow_T)
			{
				Master::mnTutorialcount++;
			}

			s_mnTagCountCow++;
			if (s_mnTagCountCow == 1)
			{
				s_tag1Cow = mntag_cow;
			}
			else if (s_mnTagCountCow == 2 && s_tag1Cow == mntag_cow)
			{
				s_tag2Cow = mntag_cow;
			}
			else if (s_mnTagCountCow == 3 && s_tag2Cow == mntag_cow)
			{
				s_tag3Cow = mntag_cow;
				if (s_tag3Cow == CowMove::Cow_1) player->mpScore->AddScore(300);
				if (s_tag2Cow == CowMove::Cow_2) player->mpScore->AddScore(600);
			}
			else
			{
				s_mnTagCountCow = 0;
				s_tag1Cow = CowMove::none;
				s_tag2Cow = CowMove::none;
				s_tag3Cow = CowMove::none;
			}
		}
		mCowtDelete = true;
		break;

	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->mpCombo->AddHit();
			player->mpScore->AddScore(mfScore * player->mpCombo->GetMultiplier());
		}
		mCowtDelete = true;
		break;

	case DEATH_LIMIT:
		mCowtDelete = true;
		break;
	}
}