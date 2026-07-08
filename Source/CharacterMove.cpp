#include "CharacterMove.h"
#include "CharacterState.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Wall.h"
#include "Player3D.h"
#include "CapsuleCollider.h"
#include <cmath>
#include "ServiceLocator.h"
#include "Scene.h"
#include "ColliderManager.h"

CharacterMove::CharacterMove(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mCurrentState(STATE_IDLE)
	, mpCurrentState(new StateIdle())
	, mActionTimer(60)
	, mfSpeed(10.0f)
	, mfTargetAngle(0.0f)
	, mfAngle(0.0f)
	, moveVec(VGet(0.0f, 0.0f, 0.0f))
	, oldmoveVec(VGet(0.0f, 0.0f, 0.0f))
	, hitPos(VGet(0.0f, 0.0f, 0.0f))
	, mVacuumTimer(0)
	, mDeleteFlag(false)
	, mfdeathTime(1000.0f)
	, mfScore(0.0f)
	, mfXp(0.0f)
	, mbBaitFlag(false)
	, mbIsVisible(true)
{
	mpModel = new Model(filename, initPos, false);

	// 初期向きをランダムに分散させる
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	mpModel->SetRotation(mvRotation);
}

CharacterMove::~CharacterMove()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
	if (mpCurrentState != nullptr)
	{
		delete mpCurrentState;
		mpCurrentState = nullptr;
	}
}

void CharacterMove::Reset(VECTOR pos)
{
	mvPosition = pos;
	mCurrentState = STATE_IDLE;
	mpTargetPlayer = nullptr;

	// 現在のStateを一度破棄し、新たに待機状態を作成
	if (mpCurrentState != nullptr)
	{
		delete mpCurrentState;
	}
	mpCurrentState = new StateIdle();

	mActionTimer = 60;
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	oldmoveVec = VGet(0.0f, 0.0f, 0.0f);
	mVacuumTimer = 0;
	mDeleteFlag = false;
	mbBaitFlag = false;
	mbIsVisible = true;
	SetDrawFlag(true); // 描画を有効化

	// 管理クラス(ObjectManager)に自身を再度登録
	if (auto scene = Master::mpSceneManager->GetCurrentScene())
	{
		if (auto objMgr = scene->GetObjectManager())
		{
			objMgr->AddObject(this);
		}
	}

	if (mpModel != nullptr)
	{
		mpModel->SetPosition(pos);
		mpModel->SetRotation(mvRotation);
	}

	// 当たり判定（コライダー）を再度有効化して登録
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(false);
		ColliderManager::GetInstance()->AddCollider(mpCapsuleCollider);
	}
}

// ==============================================================================
// Deactivate (オブジェクトの無効化)
// 死亡時や画面外に出た時など、描画と判定をオフにして処理対象から外します。
// ==============================================================================
void CharacterMove::Deactivate()
{
	mbIsVisible = false;
	SetDrawFlag(false); // 描画を無効化

	// 当たり判定を無効化
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->SetDeleteFlag(true);
	}

	// deleteはせず、Updateの更新対象リストからのみ外す（再利用のため）
	if (auto scene = Master::mpSceneManager->GetCurrentScene())
	{
		if (auto objMgr = scene->GetObjectManager())
		{
			objMgr->RemoveObjectNoDelete(this);
		}
	}
}

void CharacterMove::Update()
{
	MoveCharacter();
	CharacterDied();

	if (mCurrentState != STATE_VACUUM)
	{
		RotationCharacter();
	}

	ColliderMove();
	mpModel->Update();
}

void CharacterMove::Draw()
{
	mpModel->Draw();
}

void CharacterMove::MoveCharacter()
{
	if (mCurrentState == STATE_VACUUM) return;

	mvOldPosition = mvPosition;

	UpdateWanderAI();
	CheckWallCollision();

	mpModel->SetPosition(mvPosition);
}

void CharacterMove::UpdateWanderAI()
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Update(this);
	}
}

void CharacterMove::ChangeState(CharacterState* newState)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Exit(this);
		delete mpCurrentState;
	}
	mpCurrentState = newState;
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Enter(this);
	}
}

void CharacterMove::ChangeStateToVacuum()
{
	mCurrentState = STATE_VACUUM;
	ChangeState(new StateVacuum());
}

void CharacterMove::CheckWallCollision()
{
	bool hitwall = false;
	bool hitwalls = false;

	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall);
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
					// 複数壁に挟まれた場合は進行を止める
					else if (hitwalls)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}
	}
}

void CharacterMove::ColliderMove()
{
	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = mvPosition;
		mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		mpCapsuleCollider->mfRadius = 50.0f;
	}
}

void CharacterMove::RotationCharacter()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);

		// モデルの正面方向の仕様に合わせて180度反転させる
		mvRotation.y = targetAngle + DX_PI_F;
		mpModel->SetRotation(mvRotation);
	}
}

void CharacterMove::CharacterRotate()
{
	mvRotation.y += 0.1f;

	if (mvRotation.y > DX_PI_F * 2.0f)
	{
		mvRotation.y -= DX_PI_F * 2.0f;
	}
	mpModel->SetRotation(mvRotation);
}

void CharacterMove::SetScale(float scale)
{
	if (mpModel != nullptr)
	{
		mpModel->SetScale(scale);
	}
}

void CharacterMove::CharacterDied()
{
}

void CharacterMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	switch (reason)
	{
	}
}