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
	, currentState(STATE_IDLE)
	, characterState(new StateIdle())
	, actionTimer(60)
	, speed(10.0f)
	, targetAngle(0.0f)
	, angle(0.0f)
	, moveVec(VGet(0.0f, 0.0f, 0.0f))
	, oldmoveVec(VGet(0.0f, 0.0f, 0.0f))
	, hitPos(VGet(0.0f, 0.0f, 0.0f))
	, vacuumTimer(0)
	, deleteFlag(false)
	, mfdeathTime(1000.0f)
	, score(0.0f)
	, xp(0.0f)
	, baitFlag(false)
	, isVisible(true)
{
	model = new Model(filename, initPos, false);

	// 初期向きをランダムに分散させる
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	model->SetRotation(mvRotation);
}

CharacterMove::~CharacterMove()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
	if (characterState != nullptr)
	{
		delete characterState;
		characterState = nullptr;
	}
}

void CharacterMove::Reset(VECTOR pos)
{
	mvPosition = pos;
	currentState = STATE_IDLE;
	targetPlayer = nullptr;

	// 現在のStateを一度破棄し、新たに待機状態を作成
	if (characterState != nullptr)
	{
		delete characterState;
	}
	characterState = new StateIdle();

	actionTimer = 60;
	mvRotation.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	oldmoveVec = VGet(0.0f, 0.0f, 0.0f);
	vacuumTimer = 0;
	deleteFlag = false;
	baitFlag = false;
	isVisible = true;
	SetDrawFlag(true); // 描画を有効化

	// 管理クラス(ObjectManager)に自身を再度登録
	Master::sceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

	if (model != nullptr)
	{
		model->SetPosition(pos);
		model->SetRotation(mvRotation);
	}

	// 当たり判定（コライダー）を再度有効化して登録
	if (capsuleCollider != nullptr)
	{
		capsuleCollider->SetDeleteFlag(false);
		ColliderManager::GetInstance()->AddCollider(capsuleCollider);
	}
}

// ==============================================================================
// Deactivate (オブジェクトの無効化)
// 死亡時や画面外に出た時など、描画と判定をオフにして処理対象から外します。
// ==============================================================================
void CharacterMove::Deactivate()
{
	isVisible = false;
	SetDrawFlag(false); // 描画を無効化

	// 当たり判定を無効化
	if (capsuleCollider != nullptr)
	{
		capsuleCollider->SetDeleteFlag(true);
	}

	// deleteはせず、Updateの更新対象リストからのみ外す（再利用のため）
	Master::sceneManager->GetCurrentScene()->GetObjectManager()->RemoveObjectNoDelete(this);
}

void CharacterMove::Update()
{
	MoveCharacter();
	CharacterDied();

	if (currentState != STATE_VACUUM)
	{
		RotationCharacter();
	}

	ColliderMove();
	model->Update();
}

void CharacterMove::Draw()
{
	model->Draw();
}

void CharacterMove::MoveCharacter()
{
	if (currentState == STATE_VACUUM) return;

	mvOldPosition = mvPosition;

	UpdateWanderAI();
	CheckWallCollision();

	model->SetPosition(mvPosition);
}

void CharacterMove::UpdateWanderAI()
{
	if (characterState != nullptr)
	{
		characterState->Update(this);
	}
}

void CharacterMove::ChangeState(CharacterState* newState)
{
	if (characterState != nullptr)
	{
		characterState->Exit(this);
		delete characterState;
	}
	characterState = newState;
	if (characterState != nullptr)
	{
		characterState->Enter(this);
	}
}

void CharacterMove::ChangeStateToVacuum()
{
	currentState = STATE_VACUUM;
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
						mvPosition = VAdd(mvPosition, VScale(slide, speed * Master::GetDeltaTimeScaler()));
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
	if (capsuleCollider != nullptr)
	{
		capsuleCollider->mvPosition = mvPosition;
		capsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		capsuleCollider->radius = 50.0f;
	}
}

void CharacterMove::RotationCharacter()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);

		// モデルの正面方向の仕様に合わせて180度反転させる
		mvRotation.y = targetAngle + DX_PI_F;
		model->SetRotation(mvRotation);
	}
}

void CharacterMove::CharacterRotate()
{
	mvRotation.y += 0.1f;

	if (mvRotation.y > DX_PI_F * 2.0f)
	{
		mvRotation.y -= DX_PI_F * 2.0f;
	}
	model->SetRotation(mvRotation);
}

void CharacterMove::SetScale(float scale)
{
	if (model != nullptr)
	{
		model->SetScale(scale);
	}
}

void CharacterMove::CharacterDied()
{
	//各キャラに書き込む
}

void CharacterMove::Die(DeathReason reason)
{
	if (deleteFlag) return;

	switch (reason)
	{
	}
}
