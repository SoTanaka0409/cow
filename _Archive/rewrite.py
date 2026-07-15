import codecs

cpp_content = '''#include "ServiceLocator.h"
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
	mfXp = 20;
	mfScore = 30;
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
				fv->StartFever(mpTargetPlayer);
			}
		}
	}
}
'''

h_content = '''#pragma once
#include "CowMove.h"

class GoldCow : public CowMove
{
public:
	// オブジェクトの振る舞いを切り替えるための状態定義
	enum TagFever
	{
		kFever,
		kNoFever,
	};

public:
	/*
	 * ゴールドカウの初期化を行う
	 * [入力] filename: モデルのファイルパス, initPos: 初期位置, fever: フィーバー由来のスポーンか
	 * [出力] なし
	 * [副作用] なし
	 */
	GoldCow(std::string filename, VECTOR initPos, TagFever fever);

	/*
	 * デストラクタ
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~GoldCow();

	/*
	 * 状態をリセットし、再配置する
	 * [入力] pos: 再配置する位置
	 * [出力] なし
	 * [副作用] 死亡タイマーを初期化する
	 */
	virtual void Reset(VECTOR pos) override;

	/*
	 * 毎フレームの更新処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 寿命に達した際に自身を破棄する
	 */
	void Update() override;

	/*
	 * 死亡時の処理を行う
	 * [入力] reason: 死亡の理由
	 * [出力] なし
	 * [副作用] 条件を満たす場合、フィーバー状態をトリガーする
	 */
	void Die(DeathReason reason) override;

	/*
	 * プール再利用時にフィーバー状態を上書き設定する
	 * [入力] fever: 新しいフィーバー状態
	 * [出力] なし
	 * [副作用] fever_を更新する
	 */
	void SetFever(TagFever fever) { fever_ = fever; }

private:
	TagFever fever_;
	int death_count_;
	int death_timer_;
};
'''

with codecs.open('Source/GoldCow.cpp', 'w', 'utf-8-sig') as f:
    f.write(cpp_content)

with codecs.open('Source/GoldCow.h', 'w', 'utf-8-sig') as f:
    f.write(h_content)
