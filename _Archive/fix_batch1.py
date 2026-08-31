import os

combo_h = """#pragma once

// ボーナススコア計算やUI表示のためコンボ状態を管理する
class Combo
{
public:
	Combo();
	~Combo();

	/*
	 * UIに現在のコンボ数とロゴを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * コンボ継続判定のため毎フレーム状態を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 時間切れ時にReset()を呼び出しコンボ状態を破棄する
	 */
	void Update();

	/*
	 * 獲物獲得時にコンボ状態を更新・維持する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] combo_count_をインクリメントし、タイマーを初期値に戻す
	 */
	void AddHit();

	/*
	 * コンボ猶予時間切れなどで状態を初期化する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] combo_count_を0にリセットする
	 */
	void Reset();

	int GetCombo() const;
	float GetMultiplier() const;

private:
	int combo_count_;
	float combo_timer_;
	float combo_max_time_;

	int combo_image_;
	bool combo_show_;
	float combo_show_timer_;
};
"""

cow_cpp = """#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"

Cow::Cow(std::string filename, VECTOR initPos, float num)
	: CowMove(filename, initPos)
{
	mfScore = num;
	mfXp = 10;
	SetTagCow(CowMove::kCow1);
	mCurrentState = STATE_IDLE;
}

Cow::~Cow()
{
}
"""

cow_h = """#pragma once
#include "CowMove.h"

class Cow : public CowMove
{
public:
	/*
	 * Cow_1の標準キャラクターを初期化する
	 * [入力] filename: モデルパス, initPos: 初期配置座標, num: 基礎スコア値
	 * [出力] なし
	 * [副作用] スコアと経験値を設定する
	 */
	Cow(std::string filename, VECTOR initPos, float num);
	virtual ~Cow();
};
"""

cowmanager_cpp = """#include "ServiceLocator.h"
#include "CowManager.h"
#include "CowMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Cow.h"
#include "Cow_2.h"
#include "Cow_Tutorial.h"
#include "GoldCow.h"
#include "CapsuleCollider.h"

CowManager::CowManager()
{
}

void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever)
{
	for (int i = 0; i < count; i++)
	{
		// パフォーマンス維持のため、牛は最大30匹に制限する
		if (mCreatures.size() >= 30)
		{
			if (tag == CowMove::kCowGold)
			{
				// 金の牛を優先して出現させるため、プレイヤーから一番遠い牛を削除して枠を空ける
				bool erased = false;
				float maxDistSq = -1.0f;
				auto furthestIt = mCreatures.end();
				
				VECTOR playerPos = Master::camera_->GetPosition();

				for (auto it = mCreatures.begin(); it != mCreatures.end(); ++it)
				{
					if ((*it)->GetTagCow() != CowMove::kCowGold)
					{
						VECTOR cowPos = (*it)->GetPosition();
						float dx = cowPos.x - playerPos.x;
						float dy = cowPos.y - playerPos.y;
						float dz = cowPos.z - playerPos.z;
						float distSq = dx * dx + dy * dy + dz * dz;

						if (distSq > maxDistSq)
						{
							maxDistSq = distSq;
							furthestIt = it;
						}
					}
				}

				if (furthestIt != mCreatures.end())
				{
					(*furthestIt)->Die(DEATH_LIMIT);
					auto cow = *furthestIt;
					cow->Deactivate();
					mPools[cow->GetTagCow()].push_back(cow);
					mCreatures.erase(furthestIt);
					erased = true;
				}
				else if (!mCreatures.empty())
				{
					// 全て金の牛の場合は最古の牛を削除して枠を空ける
					mCreatures.front()->Die(DEATH_LIMIT);
					auto cow = mCreatures.front();
					cow->Deactivate();
					mPools[cow->GetTagCow()].push_back(cow);
					mCreatures.erase(mCreatures.begin());
					erased = true;
				}

				if (!erased) break;
			}
			else
			{
				break;
			}
		}

		float randX = (float)(GetRand(pos.x) - pos.x / 2);
		float randZ = (float)(GetRand(pos.z) - pos.z / 2);
		VECTOR spawnPos = VGet(randX, pos.y, randZ);

		if (tag == CowMove::kCow1)
		{
			SpawnAndInit<Cow>(tag, spawnPos, scale, filename, spawnPos, 1.0f);
		}
		else if (tag == CowMove::kCow2)
		{
			SpawnAndInit<Cow_2>(tag, spawnPos, scale, filename, spawnPos);
		}
		else if (tag == CowMove::kCowGold)
		{
			auto feverMode = is_fever ? GoldCow::kFever : GoldCow::kNoFever;
			auto cow = SpawnAndInit<GoldCow>(tag, spawnPos, scale, filename, spawnPos, feverMode);
			cow->SetFever(feverMode);
		}
		else if (tag == CowMove::kCowT)
		{
			SpawnAndInit<Cow_Tutorial>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}
"""

cowmanager_h = """#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"
#include "CreatureManager.h"

// 牛の動的生成やオブジェクトプールを管理するクラス
class CowManager : public CreatureManager<CowMove, CowMove::TagCow>
{
public:
	CowManager();

	/*
	 * 指定された種類の牛をランダムな位置に生成する
	 * [入力] filename: モデルパス, pos: 基準座標, scale: 拡大率, tag: 種類タグ, count: 生成数, is_fever: フィーバー中かどうか
	 * [出力] なし
	 * [副作用] 生成された牛が管理リストに追加されるか、プールから復帰する
	 */
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever = false);

protected:
	CowMove::TagCow GetTag(CowMove* creature) override { return creature->GetTagCow(); }
};
"""

files = {
    'Source/Combo.h': combo_h,
    'Source/Cow.cpp': cow_cpp,
    'Source/Cow.h': cow_h,
    'Source/CowManager.cpp': cowmanager_cpp,
    'Source/CowManager.h': cowmanager_h
}

for path, content in files.items():
    with open(path, 'w', encoding='utf-8-sig') as f:
        f.write(content)
