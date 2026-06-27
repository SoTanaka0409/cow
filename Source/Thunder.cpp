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
	hasStunned = false;
	pos = pos;
	// プレイヤーが回避行動をとれるよう、落下前に1秒間の猶予を設ける
	warningTimer = 60;
	strikeTimer = 20;
	intervalTimer = 180;

	state = IDLE;
	active = true;
	
	capsuleCollider->radius = 230;

	thunder = new EffekseerEffect("Resource/3D/EFK/thud.efk", pos, 200.0f);
	thunder->SetScale(VGet(1.0f, 1.0f, 1.0f));

	warning = new EffekseerEffect("Resource/3D/EFK/warning2.efk", pos, 40.0f);
	stun = new EffekseerEffect("Resource/3D/EFK/stun.efk", pos, 20.0f);
}

Thunder::~Thunder()
{
	// エフェクトは自前管理のため手動で解放する
	if (thunder != nullptr)
	{
		delete thunder;
		thunder = nullptr;
	}

	if (warning != nullptr)
	{
		delete warning;
		warning = nullptr;
	}

	if (stun != nullptr)
	{
		delete stun;
		stun = nullptr;
	}
}

void Thunder::Update()
{
	if (thunder != nullptr)
	{
		thunder->SetPosition(pos);
		thunder->Update();
	}

	if (warning != nullptr)
	{
		warning->SetPosition(pos);
		warning->Update();
	}

	if (stun != nullptr)
	{
		stun->Update();
	}

	if (!active) return;

	capsuleCollider->mvPosition = VSub(mvPosition, VGet(0, 2000, 0));
	capsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0, 2000, 0));
   
	switch (state)
	{
	case IDLE:
		intervalTimer--;
		if (intervalTimer <= 0)
		{
			float range = 3000.0f;
			pos.x = (float)(GetRand((int)range * 2) - (int)range);
			pos.z = (float)(GetRand((int)range * 2) - (int)range);
			pos.y = 0.0f;

			mvPosition = pos;
			warningTimer = 60;
			state = WARNING;

			if (warning != nullptr)
			{
				warning->Play();
			}
		}
		break;

	case WARNING:
		warningTimer--;
		if (warningTimer <= 0)
		{
			state = STRIKE;
			strikeTimer = 30;
			hasStunned = false;
			
			if (thunder != nullptr)
			{
				thunder->Play();
				
				auto players = ServiceLocator::GetPlayers();
				bool playSound = false;
				for (auto p : players)
				{
					VECTOR diff = VSub(p->GetPosition(), pos);
					if (VSquareSize(diff) < 3000.0f * 3000.0f)
					{
						playSound = true;
						break;
					}
				}
				if (playSound)
				{
					Master::soundManager->PlaySE(SoundManager::SE_KAMINARI);
				}
			}
		}
		break;

	case STRIKE:
		strikeTimer--;
		if (strikeTimer <= 0)
		{
			intervalTimer = 120;
			state = IDLE;
		}
		break;
	}

	if (stunEffectTimer > 0)
	{
		stunEffectTimer--;

		// エフェクトの再生時間が短いため、スタン期間中は定期的に再生し直す
		if (stunEffectTimer > 0 && stunEffectTimer % 30 == 0)
		{
			if (stun != nullptr)
			{
				stun->Play();
			}
		}
	}
}

void Thunder::Draw()
{
}

bool Thunder::IsActive() const
{
	return active;
}

bool Thunder::CheckHit(VECTOR playerPos, float range)
{
	if (state != STRIKE) return false;

	VECTOR diff = VSub(playerPos, pos);
	float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	return distance < range;
}

void Thunder::OnEnter(Collider* collider, Collider* check)
{
	if (state != STRIKE) return;
	// 多段ヒットによる理不尽なスタン延長を防ぐため
	if (hasStunned) return;

	if (check->parentObject->GetTag() == Tag3D_player)
	{
		Player3D* player = dynamic_cast<Player3D*>(check->parentObject);
		if (player != nullptr)
		{
			hasStunned = true;
			stunEffectTimer = 120;

			VECTOR playerPos = player->GetPosition();
			stun->SetPosition(playerPos);
			stun->Play();

			player->ApplyStun(120);

			// 落雷の威力を視覚的に強調するためカメラシェイクを発生させる
			Master::camera->SetupShake(30.0f, 45.0f, 40.0f);
		}
	}
}

void Thunder::OnTrigger(Collider* collider, Collider* check)
{
}

void Thunder::OnExit(Collider* collider, Collider* check)
{
}
