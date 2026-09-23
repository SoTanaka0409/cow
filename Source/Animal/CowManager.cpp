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
/// @brief 牛管理オブジェクトの初期化を行う。
CowManager::CowManager()
{
}
/// @brief 指定された種類の牛を生成・配置する。上限到達時は既存の牛を間引く。
/// @param filename モデルファイルパス
/// @param pos 基準座標
/// @param scale スケール
/// @param tag 牛の種類
/// @param count 生成数
/// @param is_fever フィーバー状態フラグ
/// @details mCreaturesへのオブジェクト追加、および上限時は既存オブジェクトの破棄を行う。
void CowManager::SpawnCow(const std::string& filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever, float scatterRadius)
{
	for (int i = 0; i < count; i++)
	{
		// パフォーマンス低下を防ぐため、フィールド上の牛の最大数を30匹に制限する。
		if (mCreatures.size() >= 30)
		{
			if (tag == CowMove::kCowGold)
			{
				// 金牛は出現優先度が高いため、遠方にいる不要な一般牛を破棄して生成枠を確保する。
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
					(*furthestIt)->Die(kDeathLimit);
					auto cow = *furthestIt;
					cow->Deactivate();
					pools_[cow->GetTagCow()].push_back(cow);
					mCreatures.erase(furthestIt);
					erased = true;
				}
				else if (!mCreatures.empty())
				{
					// 一般牛がいない場合でも、最も古い牛を破棄して金牛の枠を強制的に空ける。
					mCreatures.front()->Die(kDeathLimit);
					auto cow = mCreatures.front();
					cow->Deactivate();
					pools_[cow->GetTagCow()].push_back(cow);
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

