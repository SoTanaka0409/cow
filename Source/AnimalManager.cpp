#include "ServiceLocator.h"
#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
AnimalManager::AnimalManager()
{
}

/*
 * 入力: filename (モデル), pos (基準座標), scale (倍率), tag (動物種別), count (生成数)
 * 出力: なし
 * 副作用: オブジェクトのメモリ確保(またはプールからの再利用)と管理リストへの追加
 */
void AnimalManager::SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::TagAnimal tag, int count)
{
	for (int i = 0; i < count; i++)
	{
		// 複数生成時に同じ座標に重なって描画（Zファイティング等）されるのを防ぐため座標を散らす
		float randX = (float)(GetRand(pos.x) - pos.x / 2);
		float randZ = (float)(GetRand(pos.z) - pos.z / 2);
		VECTOR spawnPos = VGet(randX, 0.0f, randZ);

		// 暫定対応: 現在はkAnimal1(牛)の実装のみ。他種別追加時はFactory等へのリファクタリングを推奨
		if (tag == AnimalMove::kAnimal1)
		{
			SpawnAndInit<Animal>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}