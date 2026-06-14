#include "AnimalMove.h"
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
#include "Bait.h"
#include "TutorialScene.h"
#include "ServiceLocator.h"
#include "Player3D.h"

namespace {
	int s_mnTagCount = 0;
	AnimalMove::Tag_animal s_tag1 = AnimalMove::none;
	AnimalMove::Tag_animal s_tag2 = AnimalMove::none;
	AnimalMove::Tag_animal s_tag3 = AnimalMove::none;
}

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	mfSpeed = GameConstants::ANIMAL_SHEEP.speed;
	mActionTimer = 60;
	mfScore = GameConstants::ANIMAL_SHEEP.score;
	mfXp = GameConstants::ANIMAL_SHEEP.xp;
	mbBaitFlag = false;
	mfdeathTime = GameConstants::ANIMAL_SHEEP.deathTimeHeight;
	SetTag(Object3D::Tag3D_Animal);
}

AnimalMove::~AnimalMove()
{
}

void AnimalMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = pos;
	}
}

void AnimalMove::MoveCharacter()
{
	CharacterMove::MoveCharacter();
}

void AnimalMove::AddAnimation(AnimationState state, std::string filename)
{
}

void AnimalMove::OnEnter(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = true;
		}
	}
}

void AnimalMove::OnTrigger(Collider* collider, Collider* check)
{
}

void AnimalMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = false;
		}
	}
}

void AnimalMove::CharacterDied()
{
	if (mCurrentState != STATE_VACUUM) return;

	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);

	CharacterRotate();
	if (player != nullptr)
	{
		mvPosition.y += player->Status(Player3D::Status_AttackS);
	}

	if (mvPosition.y > mfdeathTime && !mDeleteFlag)
	{
		Die(DEATH_VACUUM);
	}

	mpModel->SetPosition(mvPosition);
}

void AnimalMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);

	switch (reason)
	{
	case DEATH_VACUUM:
	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->mpCombo->Reset();
			player->mpScore->AddScore(mfScore);

			// コンボロジック
			if (mntag_animal == AnimalMove::Tag_animal::Animal_T)
			{
				Master::mnTutorialcount++;
			}

			s_mnTagCount++;
			if (s_mnTagCount == 1)
			{
				s_tag1 = mntag_animal;
			}
			else if (s_mnTagCount == 2 && s_tag1 == mntag_animal)
			{
				s_tag2 = mntag_animal;
			}
			else if (s_mnTagCount == 3 && s_tag2 == mntag_animal)
			{
				s_tag3 = mntag_animal;
				if (s_tag3 == AnimalMove::Animal_1) player->mpLevel->AddXp(10);
				if (s_tag2 == AnimalMove::Animal_2) player->mpLevel->AddXp(20);
				if (s_tag3 == AnimalMove::Animal_3) player->mpLevel->AddXp(30);
			}
			else
			{
				s_mnTagCount = 0;
				s_tag1 = AnimalMove::none;
				s_tag2 = AnimalMove::none;
				s_tag3 = AnimalMove::none;
			}
		}
		mDeleteFlag = true;
		break;

	case DEATH_LIMIT:
		mDeleteFlag = true;
		break;
	}
}
