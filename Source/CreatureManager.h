#pragma once

#include <vector>
#include <map>
#include <utility>

// 蜍慕噪逕滓・縺輔ｌ縺溘く繝｣繝ｩ繧ｯ繧ｿ繝ｼ・育央・蜍慕黄・峨・繝ｪ繧ｹ繝医→繝励・繝ｫ繧剃ｸ蜈・ｮ｡逅・☆繧九ユ繝ｳ繝励Ξ繝ｼ繝医け繝ｩ繧ｹ
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
		// 繧｢繧ｯ繝・ぅ繝悶↑繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ・・Creatures・峨・ ObjectManager 縺檎ｮ｡逅・・delete 縺吶ｋ縺溘ａ縲・
		// 縺薙％縺ｧ delete 縺吶ｋ縺ｨ莠碁㍾隗｣謾ｾ繧ｨ繝ｩ繝ｼ・・ull繧ｨ繝ｩ繝ｼ・峨↓縺ｪ繧九・縺ｧ clear 縺ｮ縺ｿ陦後≧縲・
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
	 * @brief 邂｡逅・＠縺ｦ縺・ｋ蜈ｨ縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・譖ｴ譁ｰ縺翫ｈ縺ｳ荳崎ｦ√が繝悶ず繧ｧ繧ｯ繝医・繝励・繝ｫ霑泌唆繧定｡後≧
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 場合が繝悶ず繧ｧ繧ｯ繝医・Update螳溯｡後→縲・rase()縺ｮ螳溯｡・
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
	 * @brief 謠冗判蜃ｦ逅・ｼ育樟蝨ｨ縺ｯ螟夜Κ縺ｧ陦後▲縺ｦ縺・ｋ縺溘ａ遨ｺ・・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Draw()
	{
	}

	/*
	 * @brief 蜑企勁繝輔Λ繧ｰ(mDeleteFlag)縺檎ｫ九▲縺ｦ縺・ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ邂｡逅・Μ繧ｹ繝医°繧蛾勁螟悶＠繝励・繝ｫ縺ｸ霑斐☆
	 * [蜈･蜉嫋 
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繝ｪ繧ｹ繝医°繧峨・髯､螟悶√・繝ｼ繝ｫ縺ｸ縺ｮ霑ｽ蜉
	 */
	void Erase()
	{
		if (!mCreatures.empty())
		{
			for (auto it = mCreatures.begin(); it != mCreatures.end();)
			{
				// CowMove 縺ｨ AnimalMove 縺ｯ縺・★繧後ｂ CharacterMove 繧堤ｶ呎価縺励※縺翫ｊ GetCharacterDelete() 縺御ｽｿ縺医ｋ
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
	// 豢ｾ逕溘け繝ｩ繧ｹ縺ｧ繧ｿ繧ｰ蜿門ｾ怜・逅・ｒ螳溯｣・☆繧・
	virtual TTag GetTag(TMove* creature) = 0;

	/*
	 * @brief 繝励・繝ｫ縺九ｉ縺ｮ蠕ｩ蟶ｰ縺ｾ縺溘・譁ｰ隕冗函謌舌ｒ陦後＞縲√Μ繧ｹ繝医↓霑ｽ蜉縺吶ｋ蜈ｱ騾壼・逅・
	 * [蜈･蜉嫋 tag: 隴伜挨繧ｿ繧ｰ, spawnPos: 蜃ｺ迴ｾ蠎ｧ讓・ scale: 諡｡螟ｧ邇・ args: 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ蠑墓焚
	 * [蜃ｺ蜉嫋 逕滓・縺ｾ縺溘・蠕ｩ蟶ｰ縺励◆繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ
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
