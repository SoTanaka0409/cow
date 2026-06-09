#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

AnimalManager::AnimalManager()
	: mnTagCount(0)
{
}

AnimalManager::~AnimalManager()
{
	mAnimals.clear();
}

void AnimalManager::SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count)
{
	for (int i = 0; i < count; i++)
	{
		float randX = (float)(GetRand(pos.x) - pos.x / 2);
		float randZ = (float)(GetRand(pos.z) - pos.z / 2);
		VECTOR spawnPos = VGet(randX, 0.0f, randZ);
		if (tag == AnimalMove::Animal_1)
		{
			auto newAnimal = new Animal(filename, spawnPos);
			newAnimal->SetScale(scale);
			mAnimals.push_back(newAnimal);
		}
	}
}

void AnimalManager::Update()
{
	judgmentAnimal();
	EraseAnimal();
}

void AnimalManager::Draw()
{
}

void AnimalManager::judgmentAnimal()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	if (player == nullptr) return;

	for (auto animal : mAnimals)
	{
		if (animal->IsDead() && !animal->IsDeleteFlag())
		{
			if (animal->GetTag_animal() == AnimalMove::Tag_animal::Animal_T)
			{
				Master::mnTutorialcount++;
			}

			mnTagCount++;
			if (mnTagCount == 1)
			{
				s.tag1 = animal->GetTag_animal();
			}
			else if (mnTagCount == 2 && s.tag1 == animal->GetTag_animal())
			{
				s.tag2 = animal->GetTag_animal();
			}
			else if (mnTagCount == 3 && s.tag2 == animal->GetTag_animal())
			{
				s.tag3 = animal->GetTag_animal();
				if (s.tag3 == AnimalMove::Animal_1)
				{
					player->mpLevel->AddXp(10);
				}
				if (s.tag2 == AnimalMove::Animal_2)
				{
					player->mpLevel->AddXp(20);
				}
				if (s.tag3 == AnimalMove::Animal_3)
				{
					player->mpLevel->AddXp(30);
				}
			}
			else
			{
				mnTagCount = 0;
				s.tag1 = AnimalMove::none;
				s.tag2 = AnimalMove::none;
				s.tag3 = AnimalMove::none;
			}
		}
	}
}

void AnimalManager::EraseAnimal()
{
	if (!mAnimals.empty())
	{
		mAnimals.erase(
			std::remove_if(
				mAnimals.begin(),
				mAnimals.end(),
				[](AnimalMove* a) { return a->IsDeleteFlag(); }
			),
			mAnimals.end()
		);
	}
}
