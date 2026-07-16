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
	int s_mnTagCount = 0;
	AnimalMove::TagAnimal s_tag1 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag2 = AnimalMove::kNone;
	AnimalMove::TagAnimal s_tag3 = AnimalMove::kNone;
}

/*
 * 動物の初期化
 * [入力] filename: モデルパス, initPos: 初期座標
 * [出力] なし
 * [副作用] 各種ステータスの初期設定
 */
AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	speed_ = GameConstants::kAnimalSheep.speed;
	mActionTimer = 60;
	score_ = GameConstants::kAnimalSheep.score;
	xp_ = GameConstants::kAnimalSheep.xp;
	bait_flag_ = false;
	death_timer_ = GameConstants::kAnimalSheep.death_time_height;
	SetTag(Object3D::kTag3dAnimal);
}

/*
 * 終了処理
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
AnimalMove::~AnimalMove()
{
}

/*
 * 状態のリセット
 * [入力] pos: 再配置する座標
 * [出力] なし
 * [副作用] キャラクターとコライダーの座標を更新
 */
void AnimalMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
	}
}

/*
 * 動物の移動処理
 * [入力] なし
 * [出力] なし
 * [副作用] 座標の更新
 */
void AnimalMove::MoveCharacter()
{
	CharacterMove::MoveCharacter();
}

/*
 * アニメーションの追加
 * [入力] state: アニメーション状態, filename: ファイルパス
 * [出力] なし
 * [副作用] なし
 */
void AnimalMove::AddAnimation(AnimationState state, std::string filename)
{
}

/*
 * 接触時の処理
 * [入力] collider: 自身のコライダー, check: 相手のコライダー
 * [出力] なし
 * [副作用] 餌に接触した場合、誘引フラグを有効化
 */
void AnimalMove::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			bait_flag_ = true;
		}
	}
}

/*
 * 接触中の処理
 * [入力] collider: 自身のコライダー, check: 相手のコライダー
 * [出力] なし
 * [副作用] なし
 */
void AnimalMove::OnTrigger(Collider* collider, Collider* check)
{
}

/*
 * 接触終了時の処理
 * [入力] collider: 自身のコライダー, check: 相手のコライダー
 * [出力] なし
 * [副作用] 餌から離れた場合、誘引フラグを無効化
 */
void AnimalMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			bait_flag_ = false;
		}
	}
}

/*
 * 死亡(捕獲)判定と演出
 * [入力] なし
 * [出力] なし
 * [副作用] 吸い込み演出の進行と捕獲完了時の死亡処理の呼び出し
 */
void AnimalMove::CharacterDied()
{
	auto fv = ServiceLocator::GetFever();
	// 通常状態では捕獲演出へ移行させないため
	if (mCurrentState != STATE_VACUUM || (fv && fv->IsFever())) return;

	Player3D* player = target_player_;

	CharacterRotate();
	if (player != nullptr)
	{
		position_.y += player->Status(Player3D::Status_AttackS) * Master::GetDeltaTimeScaler();
	}

	// UFOへの吸い込み演出を完遂したか判定するため
	if (position_.y > death_timer_ && !mDeleteFlag)
	{
		Die(DEATH_VACUUM);
	}

	model_->SetPosition(position_);
}

/*
 * 死亡時の処理
 * [入力] reason: 死亡理由
 * [出力] なし
 * [副作用] 経験値・スコアの加算、コンボの更新、削除フラグの有効化
 */
void AnimalMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	Player3D* player = target_player_;

	switch (reason)
	{
	case DEATH_VACUUM:
	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->level_manager_->AddXp(xp_);
			player->combo_->Reset();
			player->score_manager_->AddScore(static_cast<int>(score_));

			if (tag_animal_ == AnimalMove::TagAnimal::kAnimalT)
			{
				Master::tutorial_count_++;
			}

			// 同種の連続捕獲ボーナスを判定・付与するため
			s_mnTagCount++;
			if (s_mnTagCount == 1)
			{
				s_tag1 = tag_animal_;
			}
			else if (s_mnTagCount == 2 && s_tag1 == tag_animal_)
			{
				s_tag2 = tag_animal_;
			}
			else if (s_mnTagCount == 3 && s_tag2 == tag_animal_)
			{
				s_tag3 = tag_animal_;
				if (s_tag3 == AnimalMove::kAnimal1) player->level_manager_->AddXp(10);
				if (s_tag2 == AnimalMove::kAnimal2) player->level_manager_->AddXp(20);
				if (s_tag3 == AnimalMove::kAnimal3) player->level_manager_->AddXp(30);
			}
			else
			{
				s_mnTagCount = 0;
				s_tag1 = AnimalMove::kNone;
				s_tag2 = AnimalMove::kNone;
				s_tag3 = AnimalMove::kNone;
			}
		}
		mDeleteFlag = true;
		break;

	case DEATH_LIMIT:
		mDeleteFlag = true;
		break;
	}
}
