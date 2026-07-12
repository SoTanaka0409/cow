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
#include "GoldCow.h"
#include "CapsuleCollider.h"

/*
 * @brief 管?E??ブジェクト?E初期化を行う
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
CowManager::CowManager()
{
}



/*
 * @brief 持E??された種類?E牛を生?Eまた?Eプ?Eルから再利用して配置する
 * [入力] filename: モチE??ファイル, pos: 出現基準座?E scale: 拡大?E tag: 牛?Eタグ, count: 生?E数, is_fever: フィーバ?Eフラグ
 * [出力] なし
 * [副作用] 牛?Eメモリ確保およ?EmCowsへの追加、また?Eプ?Eルからの取り出ぁE
 */
void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever)
{
	for (int i = 0; i < count; i++)
	{
		// パフォーマンス維持?Eため、同時?E現数を最大30匹に制限すめE
		if (mCreatures.size() >= 30)
		{
			if (tag == CowMove::kCowGold)
			{
				// 金?E牛を確実に出現させるため、?Eレイヤーから最も遠ぁE??通?E牛を優先して破?E??枠を空ける
				bool erased = false;
				float maxDistSq = -1.0f;
				auto furthestIt = mCreatures.end();
				
				VECTOR playerPos = Master::camera_->GetPosition();

				for (auto it = mCreatures.begin(); it != mCreatures.end(); ++it)
				{
					if ((*it)->GetTagCow() != CowMove::kCowGold)
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
					// 既に削除フラグが立ってぁE??牛などはObjectManager側で消される
					(*furthestIt)->Die(DEATH_LIMIT);
					auto cow = *furthestIt;
					cow->Deactivate();
					mPools[cow->GetTagCow()].push_back(cow);
					mCreatures.erase(furthestIt);
					erased = true;
				}
				else if (!mCreatures.empty())
				{
					// 全ての牛が画面?E??どの場合、一番古ぁE??のの削除フラグを立ててリストから除外すめE
					mCreatures.front()->Die(DEATH_LIMIT);
					auto cow = mCreatures.front();
					cow->Deactivate();
					mPools[cow->GetTagCow()].push_back(cow);
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
			auto feverMode = is_fever ? GoldCow::kFever : GoldCow::kNoFever;
			auto cow = SpawnAndInit<GoldCow>(tag, spawnPos, scale, filename, spawnPos, feverMode);
			cow->SetFever(feverMode);
		}
		else if (tag == CowMove::kCowT)
		{
			SpawnAndInit<Cow_Tutorial>(tag, spawnPos, scale, filename, spawnPos);
		}
	}
}

