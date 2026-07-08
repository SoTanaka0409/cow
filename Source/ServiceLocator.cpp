#include "ServiceLocator.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "CowManager.h"
#include "AnimalManager.h"
#include "GameManager.h"
#include "Fever.h"

Scene* ServiceLocator::GetCurrentScene()
{
	// 譛ｪ蛻晄悄蛹匁凾縺ｮ繧ｯ繝ｩ繝・す繝･繧帝亟縺舌◆繧√・繧､繝ｳ繧ｿ縺ｮ譛牙柑諤ｧ繧呈､懆ｨｼ
	if (Master::mpSceneManager != nullptr)
	{
		return Master::mpSceneManager->GetCurrentScene();
	}
	return nullptr;
}

ObjectManager* ServiceLocator::GetObjectManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->GetObjectManager();
	}
	return nullptr;
}

Player3D* ServiceLocator::GetPlayer()
{
	// 繧ｿ繧ｰ讀懃ｴ｢縺ｫ繧医ｊ繝励Ξ繧､繝､繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜍慕噪縺ｫ迚ｹ螳・
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		return dynamic_cast<Player3D*>(objMgr->GetObject3DByTag(Object3D::kTag3dPlayer));
	}
	return nullptr;
}

std::vector<Player3D*> ServiceLocator::GetPlayers()
{
	// 繝槭Ν繝√・繝ｬ繧､蟇ｾ蠢懃ｭ峨ｒ閠・・縺励∝ｭ伜惠縺吶ｋ蜈ｨ繝励Ξ繧､繝､繝ｼ繧貞庶髮・
	std::vector<Player3D*> players;
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		const auto& list = objMgr->GetObject3DListByTag(Object3D::kTag3dPlayer);
		for (auto obj : list)
		{
			if (Player3D* p = dynamic_cast<Player3D*>(obj))
			{
				players.push_back(p);
			}
		}
	}
	return players;
}

CowManager* ServiceLocator::GetCowManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->mpCowManager;
	}
	return nullptr;
}

AnimalManager* ServiceLocator::GetAnimalManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->mpAnimalManager;
	}
	return nullptr;
}

GameManager* ServiceLocator::GetGameManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->mpGameManager;
	}
	return nullptr;
}

Fever* ServiceLocator::GetFever()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->mpFever;
	}
	return nullptr;
}