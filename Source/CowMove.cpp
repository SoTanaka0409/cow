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
	, cowtDelete(false)
{
	mfdeathTime = GameConstants::COW_DEFAULT.deathTimeHeight;
	score = GameConstants::COW_DEFAULT.score;
	xp = GameConstants::COW_DEFAULT.xp;
	baitFlag = false;

	isVisible = true;
	effectTimer = 0;
	SetTag(Object3D::Tag3D_Cow);

	if (Master::sceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
	{
		mfdeathTime = GameConstants::COW_TUTORIAL.deathTimeHeight;
	}

	cowVm = new EffekseerEffect("Resource/3D/EFK/MowVm.efk", mvPosition, 50.0f);
}

CowMove::~CowMove()
{
}

void CowMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	cowtDelete = false;
	effectTimer = 0;
	if (cowVm != nullptr)
	{
		cowVm->SetPosition(pos);
		cowVm->Stop();
	}
	if (capsuleCollider != nullptr)
	{
		capsuleCollider->mvPosition = pos;
	}
}

void CowMove::Update()
{
	CharacterMove::Update();

	if (cowVm != nullptr && isVisible == false)
	{
		cowVm->Update();
	}
}

void CowMove::Draw()
{
	if (isVisible)
	{
		CharacterMove::Draw();
	}
	if (cowVm != nullptr && isVisible == false)
	{
		cowVm->Draw();
	}
}

void CowMove::ColliderMove()
{
	if (capsuleCollider != nullptr)
	{
		capsuleCollider->mvPosition = mvPosition;
		capsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		capsuleCollider->radius = colliderRadius;
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

	model->SetPosition(mvPosition);
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
				mvPosition = VAdd(mvPosition, VScale(dir, 15.0f * Master::GetDeltaTimeScaler()));
			}
		}
	}
	model->SetPosition(mvPosition);
}

bool CowMove::SeekBait()
{
	if (Master::sceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL) return false;
	if (currentState == STATE_VACUUM) return false;

	mvOldPosition = mvPosition;

	if (baitFlag)
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
					mvPosition = VAdd(mvPosition, VScale(VNorm(diff), 25.0f * Master::GetDeltaTimeScaler()));
				}
				else
				{
					mvPosition = bait->GetPosition();
				}
			}
		}
		model->SetPosition(mvPosition);
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
	if (collider == capsuleCollider && check->parentObject != nullptr)
	{
		if (check->parentObject->GetTag() == Tag3D_Bait)
		{
			baitFlag = true;
		}

		if (check->parentObject->GetTag() == Tag3D_Cow)
		{
			VECTOR otherPos = check->parentObject->GetPosition();
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
			mvPosition = VAdd(mvPosition, VScale(dir, 3.0f * Master::GetDeltaTimeScaler()));
		}
	}
}

void CowMove::OnTrigger(Collider* collider, Collider* check)
{
	if (collider == capsuleCollider && check->parentObject != nullptr)
	{
		if (check->parentObject->GetTag() == Tag3D_Cow)
		{
			VECTOR otherPos = check->parentObject->GetPosition();
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
			mvPosition = VAdd(mvPosition, VScale(dir, 3.0f * Master::GetDeltaTimeScaler()));
		}
	}
}

void CowMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == capsuleCollider && check->parentObject != nullptr)
	{
		if (check->parentObject->GetTag() == Tag3D_Bait)
		{
			baitFlag = false;
		}
	}
}

void CowMove::CharacterDied()
{
	if (currentState == STATE_VACUUM)
	{
		Player3D* player = targetPlayer;

		CharacterRotate();
		if (player != nullptr)
		{
			mvPosition.y += player->Status(Player3D::Status_AttackS);

			if (Master::feverFlag)
			{
				float followSpeed = 0.15f;
				VECTOR playerPos = player->GetPosition();
				mvPosition.x += (playerPos.x - mvPosition.x) * followSpeed;
				mvPosition.z += (playerPos.z - mvPosition.z) * followSpeed;
			}
		}
		model->SetPosition(mvPosition);

		if (cowVm != nullptr)
		{
			if (mvPosition.y > mfdeathTime)
			{
				if (effectTimer <= 0 && isVisible == true)
				{
					cowVm->Play();
					effectTimer = 60;
					isVisible = false;
					capsuleCollider->SetDeleteFlag(true);
				}

				if (!isVisible)
				{
					effectTimer--;
				}

				if (effectTimer <= 0 && !isVisible)
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
	isVisible = false;
	Die(DEATH_BAIT);
	cowtDelete = true;
}

void CowMove::Die(DeathReason reason)
{
	if (deleteFlag || cowtDelete) return;

	Player3D* player = targetPlayer;

	switch (reason)
	{
	case DEATH_VACUUM:
		if (player != nullptr)
		{
			player->level->AddXp(xp);
			player->combo->AddHit();
			player->score->AddScore(score * player->combo->GetMultiplier());

			// 同種連続キルによるボーナススコア計算
			if (mntag_cow == CowMove::Tag_cow::Cow_T)
			{
				Master::tutorialCount++;
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
				if (s_tag3Cow == CowMove::Cow_1) player->score->AddScore(300);
				if (s_tag2Cow == CowMove::Cow_2) player->score->AddScore(600);
			}
			else
			{
				s_mnTagCountCow = 0;
				s_tag1Cow = CowMove::none;
				s_tag2Cow = CowMove::none;
				s_tag3Cow = CowMove::none;
			}
		}
		cowtDelete = true;
		break;

	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->level->AddXp(xp);
			player->combo->AddHit();
			player->score->AddScore(score * player->combo->GetMultiplier());
		}
		cowtDelete = true;
		break;

	case DEATH_LIMIT:
		cowtDelete = true;
		break;
	}
}
