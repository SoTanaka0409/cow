#include "ServiceLocator.h"
#include "Bait.h"
#include "Model.h"
#include "CapsuleCollider.h"
#include "GameTimer.h"
#include "CowMove.h"
#include "AnimalMove.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"

/// @brief 牛を引き寄せるための餌オブジェクトを初期化する
/// @param filename モデルのファイルパス
/// @param pos 初期座標
/// @details GameTimerとモデルを生成し、吸引コライダーのサイズを設定する
Bait::Bait(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	model_ = new Model(filename, pos, false);
	game_timer_ = new GameTimer(pos, 5, GameTimer::Tag_NoGame);
	SetTag(kTag3dBait);

	capsule_collider_->position_ = position_;
	capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 100.0f, 0.0f));
	capsule_collider_->radius_ = 1500.0f;
}

/// @brief メモリリークを防ぐため、動的確保したリソースを解放する
/// @details model_ と game_timer_ のメモリが解放される
Bait::~Bait()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
	if (game_timer_ != nullptr)
	{
		delete game_timer_;
		game_timer_ = nullptr;
	}
}

/// @brief 餌モデルの描画を行う
void Bait::Draw()
{
	model_->Draw();
}

/// @brief 餌の落下と一定時間経過後の牛のキル判定を行う
/// @details タイマー終了時、範囲内の牛を死亡させて自身を破棄する
void Bait::Update()
{
	model_->Update();
	game_timer_->Update();

	if (position_.y >= 0.0f)
	{
		position_.y -= 40.0f;
	}
	capsule_collider_->position_ = position_;
	capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 100.0f, 0.0f));

	if (game_timer_->OutTimerFlag())
	{
		const auto& c = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dCow);
		for (int i = 0; i < c.size(); i++)
		{
			CowMove* cow = dynamic_cast<CowMove*>(c.at(i));
			if (cow != nullptr && cow->GetBaitFlag())
			{
				cow->KilledByBait();
				Master::sound_manager_->PlaySE(SoundManager::kSeBaitFinal);
			}
		}

		SetDeleteFlag(true);
		capsule_collider_->SetDeleteFlag(true);
	}
	model_->SetPosition(position_);
}

/// @brief コライダー侵入時に牛をキルする処理
/// @param collider 自身のコライダー
/// @param check 侵入した相手のコライダー
/// @details タイマー終了後かつ相手が牛の場合、牛を死亡させる
void Bait::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dCow)
		{
			CowMove* cow = dynamic_cast<CowMove*>(check->parent_object_);
			if (game_timer_->OutTimerFlag() && cow != nullptr)
			{
				cow->KilledByBait();
			}
		}
	}
}
