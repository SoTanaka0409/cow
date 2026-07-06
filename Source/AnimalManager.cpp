#include "ServiceLocator.h"
#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

/*
 * @brief 管理オブジェクトの初期化を行う
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
AnimalManager::AnimalManager()
{
}



/*
 * @brief 指定された種類の動物を生成またはプールから再利用して配置する
 * [入力] filename: モデルファイル, pos: 出現基準座標, scale: 拡大率, tag: 動物のタグ, count: 生成数
 * [出力] なし
 * [副作用] 動物のメモリ確保およびmAnimalsへの追加、またはプールからの取り出し
 */
void AnimalManager::SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count)
{
	for (int i = 0; i < count; i++)
	{
		float randX = (float)(GetRand(pos.x) - pos.x / 2);
		float randZ = (float)(GetRand(pos.z) - pos.z / 2);
		VECTOR spawnPos = VGet(randX, 0.0f, randZ);
		if (tag == AnimalMove::Animal_1)
		{
			SpawnAndInit<Animal>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}


