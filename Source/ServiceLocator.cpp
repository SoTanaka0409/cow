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
	// 未初期化時のクラチE??ュを防ぐため?Eインタの有効性を検証
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
	// タグ検索によりプレイヤーオブジェクトを動的に特?E
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		return dynamic_cast<Player3D*>(objMgr->GetObject3DByTag(Object3D::kTag3dPlayer));
	}
	return nullptr;
}

std::vector<Player3D*> ServiceLocator::GetPlayers()
{
	// マルチ?Eレイ対応等を老E?Eし、存在する全プレイヤーを収雁E
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
		return scene->cow_manager_;
	}
	return nullptr;
}

AnimalManager* ServiceLocator::GetAnimalManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->animal_manager_;
	}
	return nullptr;
}

GameManager* ServiceLocator::GetGameManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->game_manager_;
	}
	return nullptr;
}

Fever* ServiceLocator::GetFever()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->fever_;
	}
	return nullptr;
}