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
	, target_angle_(0.0f)
	, angle_(0.0f)
	, moveVec(VGet(0.0f, 0.0f, 0.0f))
	, oldmoveVec(VGet(0.0f, 0.0f, 0.0f))
	, hitPos(VGet(0.0f, 0.0f, 0.0f))
	, mVacuumTimer(0)
	, mDeleteFlag(false)
	, death_timer_(1000.0f)
	, mfScore(0.0f)
	, mfXp(0.0f)
	, mbBaitFlag(false)
	, mbIsVisible(true)
{
	model_ = new Model(filename, initPos, false);

	// 蛻晄悄蜷代″繧偵Λ繝ｳ繝繝縺ｫ蛻・淵縺輔○繧・
	rotation_.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	model_->SetRotation(rotation_);
}

CharacterMove::~CharacterMove()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
	if (mpCurrentState != nullptr)
	{
		delete mpCurrentState;
		mpCurrentState = nullptr;
	}
}

void CharacterMove::Reset(VECTOR pos)
{
	position_ = pos;
	mCurrentState = STATE_IDLE;
	mpTargetPlayer = nullptr;

	// 迴ｾ蝨ｨ縺ｮState繧剃ｸ蠎ｦ遐ｴ譽・＠縲∵眠縺溘↓蠕・ｩ溽憾諷九ｒ菴懈・
	if (mpCurrentState != nullptr)
	{
		delete mpCurrentState;
	}
	mpCurrentState = new StateIdle();

	mActionTimer = 60;
	rotation_.y = (float)GetRand(359) * (DX_PI_F / 180.0f);
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	oldmoveVec = VGet(0.0f, 0.0f, 0.0f);
	mVacuumTimer = 0;
	mDeleteFlag = false;
	mbBaitFlag = false;
	mbIsVisible = true;
	SetDrawFlag(true); // 謠冗判繧呈怏蜉ｹ蛹・

	// 邂｡逅・け繝ｩ繧ｹ(ObjectManager)縺ｫ閾ｪ霄ｫ繧貞・蠎ｦ逋ｻ骭ｲ
	if (auto scene = Master::mpSceneManager->GetCurrentScene())
	{
		if (auto objMgr = scene->GetObjectManager())
		{
			objMgr->AddObject(this);
		}
	}

	if (model_ != nullptr)
	{
		model_->SetPosition(pos);
		model_->SetRotation(rotation_);
	}

	// 蠖薙◆繧雁愛螳夲ｼ医さ繝ｩ繧､繝繝ｼ・峨ｒ蜀榊ｺｦ譛牙柑蛹悶＠縺ｦ逋ｻ骭ｲ
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->SetDeleteFlag(false);
		ColliderManager::GetInstance()->AddCollider(capsule_collider_);
	}
}

// ==============================================================================
// Deactivate (繧ｪ繝悶ず繧ｧ繧ｯ繝医・辟｡蜉ｹ蛹・
// 豁ｻ莠｡譎ゅｄ逕ｻ髱｢螟悶↓蜃ｺ縺滓凾縺ｪ縺ｩ縲∵緒逕ｻ縺ｨ蛻､螳壹ｒ繧ｪ繝輔↓縺励※蜃ｦ逅・ｯｾ雎｡縺九ｉ螟悶＠縺ｾ縺吶・
// ==============================================================================
void CharacterMove::Deactivate()
{
	mbIsVisible = false;
	SetDrawFlag(false); // 謠冗判繧堤┌蜉ｹ蛹・

	// 蠖薙◆繧雁愛螳壹ｒ辟｡蜉ｹ蛹・
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->SetDeleteFlag(true);
	}

	// delete縺ｯ縺帙★縲ゞpdate縺ｮ譖ｴ譁ｰ蟇ｾ雎｡繝ｪ繧ｹ繝医°繧峨・縺ｿ螟悶☆・亥・蛻ｩ逕ｨ縺ｮ縺溘ａ・・
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
	model_->Update();
}

void CharacterMove::Draw()
{
	model_->Draw();
}

void CharacterMove::MoveCharacter()
{
	if (mCurrentState == STATE_VACUUM) return;

	old_position_ = position_;

	UpdateWanderAI();
	CheckWallCollision();

	model_->SetPosition(position_);
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

	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dWall);
	if (!walls.empty())
	{
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				if (HitCheck_Capsule_Triangle(
					position_,
					VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						position_,
						VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
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
						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 隍・焚螢√↓謖溘∪繧後◆蝣ｴ蜷医・騾ｲ陦後ｒ豁｢繧√ｋ
					else if (hitwalls)
					{
						position_ = old_position_;
					}
				}
			}
		}
	}
}

void CharacterMove::ColliderMove()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		capsule_collider_->radius_ = 50.0f;
	}
}

void CharacterMove::RotationCharacter()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);

		// 繝｢繝・Ν縺ｮ豁｣髱｢譁ｹ蜷代・莉墓ｧ倥↓蜷医ｏ縺帙※180蠎ｦ蜿崎ｻ｢縺輔○繧・
		rotation_.y = targetAngle + DX_PI_F;
		model_->SetRotation(rotation_);
	}
}

void CharacterMove::CharacterRotate()
{
	rotation_.y += 0.1f;

	if (rotation_.y > DX_PI_F * 2.0f)
	{
		rotation_.y -= DX_PI_F * 2.0f;
	}
	model_->SetRotation(rotation_);
}

void CharacterMove::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
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