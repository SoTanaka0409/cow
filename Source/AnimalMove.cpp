#include "AnimalMove.h"
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

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(10.0f)
	, mnActionTimer(0)
	, mActionTimer(60)
	, mCurrentState(STATE_IDLE)
	, mfScore(-10)
	, mfXp(0)
	, mbBaitFlag(false)
	, mfdeathTime(1000.0f)
{
	SetTag(Object3D::Tag3D_Animal);
	mpModel = new Model(filename, initPos, false);
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	mpModel->SetRotation(mvRotation);
}

AnimalMove::~AnimalMove()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void AnimalMove::Update()
{
	MoveAnimal();

	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationAnimal();
	}

	ColliderMove();
	mpModel->Update();
}

void AnimalMove::Draw()
{
	mpModel->Draw();
}

void AnimalMove::ColliderMove()
{
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
	mpCapsuleCollider->mfRadius = 50.0f;
}

void AnimalMove::MoveAnimal()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);

	if (mCurrentState == STATE_VACUUM)
	{
		AnimalRotate();
		if (player != nullptr)
		{
			mvPosition.y += player->Status(Player3D::Status_AttackS);
		}

		if (mvPosition.y > mfdeathTime && !mDelete)
		{
			mIsDead = true;
			SetDeleteFlag(true);
			mpCapsuleCollider->SetDeleteFlag(true);

			if (player != nullptr)
			{
				player->mpLevel->AddXp(mfXp);
				player->mpCombo->Reset();
				player->mpScore->AddScore(mfScore);
			}
			mDelete = true;
		}
		else
		{
			mIsDead = false;
		}

		mpModel->SetPosition(mvPosition);
		return;
	}
	
	mActionTimer--;
	mvOldPosition = mvPosition;

	if (mActionTimer <= 0)
	{
		// 徘徊行動AI：50%の確率で徘徊、50%で静止する
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

	// 壁との衝突判定補正
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

void AnimalMove::RotationAnimal()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);
		mvRotation.y = targetAngle + DX_PI_F;
		mpModel->SetRotation(mvRotation);
	}
}

void AnimalMove::SetScale(float scale)
{
	mpModel->SetScale(scale);
}

void AnimalMove::AnimalRotate()
{
	mvRotation.y += 0.1f;
	if (mvRotation.y > DX_PI_F * 2.0f)
	{
		mvRotation.y -= DX_PI_F * 2.0f;
	}
	mpModel->SetRotation(mvRotation);
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
