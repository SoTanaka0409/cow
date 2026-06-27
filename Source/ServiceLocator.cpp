#include "ServiceLocator.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Player3D.h"

#include "AnimalManager.h"
#include "GameManager.h"
#include "Fever.h"

Scene* ServiceLocator::GetCurrentScene()
{
	// 未初期化時のクラッシュを防ぐためポインタの有効性を検証
	if (Master::sceneManager != nullptr)
	{
		return Master::sceneManager->GetCurrentScene();
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
	// タグ検索によりプレイヤーオブジェクトを動的に特定
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		return dynamic_cast<Player3D*>(objMgr->GetObject3DByTag(Object3D::Tag3D_player));
	}
	return nullptr;
}

std::vector<Player3D*> ServiceLocator::GetPlayers()
{
	// マルチプレイ対応等を考慮し、存在する全プレイヤーを収集
	std::vector<Player3D*> players;
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		const auto& list = objMgr->GetObject3DListByTag(Object3D::Tag3D_player);
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
		return scene->cowManager;
	}
	return nullptr;
}

AnimalManager* ServiceLocator::GetAnimalManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->animalManager;
	}
	return nullptr;
}

GameManager* ServiceLocator::GetGameManager()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->gameManager;
	}
	return nullptr;
}

Fever* ServiceLocator::GetFever()
{
	Scene* scene = GetCurrentScene();
	if (scene != nullptr)
	{
		return scene->fever;
	}
	return nullptr;
}
