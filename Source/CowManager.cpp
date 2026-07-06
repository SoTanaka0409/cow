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
 * @brief 指定された種類の牛を生成またはプールから再利用して配置する
 * [入力] filename: モデルファイル, pos: 出現基準座標, scale: 拡大率, tag: 牛のタグ, count: 生成数, mfever: フィーバーフラグ
 * [出力] なし
 * [副作用] 牛のメモリ確保およびmCowsへの追加、またはプールからの取り出し
 */
void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever)
{
	for (int i = 0; i < count; i++)
	{
		// パフォーマンス維持のため、同時出現数を最大30匹に制限する
		if (mCreatures.size() >= 30)
		{
			if (tag == CowMove::Cow_gold)
			{
				// 金の牛を確実に出現させるため、プレイヤーから最も遠い普通の牛を優先して破棄し枠を空ける
				bool erased = false;
				float maxDistSq = -1.0f;
				auto furthestIt = mCreatures.end();
				
				VECTOR playerPos = Master::mpCamera->GetPosition();

				for (auto it = mCreatures.begin(); it != mCreatures.end(); ++it)
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

				if (furthestIt != mCreatures.end())
				{
					// 既に削除フラグが立っている牛などはObjectManager側で消される
					(*furthestIt)->Die(DEATH_LIMIT);
					auto cow = *furthestIt;
					cow->Deactivate();
					mPools[cow->GetTag_cow()].push_back(cow);
					mCreatures.erase(furthestIt);
					erased = true;
				}
				else if (!mCreatures.empty())
				{
					// 全ての牛が画面内などの場合、一番古いものの削除フラグを立ててリストから除外する
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

		if (tag == CowMove::Cow_1)
		{
			SpawnAndInit<Cow>(tag, spawnPos, scale, filename, spawnPos, 1.0f);
		}
		else if (tag == CowMove::Cow_2)
		{
			SpawnAndInit<Cow_2>(tag, spawnPos, scale, filename, spawnPos);
		}
		else if (tag == CowMove::Cow_gold)
		{
			auto feverMode = mfever ? Cow_gold::fever : Cow_gold::Nofever;
			auto cow = SpawnAndInit<Cow_gold>(tag, spawnPos, scale, filename, spawnPos, feverMode);
			cow->SetFever(feverMode);
		}
		else if (tag == CowMove::Cow_T)
		{
			SpawnAndInit<Cow_Tutorial>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}


