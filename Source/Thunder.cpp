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
	mPos = pos;
	mWarningTimer = 60;   // 1秒間の警告フェーズ
	mStrikeTimer = 20;    // 0.33秒間の落雷フェーズ
	mIntervalTimer = 180; // 落雷終了から次回予兆までの間隔時間（3秒）

	mState = IDLE;
	mActive = true;
	
	mpCapsuleCollider->mfRadius = 230;

	mpThunder = new EffekseerEffect("Resource/3D/EFK/thud.efk", mPos, 200.0f);
	mpThunder->SetScale(VGet(1.0f, 1.0f, 1.0f));

	mpWarning = new EffekseerEffect("Resource/3D/EFK/warning2.efk", mPos, 40.0f);
	mpStun = new EffekseerEffect("Resource/3D/EFK/stun.efk", mPos, 20.0f);
}

Thunder::~Thunder()
{
	// 動的確保されたEffekseerエフェクト資源を破棄してメモリリークを防ぐ
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
		mpThunder->SetPosition(mPos);
		mpThunder->Update();
	}

	if (mpWarning != nullptr)
	{
		mpWarning->SetPosition(mPos);
		mpWarning->Update();
	}

	if (mpStun != nullptr)
	{
		mpStun->Update();
	}

	if (!mActive) return;

	mpCapsuleCollider->mvPosition = VSub(mvPosition, VGet(0, 2000, 0));
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0, 2000, 0));
   
	switch (mState)
	{
	case IDLE:
		mIntervalTimer--;
		if (mIntervalTimer <= 0)
		{
			// 落雷の発生地点をステージ内の広範囲からランダムに選定する
			float range = 3000.0f;
			mPos.x = (float)(GetRand((int)range * 2) - (int)range);
			mPos.z = (float)(GetRand((int)range * 2) - (int)range);
			mPos.y = 0.0f;

			mvPosition = mPos;
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
				
				auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
				if (p != nullptr)
				{
					// 落雷発生時の環境音を、プレイヤーの耳元近くのときのみ再生する
					VECTOR diff = VSub(p->GetPosition(), mPos);
					if (VSquareSize(diff) < 3000.0f * 3000.0f)
					{
						Master::mpSoundManager->PlaySE(SoundManager::SE_KAMINARI);
					}
				}
			}
		}
		break;

	case STRIKE:
		mStrikeTimer--;
		if (mStrikeTimer <= 0)
		{
			mIntervalTimer = 120; // 次回落雷までのインターバルを2秒に設定
			mState = IDLE;
		}
		break;
	}

	if (mStunEffectTimer > 0)
	{
		mStunEffectTimer--;

		// スタン中の星エフェクトが頭上で回転し続けるよう、周期的に再トリガーする
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

	VECTOR diff = VSub(playerPos, mPos);
	float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	return distance < range;
}

void Thunder::OnEnter(Collider* collider, Collider* check)
{
	if (mState != STRIKE) return;
	if (mHasStunned) return; // 1回の落雷フェーズで複数回スタンするのを防止

	if (check->mpParentObject->GetTag() == Tag3D_player)
	{
		Player3D* player = dynamic_cast<Player3D*>(check->mpParentObject);
		if (player != nullptr)
		{
			mHasStunned = true;
			mStunEffectTimer = 120; // 2秒間スタン

			VECTOR playerPos = player->GetPosition();
			mpStun->SetPosition(playerPos);
			mpStun->Play();

			// プレイヤーにスタン状態を適用し、コントロール不能にする
			player->ApplyStun(120);

			// 落雷の衝撃をプレイヤーに伝えるためにカメラを揺らす
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
