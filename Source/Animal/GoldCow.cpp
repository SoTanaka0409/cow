#include "ServiceLocator.h"
#include "GoldCow.h"
#include "CapsuleCollider.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Bait.h"
#include "Wall.h"

/// @brief ゴールドカウの初期化を行う
/// @param filename モデルのファイルパス
/// @param init_pos 初期位置
/// @param kFever フィーバー由来のスポーンか
/// @details スコア、経験値、当たり判定のサイズを設定する
GoldCow::GoldCow(const std::string& filename, VECTOR init_pos, TagFever kFever)
	: CowMove(filename, init_pos)
	, fever_(kFever)
	, death_count_(0)
	, death_timer_(1200)
{
	SetTagCow(CowMove::kCowGold);
	xp_ = 20;
	score_ = 30;
	// プレイヤーが触れやすくするため、当たり判定を大きく設定する
	collider_radius_ = 150.0f;
}

/// @brief デストラクタ
GoldCow::~GoldCow()
{
}

/// @brief 状態をリセットし、再配置する
/// @param pos 再配置する位置
/// @details 死亡タイマーを初期化する
void GoldCow::Reset(VECTOR pos)
{
	CowMove::Reset(pos);
	death_count_ = 0;
}

/// @brief 毎フレームの更新処理
/// @details 寿命に達した際に自身を破棄する
void GoldCow::Update()
{
	death_count_++;
	CowMove::Update();

	// フィーバー限定の牛がフィールドに残り続けるのを防ぐため
	auto fv = ServiceLocator::GetFever();
	if (fever_ == kFever && (fv == nullptr || fv->IsFever() == false || death_count_ >= death_timer_))
	{
		Die(kDeathLimit);
	}
}

/// @brief 死亡時の処理を行う
/// @param reason 死亡の理由
/// @details 条件を満たす場合、フィーバー状態をトリガーする
void GoldCow::Die(DeathReason reason)
{
	if (mDeleteFlag) return;
	CowMove::Die(reason);

	// プレイヤーが通常時に倒した場合、フィーバー状態へ移行させるため
	if (reason == kDeathVacuum || reason == kDeathBait)
	{
		if (this->fever_ == kNoFever)
		{
			if (auto fv = ServiceLocator::GetFever())
			{
				fv->StartFever(target_player_);
			}
		}
	}
}
