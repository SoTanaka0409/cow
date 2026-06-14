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
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		// Player3Dとして安全にキャストして返す
		return dynamic_cast<Player3D*>(objMgr->GetObject3DByTag(Object3D::Tag3D_player));
	}
	return nullptr;
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
