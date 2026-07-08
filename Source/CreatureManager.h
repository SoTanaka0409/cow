#pragma once

#include <vector>
#include <map>
#include <utility>

// 動的生成されたキャラクター（牛・動物）のリストとプールを一元管理するテンプレートクラス
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
		for (auto creature : mCreatures)
		{
			delete creature;
		}
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
	 * @brief 管理している全てのオブジェクトの更新および不要オブジェクトのプール返却を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各オブジェクトのUpdate実行と、Erase()の実行
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
	 * @brief 描画処理（現在は外部で行っているため空）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw()
	{
	}

	/*
	 * @brief 削除フラグ(mDeleteFlag)が立っているオブジェクトを管理リストから除外しプールへ返す
	 * [入力] 
	 * [出力] なし
	 * [副作用] リストからの除外、プールへの追加
	 */
	void Erase()
	{
		if (!mCreatures.empty())
		{
			for (auto it = mCreatures.begin(); it != mCreatures.end();)
			{
				// CowMove と AnimalMove はいずれも CharacterMove を継承しており GetCharacterDelete() が使える
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
	// 派生クラスでタグ取得処理を実装する
	virtual TTag GetTag(TMove* creature) = 0;

	/*
	 * @brief プールからの復帰または新規生成を行い、リストに追加する共通処理
	 * [入力] tag: 識別タグ, spawnPos: 出現座標, scale: 拡大率, args: コンストラクタ引数
	 * [出力] 生成または復帰したオブジェクトのポインタ
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
