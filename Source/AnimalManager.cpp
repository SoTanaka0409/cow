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
 * @brief 管理リストおよびオブジェクトプールの動物を全解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 動物オブジェクトのメモリ解放
 */
AnimalManager::~AnimalManager()
{
	mAnimals.clear();
	for (auto& pair : mPools)
	{
		for (auto animal : pair.second)
		{
			delete animal;
		}
	}
	mPools.clear();
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
			if (!mPools[tag].empty())
			{
				auto animal = mPools[tag].back();
				mPools[tag].pop_back();
				animal->Reset(spawnPos);
				animal->SetScale(scale);
				mAnimals.push_back(animal);
			}
			else
			{
				auto newAnimal = new Animal(filename, spawnPos);
				newAnimal->SetScale(scale);
				mAnimals.push_back(newAnimal);
			}
		}
	}
}

/*
 * @brief 全ての動物の更新処理と不要な動物の削除（プール返却）を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 各動物のUpdate実行とEraseAnimalの実行
 */
void AnimalManager::Update()
{
	for (auto animal : mAnimals)
	{
		animal->Update();
	}
	EraseAnimal();
}

/*
 * @brief 全ての動物の描画を行う（現在は描画処理を外部で行っているため空）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void AnimalManager::Draw()
{
}

/*
 * @brief 削除フラグが立っている動物を非アクティブ化しプールに返却する
 * [入力] なし
 * [出力] なし
 * [副作用] mAnimalsからの削除およびmPoolsへの追加
 */
void AnimalManager::EraseAnimal()
{
	if (!mAnimals.empty())
	{
		for (auto it = mAnimals.begin(); it != mAnimals.end();)
		{
			if ((*it)->GetCharacterDelete())
			{
				auto animal = *it;
				animal->Deactivate();
				mPools[animal->GetTag_animal()].push_back(animal);
				it = mAnimals.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}
