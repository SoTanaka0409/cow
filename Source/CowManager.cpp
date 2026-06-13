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
	: mnTagCount(0)
{
}

CowManager::~CowManager()
{
	mCows.clear();
}

void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever)
{
	for (int i = 0; i < count; i++)
	{
		
		// 画面上の同時アクティブ牛数上限を30匹に制限する
		if (mCows.size() >= 30)
		{
			if (tag == CowMove::Cow_gold)
			{
				// 金の牛が出現するスペースを作るため、普通の牛の中でプレイヤーから最も遠いものを安全に破棄する
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
					// 既に削除フラグが立っている牛などはObjectManager側で消されるが
					(*furthestIt)->Die(CowMove::DEATH_LIMIT);
					mCows.erase(furthestIt);
					erased = true;
				}
				else if (!mCows.empty())
				{
					// 全ての牛が画面内の場合は、一番古い牛の削除フラグを立ててリストから除外
					mCows.front()->Die(CowMove::DEATH_LIMIT);
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
			auto newCow = new Cow(filename, spawnPos, 1.0f);
			newCow->SetScale(scale);
			mCows.push_back(newCow);
		}
		else if (tag == CowMove::Cow_2)
		{
			auto newCow = new Cow_2(filename, spawnPos);
			newCow->SetScale(scale);
			mCows.push_back(newCow);
		}
		else if (tag == CowMove::Cow_gold)
		{
			auto feverMode = mfever ? Cow_gold::fever : Cow_gold::Nofever;
			auto newCow = new Cow_gold(filename, spawnPos, feverMode);
			newCow->SetScale(scale);
			mCows.push_back(newCow);
		}
	}
}

void CowManager::Update()
{
	judgmentCow();
	EraseCow();
}

void CowManager::Draw()
{
}

void CowManager::judgmentCow()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	if (player == nullptr) return;

	for (auto cow : mCows)
	{
		if (cow->IsDead() && !cow->IsJudged())
		{
			cow->SetJudged(true);
			if (cow->GetTag_cow() == CowMove::Tag_cow::Cow_T)
			{
				Master::mnTutorialcount++;
			}

			mnTagCount++;
			if (mnTagCount == 1)
			{
				s.tag1 = cow->GetTag_cow();
			}
			else if (mnTagCount == 2 && s.tag1 == cow->GetTag_cow())
			{
				s.tag2 = cow->GetTag_cow();
			}
			else if (mnTagCount == 3 && s.tag2 == cow->GetTag_cow())
			{
				s.tag3 = cow->GetTag_cow();
				if (s.tag3 == CowMove::Cow_1)
				{
					player->mpLevel->AddXp(10);
				}
				if (s.tag2 == CowMove::Cow_2)
				{
					player->mpLevel->AddXp(20);
				}
				if (s.tag3 == CowMove::Cow_3)
				{
					player->mpLevel->AddXp(30);
				}
			}
			else
			{
				mnTagCount = 0;
				s.tag1 = CowMove::none;
				s.tag2 = CowMove::none;
				s.tag3 = CowMove::none;
			}
		}
	}
}

void CowManager::EraseCow()
{
	if (!mCows.empty())
	{
		for (auto it = mCows.begin(); it != mCows.end();)
		{
			if ((*it)->IsDeleteFlag())
			{
				it = mCows.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}
