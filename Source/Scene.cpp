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

	mpObjectManager = new ObjectManager();
	mpColliderManager = new ColliderManager();
	mpGameManager = new GameManager();
	mpCowManager = new CowManager();
	mpAnimalManager = new AnimalManager();
	mpFever = new Fever();


	mfFadeAlpha = 0.0f;
	mfFadeSpeed = 5.0f;
}

Scene::~Scene()
{

	if (mpObjectManager != nullptr)
	{
		mpObjectManager->DeleteAll3D();
		mpObjectManager->DeleteAll2D(); // シーン切り替え時のメモリリーク防止
		delete mpObjectManager;
	}


	if (mpColliderManager != nullptr)
	{
		mpColliderManager->DeleteAllCollider();
		delete mpColliderManager;
	}


	if (mpGameManager != nullptr)
	{
		delete mpGameManager;
	}
	if (mpCowManager != nullptr)
	{
		delete mpCowManager;
	}
	if (mpFever != nullptr)
	{
		delete mpFever;
	}
	if (mpAnimalManager != nullptr)
	{
		delete mpAnimalManager;
	}
}

void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}
	if (mpColliderManager != nullptr)
	{
		mpColliderManager->Draw(); // デバッグ用の当たり判定可視化
	}
	if (mpAnimalManager != nullptr)
	{
		mpAnimalManager->Draw();
	}
}

void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Update();
	}
	if (mpColliderManager != nullptr)
	{
		mpColliderManager->Update();
	}
	if (mpFever != nullptr)
	{
		mpFever->Update();
	}
	if (mpAnimalManager != nullptr)
	{
		mpAnimalManager->Update();
	}
}

void Scene::Fade(SceneFade fade)
{
	// フェードイン
	if (fade == SceneFade::SceneFade_In)
	{

		mfFadeAlpha -= mfFadeSpeed;
		if (mfFadeAlpha < 0) mfFadeAlpha = 0; 


		if (mfFadeAlpha > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); // 画面全体を暗転
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
		}
	}
	// フェードアウト
	else if (fade == SceneFade::SceneFade_Out)
	{

		mfFadeAlpha += mfFadeSpeed;
		if (mfFadeAlpha > 255) mfFadeAlpha = 255; // DxLibの仕様によりアルファ値上限は255


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
	}
	// ロード中
	else if (fade == SceneFade::SceneFade_Load)
	{
		// 将来のロード画面拡張用スペース
	}
}
