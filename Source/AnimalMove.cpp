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
	// 證ｫ螳壼�E��E�蠢・ 繧�E�繝ｳ繝懊・繝ｼ繝翫せ險育�E�礼畑縺�E�繧�E�繝ｭ繝ｼ繝�EΝ螟画焚�E�E�
	int s_mnTagCount = 0;
	AnimalMove::TagAnimal s_tag1 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag2 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag3 = AnimalMove::kNone;
}

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	// 蝓ｺ遉弱ヱ繝ｩ繝｡繝ｼ繧�E�縺�E�縺励※鄒翫・螳壽焚繧帝�E逕ｨ
	mfSpeed = GameConstants::kAnimalSheep.speed;
	mActionTimer = 60;
	mfScore = GameConstants::kAnimalSheep.score;
	mfXp = GameConstants::kAnimalSheep.xp;
	mbBaitFlag = false;
	mfdeathTime = GameConstants::kAnimalSheep.death_time_height;
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
	// 鬢後が繝悶ず繧�E�繧�E�繝医∈縺�E�謗･隗ｦ繧呈､懁E��縺励∬�E�伜ｰ弱ヵ繝ｩ繧�E�繧堤�E�九※繧・
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
	// 鬢後�E譛牙柑遽・峁E��悶↓蜃�E�縺溘◆繧∬�E�伜ｰ弱ヵ繝ｩ繧�E�繧定ｧ�E�髯�E�縺吶�E�E
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
	// 貍泌�E驛ｽ蜷井ｸ翫√ヵ繧�E�繝ｼ繝�E・荳�E�縺翫�E�縺�E�蜷�E�縺・�E��E�縺�E�迥�E�諷倶�E��E�螟悶〒縺�E�豁E��莠�E�蛻�E�螳壹�E�陦後ｏ縺�E�縺・
	auto fv = ServiceLocator::GetFever();
	if (mCurrentState != STATE_VACUUM || (fv && fv->IsFever())) return;

	Player3D* player = mpTargetPlayer;

	CharacterRotate();
	if (player != nullptr)
	{
		position_.y += player->Status(Player3D::Status_AttackS);
	}

	// 繝励Ξ繧�E�繝､繝ｼ縺�E�蜷代�E�縺�E�豬�E�驕翫�E�縲∽�E�螳夐ｫ伜ｺ�E�縺�E�驕斐�E�縺滓ｮ�E�髫弱〒謐�E佐螳御�E�・→縺吶�E�E
	if (position_.y > mfdeathTime && !mDeleteFlag)
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

			// 證ｫ螳壼�E��E�蠢・ 蜷檎ｨ�E�騾�E�邯壽黒迯�E�譎ゅ↓霑�E�蜉邨碁E��灘�E�繧剁E��倁E��弱☁E��九◆繧√�E繧�E�繝ｳ繝懊Ο繧�E�繝�EぁE
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