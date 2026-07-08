#include "ServiceLocator.h"
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
#include "Cow_gold.h"
#include "CapsuleCollider.h"

/*
 * @brief 邂｡逅・が繝悶ず繧ｧ繧ｯ繝医・蛻晄悄蛹悶ｒ陦後≧
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
CowManager::CowManager()
{
}



/*
 * @brief 謖・ｮ壹＆繧後◆遞ｮ鬘槭・迚帙ｒ逕滓・縺ｾ縺溘・繝励・繝ｫ縺九ｉ蜀榊茜逕ｨ縺励※驟咲ｽｮ縺吶ｋ
 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝輔ぃ繧､繝ｫ, pos: 蜃ｺ迴ｾ蝓ｺ貅門ｺｧ讓・ scale: 諡｡螟ｧ邇・ tag: 迚帙・繧ｿ繧ｰ, count: 逕滓・謨ｰ, mfever: 繝輔ぅ繝ｼ繝舌・繝輔Λ繧ｰ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 迚帙・繝｡繝｢繝ｪ遒ｺ菫昴♀繧医・mCows縺ｸ縺ｮ霑ｽ蜉縲√∪縺溘・繝励・繝ｫ縺九ｉ縺ｮ蜿悶ｊ蜃ｺ縺・
 */
void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool mfever)
{
	for (int i = 0; i < count; i++)
	{
		// 繝代ヵ繧ｩ繝ｼ繝槭Φ繧ｹ邯ｭ謖√・縺溘ａ縲∝酔譎ょ・迴ｾ謨ｰ繧呈怙螟ｧ30蛹ｹ縺ｫ蛻ｶ髯舌☆繧・
		if (mCreatures.size() >= 30)
		{
			if (tag == CowMove::kCowGold)
			{
				// 驥代・迚帙ｒ遒ｺ螳溘↓蜃ｺ迴ｾ縺輔○繧九◆繧√√・繝ｬ繧､繝､繝ｼ縺九ｉ譛繧る□縺・勸騾壹・迚帙ｒ蜆ｪ蜈医＠縺ｦ遐ｴ譽・＠譫繧堤ｩｺ縺代ｋ
				bool erased = false;
				float maxDistSq = -1.0f;
				auto furthestIt = mCreatures.end();
				
				VECTOR playerPos = Master::mpCamera->GetPosition();

				for (auto it = mCreatures.begin(); it != mCreatures.end(); ++it)
				{
					if ((*it)->GetTag_cow() != CowMove::kCowGold)
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
					// 譌｢縺ｫ蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺ｦ縺・ｋ迚帙↑縺ｩ縺ｯObjectManager蛛ｴ縺ｧ豸医＆繧後ｋ
					(*furthestIt)->Die(DEATH_LIMIT);
					auto cow = *furthestIt;
					cow->Deactivate();
					mPools[cow->GetTag_cow()].push_back(cow);
					mCreatures.erase(furthestIt);
					erased = true;
				}
				else if (!mCreatures.empty())
				{
					// 蜈ｨ縺ｦ縺ｮ迚帙′逕ｻ髱｢蜀・↑縺ｩ縺ｮ蝣ｴ蜷医∽ｸ逡ｪ蜿､縺・ｂ縺ｮ縺ｮ蜑企勁繝輔Λ繧ｰ繧堤ｫ九※縺ｦ繝ｪ繧ｹ繝医°繧蛾勁螟悶☆繧・
					mCreatures.front()->Die(DEATH_LIMIT);
					auto cow = mCreatures.front();
					cow->Deactivate();
					mPools[cow->GetTag_cow()].push_back(cow);
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
			auto feverMode = mfever ? Cow_gold::kFever : Cow_gold::kNoFever;
			auto cow = SpawnAndInit<Cow_gold>(tag, spawnPos, scale, filename, spawnPos, feverMode);
			cow->SetFever(feverMode);
		}
		else if (tag == CowMove::kCowT)
		{
			SpawnAndInit<Cow_Tutorial>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}

