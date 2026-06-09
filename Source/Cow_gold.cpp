#include "Cow_gold.h"
#include "CapsuleCollider.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Bait.h"
#include "Wall.h"

Cow_gold::Cow_gold(std::string filename, VECTOR initPos, Tag_fever fever)
	: CowMove(filename, initPos)
	, mnFever(fever)
	, DeathCount(0)
	, DeathTimer(1200)
{
	SetTag_cow(CowMove::Cow_gold);
	mfXp = 20;
	mfScore = 30;
	mColliderRadius = 150.0f;
}

Cow_gold::~Cow_gold()
{
}

void Cow_gold::Update()
{
	DeathCount++;
	MoveCow();
	
	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCow();
	}

	ColliderMove();
	CowDied();
	mpModel->Update();

	// フィーバー用金の牛で、フィーバーが終了したか生存時間を超えた場合は自動消滅させる
	if (mnFever == fever && (Master::mpSceneManager->GetCurrentScene()->mpFever->IsFever() == false || DeathCount >= DeathTimer))
	{
		SetDeleteFlag(true);
		mpCapsuleCollider->SetDeleteFlag(true);
	}
}

void Cow_gold::CowDied()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	if (player == nullptr) return; 

	if (mCurrentState == STATE_VACUUM)
	{
		if (mIsDead)
		{
			if (mpCowVm != nullptr)
			{
				mpCowVm->Update();
			}
			return;
		}

		CowRotate();
		mvPosition.y += player->Status(Player3D::Status_AttackS);

		VECTOR playerPos = player->GetPosition();
		float followSpeed = 0.15f;

		if (this->mnFever == fever)
		{
			mvPosition.x += (playerPos.x - mvPosition.x) * followSpeed;
			mvPosition.z += (playerPos.z - mvPosition.z) * followSpeed;
		}

		if (mvPosition.y > mfdeathTime && !mCowtDelete)
		{
			if (mpCowVm != nullptr)
			{
				mpCowVm->SetPosition(mvPosition);
				mpCowVm->Play();
			}

			mIsDead = true;
			mEffectTimer = 30;

			Master::mpSoundManager->PlaySE(SoundManager::SE_COW);
			SetDeleteFlag(true);
			mpCapsuleCollider->SetDeleteFlag(true);

			player->mpLevel->AddXp(mfXp);
			player->mpCombo->AddHit();
			player->mpScore->AddScore(mfScore + player->mpCombo->GetMultiplier());

			if (this->mnFever == Nofever)
			{
				Master::mpSceneManager->GetCurrentScene()->mpFever->StartFever();
			}
			mCowtDelete = true;
		}

		mpModel->SetPosition(mvPosition);
	}
}

void Cow_gold::KilledByBait()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	if (player != nullptr)
	{
		Master::mnCaughtCowCount++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_COW);
		player->mpLevel->AddXp(mfXp);
		player->mpCombo->AddHit();
		player->mpScore->AddScore(mfScore + player->mpCombo->GetMultiplier());

		if (this->mnFever == Nofever)
		{
			Master::mpSceneManager->GetCurrentScene()->mpFever->StartFever();
		}

		if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
		{
			Master::TutrialVacumFlag = true;
		}
	}

	mIsDead = true;
	mpCapsuleCollider->SetDeleteFlag(true);
	SetDeleteFlag(true);
}

void Cow_gold::MoveCow()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	mvOldPosition = mvPosition;

	if (mCurrentState != STATE_VACUUM)
	{
		if (mbBaitFlag)
		{
			const auto& b = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Bait);
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
			return;
		}

		mActionTimer--;
		if (mActionTimer <= 0)
		{
			if (GetRand(100) < 50)
			{
				mCurrentState = STATE_WALK;
				float angle = GetRand(359) * DX_PI_F / 180.0f;
				moveVec.x = sinf(angle);
				moveVec.z = cosf(angle);
				mActionTimer = 60 + GetRand(120);
			}
			else
			{
				mCurrentState = STATE_IDLE;
				mActionTimer = 60 + GetRand(60);
			}
		}
	}

	if (mCurrentState == STATE_WALK)
	{
		mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
		mvPosition.y -= 4.0f;
		if (mvPosition.y <= 0)
		{
			mvPosition.y = 0;
		}
	}
	else if (mCurrentState == STATE_IDLE)
	{
		mvPosition.y -= 4.0f;
		if (mvPosition.y <= 0)
		{
			mvPosition.y = 0;
		}
	}	

	// 壁との衝突判定
	bool hitwall = false;
	bool hitwalls = false;
	const auto& walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall);
	if (!walls.empty())
	{
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				if (HitCheck_Capsule_Triangle(
					mvPosition,
					VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition,
						VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));
					if (hitwall && !hitwalls)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					else if (hitwalls)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}
	}

	mpModel->SetPosition(mvPosition);
}
