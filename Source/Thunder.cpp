#include "ServiceLocator.h"
#include "Thunder.h"
#include <cmath>
#include "CapsuleCollider.h"
#include "dxlib.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

Thunder::Thunder(VECTOR pos)
	: Object3D(pos)
{
	mHasStunned = false;
	pos_ = pos;
	// 繝励Ξ繧､繝､繝ｼ縺悟屓驕ｿ陦悟虚繧偵→繧後ｋ繧医≧縲∬誠荳句燕縺ｫ1遘帝俣縺ｮ迪ｶ莠医ｒ險ｭ縺代ｋ
	mWarningTimer = 60;
	mStrikeTimer = 20;
	mIntervalTimer = 180;

	mState = IDLE;
	mActive = true;
	
	capsule_collider_->radius_ = 230;

	mpThunder = new EffekseerEffect("Resource/3D/EFK/thud.efk", pos_, 200.0f);
	mpThunder->SetScale(VGet(1.0f, 1.0f, 1.0f));

	mpWarning = new EffekseerEffect("Resource/3D/EFK/warning2.efk", pos_, 40.0f);
	mpStun = new EffekseerEffect("Resource/3D/EFK/stun.efk", pos_, 20.0f);
}

Thunder::~Thunder()
{
	// 繧ｨ繝輔ぉ繧ｯ繝医・閾ｪ蜑咲ｮ｡逅・・縺溘ａ謇句虚縺ｧ隗｣謾ｾ縺吶ｋ
	if (mpThunder != nullptr)
	{
		delete mpThunder;
		mpThunder = nullptr;
	}

	if (mpWarning != nullptr)
	{
		delete mpWarning;
		mpWarning = nullptr;
	}

	if (mpStun != nullptr)
	{
		delete mpStun;
		mpStun = nullptr;
	}
}

void Thunder::Update()
{
	if (mpThunder != nullptr)
	{
		mpThunder->SetPosition(pos_);
		mpThunder->Update();
	}

	if (mpWarning != nullptr)
	{
		mpWarning->SetPosition(pos_);
		mpWarning->Update();
	}

	if (mpStun != nullptr)
	{
		mpStun->Update();
	}

	if (!mActive) return;

	capsule_collider_->position_ = VSub(position_, VGet(0, 2000, 0));
	capsule_collider_->position2_ = VAdd(position_, VGet(0, 2000, 0));
   
	switch (mState)
	{
	case IDLE:
		mIntervalTimer--;
		if (mIntervalTimer <= 0)
		{
			float range = 3000.0f;
			pos_.x = (float)(GetRand((int)range * 2) - (int)range);
			pos_.z = (float)(GetRand((int)range * 2) - (int)range);
			pos_.y = 0.0f;

			position_ = pos_;
			mWarningTimer = 60;
			mState = WARNING;

			if (mpWarning != nullptr)
			{
				mpWarning->Play();
			}
		}
		break;

	case WARNING:
		mWarningTimer--;
		if (mWarningTimer <= 0)
		{
			mState = STRIKE;
			mStrikeTimer = 30;
			mHasStunned = false;
			
			if (mpThunder != nullptr)
			{
				mpThunder->Play();
				
				auto players = ServiceLocator::GetPlayers();
				bool playSound = false;
				for (auto p : players)
				{
					VECTOR diff = VSub(p->GetPosition(), pos_);
					if (VSquareSize(diff) < 3000.0f * 3000.0f)
					{
						playSound = true;
						break;
					}
				}
				if (playSound)
				{
					Master::mpSoundManager->PlaySE(SoundManager::kSeKaminari);
				}
			}
		}
		break;

	case STRIKE:
		mStrikeTimer--;
		if (mStrikeTimer <= 0)
		{
			mIntervalTimer = 120;
			mState = IDLE;
		}
		break;
	}

	if (mStunEffectTimer > 0)
	{
		mStunEffectTimer--;

		// 繧ｨ繝輔ぉ繧ｯ繝医・蜀咲函譎る俣縺檎洒縺・◆繧√√せ繧ｿ繝ｳ譛滄俣荳ｭ縺ｯ螳壽悄逧・↓蜀咲函縺礼峩縺・
		if (mStunEffectTimer > 0 && mStunEffectTimer % 30 == 0)
		{
			if (mpStun != nullptr)
			{
				mpStun->Play();
			}
		}
	}
}

void Thunder::Draw()
{
}

bool Thunder::IsActive() const
{
	return mActive;
}

bool Thunder::CheckHit(VECTOR playerPos, float range)
{
	if (mState != STRIKE) return false;

	VECTOR diff = VSub(playerPos, pos_);
	float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	return distance < range;
}

void Thunder::OnEnter(Collider* collider, Collider* check)
{
	if (mState != STRIKE) return;
	// 螟壽ｮｵ繝偵ャ繝医↓繧医ｋ逅・ｸ榊ｰｽ縺ｪ繧ｹ繧ｿ繝ｳ蟒ｶ髟ｷ繧帝亟縺舌◆繧・
	if (mHasStunned) return;

	if (check->parent_object_->GetTag() == kTag3dPlayer)
	{
		Player3D* player = dynamic_cast<Player3D*>(check->parent_object_);
		if (player != nullptr)
		{
			mHasStunned = true;
			mStunEffectTimer = 120;

			VECTOR playerPos = player->GetPosition();
			mpStun->SetPosition(playerPos);
			mpStun->Play();

			player->ApplyStun(120);

			// 關ｽ髮ｷ縺ｮ螽∝鴨繧定ｦ冶ｦ夂噪縺ｫ蠑ｷ隱ｿ縺吶ｋ縺溘ａ繧ｫ繝｡繝ｩ繧ｷ繧ｧ繧､繧ｯ繧堤匱逕溘＆縺帙ｋ
			Master::mpCamera->SetupShake(30.0f, 45.0f, 40.0f);
		}
	}
}

void Thunder::OnTrigger(Collider* collider, Collider* check)
{
}

void Thunder::OnExit(Collider* collider, Collider* check)
{
}