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
 * @brief 管理オブジェクトの初期化を行う
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
CowManager::CowManager()
{
}

/*
 * @brief 管理リストおよびオブジェクトプールの牛を全解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 牛オブジェクトのメモリ解放
 */
CowManager::~CowManager()
{
	cows.clear();
	for (auto& pair : pools)
	{
		for (auto cow : pair.second)
		{
			delete cow;
		}
	}
	pools.clear();
}

/*
 * @brief 指定された種類の牛を生成またはプールから再利用して配置する
 * [入力] filename: モデルファイル, pos: 出現基準座標, scale: 拡大率, tag: 牛のタグ, count: 生成数, fever: フィーバーフラグ
 * [出力] なし
 * [副作用] 牛のメモリ確保およびmCowsへの追加、またはプールからの取り出し
 */
void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool fever)
{
	for (int i = 0; i < count; i++)
	{
		// パフォーマンス維持のため、同時出現数を最大30匹に制限する
		if (cows.size() >= 30)
		{
			if (tag == CowMove::Cow_gold)
			{
				// 金の牛を確実に出現させるため、プレイヤーから最も遠い普通の牛を優先して破棄し枠を空ける
				bool erased = false;
				float maxDistSq = -1.0f;
				auto furthestIt = cows.end();
				
				VECTOR playerPos = Master::camera->GetPosition();

				for (auto it = cows.begin(); it != cows.end(); ++it)
				{
					if ((*it)->GetTag_cow() != CowMove::Cow_gold)
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

				if (furthestIt != cows.end())
				{
					// 既に削除フラグが立っている牛などはObjectManager側で消される
					(*furthestIt)->Die(DEATH_LIMIT);
					auto cow = *furthestIt;
					cow->Deactivate();
					pools[cow->GetTag_cow()].push_back(cow);
					cows.erase(furthestIt);
					erased = true;
				}
				else if (!cows.empty())
				{
					// 全ての牛が画面内などの場合、一番古いものの削除フラグを立ててリストから除外する
					cows.front()->Die(DEATH_LIMIT);
					auto cow = cows.front();
					cow->Deactivate();
					pools[cow->GetTag_cow()].push_back(cow);
					cows.erase(cows.begin());
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

		if (tag == CowMove::Cow_1)
		{
			if (!pools[tag].empty())
			{
				auto cow = pools[tag].back();
				pools[tag].pop_back();
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				cows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow(filename, spawnPos, 1.0f);
				newCow->SetScale(scale);
				cows.push_back(newCow);
			}
		}
		else if (tag == CowMove::Cow_2)
		{
			if (!pools[tag].empty())
			{
				auto cow = pools[tag].back();
				pools[tag].pop_back();
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				cows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow_2(filename, spawnPos);
				newCow->SetScale(scale);
				cows.push_back(newCow);
			}
		}
		else if (tag == CowMove::Cow_gold)
		{
			auto feverMode = fever ? Cow_gold::Fever : Cow_gold::NoFever;
			if (!pools[tag].empty())
			{
				auto cow = dynamic_cast<Cow_gold*>(pools[tag].back());
				pools[tag].pop_back();
				if (cow) cow->SetFever(feverMode);
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				cows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow_gold(filename, spawnPos, feverMode);
				newCow->SetScale(scale);
				cows.push_back(newCow);
			}
		}
		else if (tag == CowMove::Cow_T)
		{
			if (!pools[tag].empty())
			{
				auto cow = pools[tag].back();
				pools[tag].pop_back();
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				cows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow_Tutorial(filename, spawnPos);
				newCow->SetScale(scale);
				cows.push_back(newCow);
			}
		}
	}
}

/*
 * @brief 全ての牛の更新処理と不要な牛の削除（プール返却）を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 各牛のUpdate実行とEraseCowの実行
 */
void CowManager::Update()
{
	for (auto cow : cows)
	{
		cow->Update();
	}
	EraseCow();
}

/*
 * @brief 全ての牛の描画を行う（現在は描画処理を外部で行っているため空）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void CowManager::Draw()
{
}

/*
 * @brief 削除フラグが立っている牛を非アクティブ化しプールに返却する
 * [入力] なし
 * [出力] なし
 * [副作用] mCowsからの削除およびmPoolsへの追加
 */
void CowManager::EraseCow()
{
	if (!cows.empty())
	{
		for (auto it = cows.begin(); it != cows.end();)
		{
			if ((*it)->GetCowDelete())
			{
				auto cow = *it;
				cow->Deactivate();
				pools[cow->GetTag_cow()].push_back(cow);
				it = cows.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}
