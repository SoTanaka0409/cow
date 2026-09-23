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

/// @brief 動物の初期化
/// @param filename モデルパス
/// @param init_pos 初期座標
/// @details 各種ステータスの初期設定
AnimalMove::AnimalMove(const std::string& filename, VECTOR init_pos)
	: CharacterMove(filename, init_pos)
{
	speed_ = GameConstants::kAnimalSheep.speed;
	action_timer_ = 60;
	score_ = GameConstants::kAnimalSheep.score;
	xp_ = GameConstants::kAnimalSheep.xp;
	bait_flag_ = false;
	death_timer_ = GameConstants::kAnimalSheep.death_time_height;
	SetTag(Object3D::kTag3dAnimal);
}

/// @brief 終了処理
AnimalMove::~AnimalMove()
{
}

/// @brief 状態のリセット
/// @param pos 再配置する座標
/// @details キャラクターとコライダーの座標を更新
void AnimalMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
	}
}

/// @brief 動物の移動処理
/// @details 座標の更新
void AnimalMove::MoveCharacter()
{
	CharacterMove::MoveCharacter();
}

/// @brief アニメーションの追加
/// @param state アニメーション状態
/// @param filename ファイルパス
void AnimalMove::AddAnimation(AnimationState state, const std::string& filename)
{
}

/// @brief 接触時の処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
/// @details 餌に接触した場合、誘引フラグを有効化
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

/// @brief 接触中の処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
void AnimalMove::OnTrigger(Collider* collider, Collider* check)
{
}

/// @brief 接触終了時の処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
/// @details 餌から離れた場合、誘引フラグを無効化
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

/// @brief 死亡(捕獲)判定と演出
/// @details 吸い込み演出の進行と捕獲完了時の死亡処理の呼び出し
void AnimalMove::CharacterDied()
{
	auto fv = ServiceLocator::GetFever();
	// 通常状態では捕獲演出へ移行させないため
	if (ai_state_ != kStateVacuum || (fv && fv->IsFever())) return;

	Player3D* player = target_player_;

	CharacterRotate();
	if (player != nullptr)
	{
		position_.y += player->Status(Player3D::kStatusAttackS) * Master::GetDeltaTimeScaler();
	}

	// UFOへの吸い込み演出を完遂したか判定するため
	if (position_.y > death_timer_ && !mDeleteFlag)
	{
		Die(kDeathVacuum);
	}

	model_->SetPosition(position_);
}

/// @brief 死亡時の処理
/// @param reason 死亡理由
/// @details 経験値・スコアの加算、コンボの更新、削除フラグの有効化
void AnimalMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	Player3D* player = target_player_;

	switch (reason)
	{
	case kDeathVacuum:
	case kDeathBait:
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

	case kDeathLimit:
		mDeleteFlag = true;
		break;
	}
}
