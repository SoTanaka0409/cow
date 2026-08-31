#include "ServiceLocator.h"
#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

/// @brief 入力: なし
AnimalManager::AnimalManager()
{
}

/// @brief 初期化処理を行う
/// @details filename (モデル)
/// @details pos (基準座標)
/// @details scale (倍率)
/// @details tag (動物種別)
/// @details count (生成数)
/// @details オブジェクトのメモリ確保(またはプールからの再利用)と管理リストへの追加
void AnimalManager::SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::TagAnimal tag, int count, float scatterRadius)
{
	for (int i = 0; i < count; i++)
	{
		float randX = pos.x;
		float randZ = pos.z;
		if (count > 1 || scatterRadius > 200.0f)
		{
			if (scatterRadius > 0.0f)
			{
				randX += (float)(GetRand((int)(scatterRadius * 2.0f)) - scatterRadius);
				randZ += (float)(GetRand((int)(scatterRadius * 2.0f)) - scatterRadius);
			}
		}
		VECTOR spawnPos = VGet(randX, 0.0f, randZ);

		// 暫定対応: 現在はkAnimal1(牛)の実装のみ。他種別追加時はFactory等へのリファクタリングを推奨
		if (tag == AnimalMove::kAnimal1)
		{
			SpawnAndInit<Animal>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}
