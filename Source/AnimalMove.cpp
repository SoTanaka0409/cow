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
	// 隴会ｽｫ陞ｳ螢ｼ・ｯ・ｾ陟｢繝ｻ 郢ｧ・ｳ郢晢ｽｳ郢晄㈱繝ｻ郢晢ｽｼ郢晉ｿｫ縺幃坎閧ｲ・ｮ遉ｼ逡醍ｸｺ・ｮ郢ｧ・ｰ郢晢ｽｭ郢晢ｽｼ郢晁・ﾎ晁棔逕ｻ辟夐・・､
	int s_mnTagCount = 0;
	AnimalMove::TagAnimal s_tag1 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag2 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag3 = AnimalMove::kNone;
}

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	// 陜難ｽｺ驕牙ｼｱ繝ｱ郢晢ｽｩ郢晢ｽ｡郢晢ｽｼ郢ｧ・ｿ邵ｺ・ｨ邵ｺ蜉ｱ窶ｻ驗堤ｿｫ繝ｻ陞ｳ螢ｽ辟夂ｹｧ蟶昶・騾包ｽｨ
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
	// 鬯｢蠕後′郢晄じ縺夂ｹｧ・ｧ郢ｧ・ｯ郢晏現竏育ｸｺ・ｮ隰暦ｽ･髫暦ｽｦ郢ｧ蜻茨ｽ､諛・｡咲ｸｺ蜉ｱﾂ竏ｬ・ｪ莨懶ｽｰ蠑ｱ繝ｵ郢晢ｽｩ郢ｧ・ｰ郢ｧ蝣､・ｫ荵昶ｻ郢ｧ繝ｻ
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
	// 鬯｢蠕後・隴帷甥譟鷹⊃繝ｻ蟲・棔謔ｶ竊楢怎・ｺ邵ｺ貅倪螺郢ｧ竏ｬ・ｪ莨懶ｽｰ蠑ｱ繝ｵ郢晢ｽｩ郢ｧ・ｰ郢ｧ螳夲ｽｧ・｣鬮ｯ・､邵ｺ蜷ｶ・・
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
	// 雋肴ｳ後・鬩幢ｽｽ陷ｷ莠包ｽｸ鄙ｫﾂ竏壹Ψ郢ｧ・｣郢晢ｽｼ郢晁・繝ｻ闕ｳ・ｭ邵ｺ鄙ｫ・育ｸｺ・ｳ陷ｷ・ｸ邵ｺ繝ｻ・ｾ・ｼ邵ｺ・ｿ霑･・ｶ隲ｷ蛟ｶ・ｻ・･陞滓じ縲堤ｸｺ・ｯ雎・ｽｻ闔・｡陋ｻ・､陞ｳ螢ｹ・帝勗蠕鯉ｽ冗ｸｺ・ｪ邵ｺ繝ｻ
	auto fv = ServiceLocator::GetFever();
	if (mCurrentState != STATE_VACUUM || (fv && fv->IsFever())) return;

	Player3D* player = mpTargetPlayer;

	CharacterRotate();
	if (player != nullptr)
	{
		position_.y += player->Status(Player3D::Status_AttackS);
	}

	// 郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ・ｸ陷ｷ莉｣・邵ｺ・ｦ雎ｬ・ｮ鬩慕ｿｫ・邵ｲ竏ｽ・ｸﾂ陞ｳ螟撰ｽｫ莨懶ｽｺ・ｦ邵ｺ・ｫ鬩墓鱒・邵ｺ貊難ｽｮ・ｵ鬮ｫ蠑ｱ縲定ｬ先・菴占楜蠕｡・ｺ繝ｻ竊堤ｸｺ蜷ｶ・・
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
			player->mpCombo->Reset();
			player->mpScore->AddScore(mfScore);

			// 隴会ｽｫ陞ｳ螢ｼ・ｯ・ｾ陟｢繝ｻ 陷ｷ讙趣ｽｨ・ｮ鬨ｾ・｣驍ｯ螢ｽ鮟定ｿｯ・ｲ隴弱ｅ竊馴恆・ｽ陷会｣ｰ驍ｨ遒・ｽｨ轣伉・､郢ｧ蜑・ｽｻ蛟・ｽｸ蠑ｱ笘・ｹｧ荵昶螺郢ｧ竏壹・郢ｧ・ｳ郢晢ｽｳ郢晄㈱ﾎ溽ｹｧ・ｸ郢昴・縺・
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