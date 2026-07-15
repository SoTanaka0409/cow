import os

cow_2_cpp = """#include "Cow_2.h"
#include "CapsuleCollider.h"

Cow_2::Cow_2(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 20;
	mfXp = 10;
	collider_radius_ = 50.0f;
	SetTagCow(CowMove::kCow2);
}

Cow_2::~Cow_2()
{
}
"""

cow_tutorial_cpp = """#include "Cow_Tutorial.h"

Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 30;
	// 短時間でレベルアップを体験させるため経験値を高く設定する
	mfXp = 150;
	SetTagCow(CowMove::kCowT);
}

Cow_Tutorial::~Cow_Tutorial()
{
}
"""

creaturemanager_h = """#pragma once

#include <vector>
#include <map>
#include <utility>

// 動的生成されたキャラクターのリストとプールを一括管理するテンプレートクラス
template <typename TMove, typename TTag>
class CreatureManager
{
public:
	CreatureManager()
	{
		mCreatures.reserve(500);
	}

	virtual ~CreatureManager()
	{
		// アクティブなキャラクターのメモリ管理はObjectManagerに任せるため、プールのみ解放する
		mCreatures.clear();
		for (auto& pair : mPools)
		{
			for (auto creature : pair.second)
			{
				delete creature;
			}
		}
		mPools.clear();
	}

	/*
	 * 全てのオブジェクトの更新および不要なオブジェクトのプール返却を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] オブジェクトの更新とリスト整理の実行
	 */
	void Update()
	{
		for (auto creature : mCreatures)
		{
			creature->Update();
		}
		Erase();
	}

	/*
	 * 描画処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw()
	{
	}

	/*
	 * 削除フラグの立ったオブジェクトを管理リストから除外しプールへ返す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] リストからの除外とプールへの追加
	 */
	void Erase()
	{
		if (!mCreatures.empty())
		{
			for (auto it = mCreatures.begin(); it != mCreatures.end();)
			{
				if ((*it)->GetCharacterDelete())
				{
					auto creature = *it;
					creature->Deactivate();
					
					TTag tag = GetTag(creature);
					mPools[tag].push_back(creature);
					it = mCreatures.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
	}

protected:
	virtual TTag GetTag(TMove* creature) = 0;

	/*
	 * プールからの復帰または新規生成を行い、リストに追加する
	 * [入力] tag: 識別タグ, spawnPos: 出現座標, scale: 拡大率, args: コンストラクタ引数
	 * [出力] 生成または復帰したオブジェクトのポインタ
	 * [副作用] リストへの追加とオブジェクトの初期化
	 */
	template <typename TConcrete, typename... Args>
	TConcrete* SpawnAndInit(TTag tag, VECTOR spawnPos, float scale, Args&&... args)
	{
		TConcrete* creature = nullptr;
		if (!mPools[tag].empty())
		{
			creature = static_cast<TConcrete*>(mPools[tag].back());
			mPools[tag].pop_back();
			creature->Reset(spawnPos);
		}
		else
		{
			creature = new TConcrete(std::forward<Args>(args)...);
		}
		creature->SetScale(scale);
		mCreatures.push_back(creature);
		return creature;
	}

	std::vector<TMove*> mCreatures;
	std::map<TTag, std::vector<TMove*>> mPools;
};
"""

files = {
    'Source/Cow_2.cpp': cow_2_cpp,
    'Source/Cow_Tutorial.cpp': cow_tutorial_cpp,
    'Source/CreatureManager.h': creaturemanager_h
}

for path, content in files.items():
    with open(path, 'w', encoding='utf-8-sig') as f:
        f.write(content)
