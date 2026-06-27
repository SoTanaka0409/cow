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
	// 暫定対応: コンボボーナス計算用のグローバル変数群
	int s_mnTagCount = 0;
	AnimalMove::Tag_animal s_tag1 = AnimalMove::none;
	AnimalMove::Tag_animal s_tag2 = AnimalMove::none;
	AnimalMove::Tag_animal s_tag3 = AnimalMove::none;
}

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	// 基礎パラメータとして羊の定数を適用
	speed = GameConstants::ANIMAL_SHEEP.speed;
	actionTimer = 60;
	score = GameConstants::ANIMAL_SHEEP.score;
	xp = GameConstants::ANIMAL_SHEEP.xp;
	baitFlag = false;
	mfdeathTime = GameConstants::ANIMAL_SHEEP.deathTimeHeight;
	SetTag(Object3D::Tag3D_Animal);
}

AnimalMove::~AnimalMove()
{
}

void AnimalMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	if (capsuleCollider != nullptr)
	{
		capsuleCollider->mvPosition = pos;
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
	// 餌オブジェクトへの接触を検知し、誘導フラグを立てる
	if (collider == capsuleCollider && check->parentObject != nullptr)
	{
		if (check->parentObject->GetTag() == Tag3D_Bait)
		{
			baitFlag = true;
		}
	}
}

void AnimalMove::OnTrigger(Collider* collider, Collider* check)
{
}

void AnimalMove::OnExit(Collider* collider, Collider* check)
{
	// 餌の有効範囲外に出たため誘導フラグを解除する
	if (collider == capsuleCollider && check->parentObject != nullptr)
	{
		if (check->parentObject->GetTag() == Tag3D_Bait)
		{
			baitFlag = false;
		}
	}
}

void AnimalMove::CharacterDied()
{
	// 演出都合上、フィーバー中および吸い込み状態以外では死亡判定を行わない
	if (currentState != STATE_VACUUM||ServiceLocator::GetFever()->IsFever()) return;

	Player3D* player = targetPlayer;

	CharacterRotate();
	if (player != nullptr)
	{
		mvPosition.y += player->Status(Player3D::Status_AttackS);
	}

	// プレイヤーへ向けて浮遊し、一定高度に達した段階で捕獲完了とする
	if (mvPosition.y > mfdeathTime && !deleteFlag)
	{
		Die(DEATH_VACUUM);
	}

	model->SetPosition(mvPosition);
}

void AnimalMove::Die(DeathReason reason)
{
	if (deleteFlag) return;

	Player3D* player = targetPlayer;

	switch (reason)
	{
	case DEATH_VACUUM:
	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->level->AddXp(xp);
			player->combo->Reset();
			player->score->AddScore(score);

			// 暫定対応: 同種連続捕獲時に追加経験値を付与するためのコンボロジック
			if (mntag_animal == AnimalMove::Tag_animal::Animal_T)
			{
				Master::tutorialCount++;
			}

			s_mnTagCount++;
			if (s_mnTagCount == 1)
			{
				s_tag1 = mntag_animal;
			}
			else if (s_mnTagCount == 2 && s_tag1 == mntag_animal)
			{
				s_tag2 = mntag_animal;
			}
			else if (s_mnTagCount == 3 && s_tag2 == mntag_animal)
			{
				s_tag3 = mntag_animal;
				if (s_tag3 == AnimalMove::Animal_1) player->level->AddXp(10);
				if (s_tag2 == AnimalMove::Animal_2) player->level->AddXp(20);
				if (s_tag3 == AnimalMove::Animal_3) player->level->AddXp(30);
			}
			else
			{
				s_mnTagCount = 0;
				s_tag1 = AnimalMove::none;
				s_tag2 = AnimalMove::none;
				s_tag3 = AnimalMove::none;
			}
		}
		deleteFlag = true;
		break;

	case DEATH_LIMIT:
		deleteFlag = true;
		break;
	}
}
