#include "CowMove.h"
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

CowMove::CowMove(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(10.0f)
	, mnActionTimer(0)
	, mActionTimer(60)
	, mCurrentState(STATE_IDLE)
	, mfdeathTime(1900.0f)
	, mfScore(10)
	, mfXp(10)
	, mbBaitFlag(false)
	, mbIsJudged(false)
	, mCowtDelete(false)
{
	mbIsVisible = true;
	mEffectTimer = 0;
	SetTag(Object3D::Tag3D_Cow);

	mpModel = new Model(filename, initPos, false);

	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
	{
		mfdeathTime = 700.0f; // チュートリアル用の限界高度
	}

	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	mpModel->SetRotation(mvRotation);

	mpCowVm = new EffekseerEffect("Resource/3D/EFK/NowVm.efk", mvPosition, 180.0f);
}

CowMove::~CowMove()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}

	if (mpCowVm != nullptr)
	{
		delete mpCowVm;
		mpCowVm = nullptr;
	}
}

void CowMove::Update()
{
	MoveCow();
	CowDied();

	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCow();
	}

	if (mpCowVm != nullptr)
	{
		mpCowVm->Update();
	}

	ColliderMove();
	mpModel->Update();
}

void CowMove::Draw()
{
	if (mbIsVisible)
	{
		mpModel->Draw();
	}
}

void CowMove::CowDied()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);

	if (mCurrentState == STATE_VACUUM)
	{
		if (mIsDead)
		{
			mEffectTimer--;
			if (mEffectTimer <= 0)
			{
				SetDeleteFlag(true);
				mpCapsuleCollider->SetDeleteFlag(true);
				mCowtDelete = true;
			}
			return;
		}

		CowRotate();
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
		else
		{
			mvPosition.y += 6.0f;
		}

		if (mpCowVm != nullptr)
		{
			mpCowVm->SetPosition(mvPosition);
		}

		if (mvPosition.y > mfdeathTime && !mIsDead)
		{
			mIsDead = true;
			Master::mnCaughtCowCount++;
			Master::mpSoundManager->PlaySE(SoundManager::SE_COW);
			mbIsVisible = false;
			mEffectTimer = 60; 
			if (mpCowVm != nullptr)
			{
				mpCowVm->Play();
			}

			if (player != nullptr)
			{
				player->mpLevel->AddXp(mfXp);
				player->mpCombo->AddHit();
				player->mpScore->AddScore(mfScore + player->mpCombo->GetMultiplier());
			}

			if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
			{
				Master::TutrialVacumFlag = true;
			}
		}

		mpModel->SetPosition(mvPosition);
		return;
	}

	if (mIsDead)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
		SetDeleteFlag(true);
	}
}

void CowMove::KilledByBait()
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

		if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TUTORIAL)
		{
			Master::TutrialVacumFlag = true;
		}
	}

	mIsDead = true;
	mpCapsuleCollider->SetDeleteFlag(true);
	SetDeleteFlag(true);
}

void CowMove::ColliderMove()
{
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
	mpCapsuleCollider->mfRadius = mColliderRadius;
}

void CowMove::MoveCow()
{
	AvoidOtherCows();

	if (SeekBait())
	{
		return;
	}

	UpdateWanderAI();
	CheckWallCollision();

	mpModel->SetPosition(mvPosition);
}

void CowMove::AvoidOtherCows()
{
	if (mCurrentState == STATE_VACUUM) return;

	const auto& cList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Cow);
	for (auto obj : cList)
	{
		if (obj == this) continue;
		CowMove* otherCow = dynamic_cast<CowMove*>(obj);
		if (otherCow && otherCow->GetCurrentState() == STATE_VACUUM) continue;
		
		VECTOR otherPos = obj->GetPosition();
		VECTOR dir = VSub(mvPosition, otherPos);
		dir.y = 0.0f;
		float lengthSq = VSquareSize(dir);
		if (lengthSq < 400.0f * 400.0f)
		{
			if (lengthSq < 0.001f)
			{
				dir = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				if (VSquareSize(dir) < 0.001f) dir = VGet(1.0f, 0.0f, 0.0f);
			}
			dir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(dir, 15.0f));
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
		return true;
	}
	return false;
}

void CowMove::UpdateWanderAI()
{
	if (mCurrentState == STATE_VACUUM) return;

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

	if (mCurrentState == STATE_WALK)
	{
		mvPosition = VAdd(mvPosition, VScale(moveVec, mfSpeed));
		mvPosition.y -= 7.0f;
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
}

void CowMove::CheckWallCollision()
{
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
}

void CowMove::RotationCow()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);
		mvRotation.y = targetAngle + DX_PI_F;
		mpModel->SetRotation(mvRotation);
	}
}

void CowMove::SetScale(float scale)
{
	mpModel->SetScale(scale);
}

void CowMove::CowRotate()
{
	float rotateSpeed = 0.1f;
	mvRotation.y += rotateSpeed;
	if (mvRotation.y > DX_PI_F * 2.0f)
	{
		mvRotation.y -= DX_PI_F * 2.0f;
	}
	mpModel->SetRotation(mvRotation);
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
