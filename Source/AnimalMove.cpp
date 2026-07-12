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
	// æš«å®šå?E??E?•K—v ã‚?E?ãƒ³ãƒœãEãƒ¼ãƒŠã‚¹è¨ˆç?E?—ç”¨ã?E?ã‚?E?ãƒ­ãƒ¼ãƒ?Eƒ«å¤‰æ•°?E?E?
	int s_mnTagCount = 0;
	AnimalMove::TagAnimal s_tag1 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag2 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag3 = AnimalMove::kNone;
}

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	// åŸºç¤Žãƒ‘ãƒ©ãƒ¡ãƒ¼ã‚?E?ã?E?ã—ã¦ç¾ŠãEå®šæ•°ã‚’é?Eç”¨
	mfSpeed = GameConstants::kAnimalSheep.speed;
	mActionTimer = 60;
	mfScore = GameConstants::kAnimalSheep.score;
	mfXp = GameConstants::kAnimalSheep.xp;
	mbBaitFlag = false;
	death_timer_ = GameConstants::kAnimalSheep.death_time_height;
	SetTag(Object3D::kTag3dAnimal);
}

AnimalMove::~AnimalMove()
{
}

void AnimalMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
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
	// é¤Œã‚ªãƒ–ã‚¸ã‚?E?ã‚?E?ãƒˆã¸ã?E?æŽ¥è§¦ã‚’æ¤?E??ã—ã€è?E?˜å°Žãƒ•ãƒ©ã‚?E?ã‚’ç?E?‹ã¦ã‚E
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
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
	// é¤Œã?Eæœ‰åŠ¹ç¯E?E??–ã«å‡?E?ãŸãŸã‚è?E?˜å°Žãƒ•ãƒ©ã‚?E?ã‚’è§?E?é™?E?ã™ã?E?E
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			mbBaitFlag = false;
		}
	}
}

void AnimalMove::CharacterDied()
{
	// æ¼”å?Eéƒ½åˆä¸Šã€ãƒ•ã‚?E?ãƒ¼ãƒ?EEä¸?E?ãŠã?E?ã?E?å?E?ãE?E??E?ã?E?çŠ?E?æ…‹ä?E??E?å¤–ã§ã?E?æ­E??äº?E?åˆ?E?å®šã?E?è¡Œã‚ã?E?ãE
	auto fv = ServiceLocator::GetFever();
	if (mCurrentState != STATE_VACUUM || (fv && fv->IsFever())) return;

	Player3D* player = mpTargetPlayer;

	CharacterRotate();
	if (player != nullptr)
	{
		position_.y += player->Status(Player3D::Status_AttackS);
	}

	// ãƒ—ãƒ¬ã‚?E?ãƒ¤ãƒ¼ã?E?å‘ã?E?ã?E?æµ?E?éŠã?E?ã€ä?E?€å®šé«˜åº?E?ã?E?é”ã?E?ãŸæ®?E?éšŽã§æ?E²å®Œä?E?E¨ã™ã?E?E
	if (position_.y > death_timer_ && !mDeleteFlag)
	{
		Die(DEATH_VACUUM);
	}

	model_->SetPosition(position_);
}

void AnimalMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	Player3D* player = mpTargetPlayer;

	switch (reason)
	{
	case DEATH_VACUUM:
	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->combo_->Reset();
			player->mpScore->AddScore(mfScore);

			// æš«å®šå?E??E?•K—v åŒç¨?E?é€?E?ç¶šæ•ç?E?æ™‚ã«è¿?E?åŠ çµŒéE??“å€?E?ã‚?E???E??Žã?E??‹ãŸã‚ã?Eã‚?E?ãƒ³ãƒœãƒ­ã‚?E?ãƒ?E‚ŸE
			if (tag_animal_ == AnimalMove::TagAnimal::kAnimalT)
			{
				Master::mnTutorialcount++;
			}

			s_mnTagCount++;
			if (s_mnTagCount == 1)
			{
				s_tag1 = tag_animal_;
			}
			else if (s_mnTagCount == 2 && s_tag1 == tag_animal_)
			{
				s_tag2 = tag_animal_;
			}
			else if (s_mnTagCount == 3 && s_tag2 == tag_animal_)
			{
				s_tag3 = tag_animal_;
				if (s_tag3 == AnimalMove::kAnimal1) player->mpLevel->AddXp(10);
				if (s_tag2 == AnimalMove::kAnimal2) player->mpLevel->AddXp(20);
				if (s_tag3 == AnimalMove::kAnimal3) player->mpLevel->AddXp(30);
			}
			else
			{
				s_mnTagCount = 0;
				s_tag1 = AnimalMove::kNone;
				s_tag2 = AnimalMove::kNone;
				s_tag3 = AnimalMove::kNone;
			}
		}
		mDeleteFlag = true;
		break;

	case DEATH_LIMIT:
		mDeleteFlag = true;
		break;
	}
}