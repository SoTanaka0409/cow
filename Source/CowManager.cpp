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

CowManager::CowManager()
{
}

CowManager::~CowManager()
{
	mCows.clear();
	for (auto& pair : mPools)
	{
		for (auto cow : pair.second)
		{
			delete cow;
		}
	}
	mPools.clear();
}

void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever)
{
	for (int i = 0; i < count; i++)
	{
		
		// 画面丁EE同時アクチE??ブ牛数上限めE0匹に制限すめE
		if (mCows.size() >= 30)
		{
			if (tag == CowMove::Cow_gold)
			{
				// 釁EE牛が出現するスチEEスを作るため、普送EE牁EE中でプレイヤーから最も遠ぁE??のを宁EEに破?E??めE
				bool erased = false;
				float maxDistSq = -1.0f;
				auto furthestIt = mCows.end();
				
				VECTOR playerPos = Master::mpCamera->GetPosition();

				for (auto it = mCows.begin(); it != mCows.end(); ++it)
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

				if (furthestIt != mCows.end())
				{
					// 既に削除フラグが立ってぁE��牛などはObjectManager側で消されるぁE
					(*furthestIt)->Die(DEATH_LIMIT);
					auto cow = *furthestIt;
					cow->Deactivate();
					mPools[cow->GetTag_cow()].push_back(cow);
					mCows.erase(furthestIt);
					erased = true;
				}
				else if (!mCows.empty())
				{
					// 全ての牛が画面冁E��どの場合�E、一番古ぁE��のの削除フラグを立ててリストから除夁E
					mCows.front()->Die(DEATH_LIMIT);
					auto cow = mCows.front();
					cow->Deactivate();
					mPools[cow->GetTag_cow()].push_back(cow);
					mCows.erase(mCows.begin());
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
			if (!mPools[tag].empty())
			{
				auto cow = mPools[tag].back();
				mPools[tag].pop_back();
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				mCows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow(filename, spawnPos, 1.0f);
				newCow->SetScale(scale);
				mCows.push_back(newCow);
			}
		}
		else if (tag == CowMove::Cow_2)
		{
			if (!mPools[tag].empty())
			{
				auto cow = mPools[tag].back();
				mPools[tag].pop_back();
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				mCows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow_2(filename, spawnPos);
				newCow->SetScale(scale);
				mCows.push_back(newCow);
			}
		}
		else if (tag == CowMove::Cow_gold)
		{
			auto feverMode = mfever ? Cow_gold::fever : Cow_gold::Nofever;
			if (!mPools[tag].empty())
			{
				auto cow = dynamic_cast<Cow_gold*>(mPools[tag].back());
				mPools[tag].pop_back();
				if (cow) cow->SetFever(feverMode);
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				mCows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow_gold(filename, spawnPos, feverMode);
				newCow->SetScale(scale);
				mCows.push_back(newCow);
			}
		}
		else if (tag == CowMove::Cow_T)
		{
			if (!mPools[tag].empty())
			{
				auto cow = mPools[tag].back();
				mPools[tag].pop_back();
				cow->Reset(spawnPos);
				cow->SetScale(scale);
				mCows.push_back(cow);
			}
			else
			{
				auto newCow = new Cow_Tutorial(filename, spawnPos);
				newCow->SetScale(scale);
				mCows.push_back(newCow);
			}
		}
	}
}

void CowManager::Update()
{
	for (auto cow : mCows)
	{
		cow->Update();
	}
	EraseCow();
}

void CowManager::Draw()
{
}

void CowManager::EraseCow()
{
	if (!mCows.empty())
	{
		for (auto it = mCows.begin(); it != mCows.end();)
		{
			if ((*it)->GetCowDelete())
			{
				auto cow = *it;
				cow->Deactivate();
				mPools[cow->GetTag_cow()].push_back(cow);
				it = mCows.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

