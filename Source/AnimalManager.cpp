#include "ServiceLocator.h"
#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

/*
 * @brief 邂｡逅・が繝悶ず繧ｧ繧ｯ繝医・蛻晄悄蛹悶ｒ陦後≧
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
AnimalManager::AnimalManager()
{
}



/*
 * @brief 謖・ｮ壹＆繧後◆遞ｮ鬘槭・蜍慕黄繧堤函謌舌∪縺溘・繝励・繝ｫ縺九ｉ蜀榊茜逕ｨ縺励※驟咲ｽｮ縺吶ｋ
 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝輔ぃ繧､繝ｫ, pos: 蜃ｺ迴ｾ蝓ｺ貅門ｺｧ讓・ scale: 諡｡螟ｧ邇・ tag: 蜍慕黄縺ｮ繧ｿ繧ｰ, count: 逕滓・謨ｰ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 蜍慕黄縺ｮ繝｡繝｢繝ｪ遒ｺ菫昴♀繧医・mAnimals縺ｸ縺ｮ霑ｽ蜉縲√∪縺溘・繝励・繝ｫ縺九ｉ縺ｮ蜿悶ｊ蜃ｺ縺・
 */
void AnimalManager::SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::TagAnimal tag, int count)
{
	for (int i = 0; i < count; i++)
	{
		float randX = (float)(GetRand(pos.x) - pos.x / 2);
		float randZ = (float)(GetRand(pos.z) - pos.z / 2);
		VECTOR spawnPos = VGet(randX, 0.0f, randZ);
		if (tag == AnimalMove::kAnimal1)
		{
			SpawnAndInit<Animal>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}

