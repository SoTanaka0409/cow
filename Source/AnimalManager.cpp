#include "ServiceLocator.h"
#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

AnimalManager::AnimalManager()
{
}

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

void AnimalManager::Update()
{
	for (auto animal : mAnimals)
	{
		animal->Update();
	}
	EraseAnimal();
}

void AnimalManager::Draw()
{
}

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
