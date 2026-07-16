#include "ServiceLocator.h"
#include "GoldCow.h"
#include "CapsuleCollider.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Bait.h"
#include "Wall.h"

/*
 * ゴールドカウの初期化を行う
 * [入力] filename: モデルのファイルパス, initPos: 初期位置, kFever: フィーバー由来のスポーンか
 * [出力] なし
 * [副作用] スコア、経験値、当たり判定のサイズを設定する
 */
GoldCow::GoldCow(std::string filename, VECTOR initPos, TagFever kFever)
	: CowMove(filename, initPos)
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

/*
 * デストラクタ
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
GoldCow::~GoldCow()
{
}

/*
 * 状態をリセットし、再配置する
 * [入力] pos: 再配置する位置
 * [出力] なし
 * [副作用] 死亡タイマーを初期化する
 */
void GoldCow::Reset(VECTOR pos)
{
	CowMove::Reset(pos);
	death_count_ = 0;
}

/*
 * 毎フレームの更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] 寿命に達した際に自身を破棄する
 */
void GoldCow::Update()
{
	death_count_++;
	CowMove::Update();

	// フィーバー限定の牛がフィールドに残り続けるのを防ぐため
	auto fv = ServiceLocator::GetFever();
	if (fever_ == kFever && (fv == nullptr || fv->IsFever() == false || death_count_ >= death_timer_))
	{
		Die(DEATH_LIMIT);
	}
}

/*
 * 死亡時の処理を行う
 * [入力] reason: 死亡の理由
 * [出力] なし
 * [副作用] 条件を満たす場合、フィーバー状態をトリガーする
 */
void GoldCow::Die(DeathReason reason)
{
	if (mDeleteFlag) return;
	CowMove::Die(reason);

	// プレイヤーが通常時に倒した場合、フィーバー状態へ移行させるため
	if (reason == DEATH_VACUUM || reason == DEATH_BAIT)
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
