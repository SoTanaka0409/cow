
#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

Scene::Scene()
{

	objectManager = new ObjectManager();
	colliderManager = new ColliderManager();
	gameManager = new GameManager();
	cowManager = new CowManager();
	animalManager = new AnimalManager();
	fever = new Fever();


	fadeAlpha = 0.0f;
	fadeSpeed = 5.0f;
}

Scene::~Scene()
{

	if (objectManager != nullptr)
	{
		objectManager->DeleteAll3D();
		objectManager->DeleteAll2D(); // シーン切り替え時のメモリリーク防止
		delete objectManager;
	}


	if (colliderManager != nullptr)
	{
		colliderManager->DeleteAllCollider();
		delete colliderManager;
	}


	if (gameManager != nullptr)
	{
		delete gameManager;
	}
	if (cowManager != nullptr)
	{
		delete cowManager;
	}
	if (fever != nullptr)
	{
		delete fever;
	}
	if (animalManager != nullptr)
	{
		delete animalManager;
	}
}

void Scene::Draw()
{
	if (objectManager != nullptr)
	{
		objectManager->Draw();
	}
	if (colliderManager != nullptr)
	{
		colliderManager->Draw(); // デバッグ用の当たり判定可視化
	}
	if (animalManager != nullptr)
	{
		animalManager->Draw();
	}
}

void Scene::Update()
{
	if (objectManager != nullptr)
	{
		objectManager->Update();
	}
	if (colliderManager != nullptr)
	{
		colliderManager->Update();
	}
	if (fever != nullptr)
	{
		fever->Update();
	}
	if (animalManager != nullptr)
	{
		animalManager->Update();
	}
}

void Scene::Fade(SceneFade fade)
{
	// フェードイン
	if (fade == SceneFade::SceneFade_In)
	{

		fadeAlpha -= fadeSpeed;
		if (fadeAlpha < 0) fadeAlpha = 0; 


		if (fadeAlpha > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fadeAlpha);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); // 画面全体を暗転
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
		}
	}
	// フェードアウト
	else if (fade == SceneFade::SceneFade_Out)
	{

		fadeAlpha += fadeSpeed;
		if (fadeAlpha > 255) fadeAlpha = 255; // DxLibの仕様によりアルファ値上限は255


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fadeAlpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
	}
	// ロード中
	else if (fade == SceneFade::SceneFade_Load)
	{
		// 将来のロード画面拡張用スペース
	}
}
