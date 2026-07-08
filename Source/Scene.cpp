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
		mpObjectManager->DeleteAll2D(); // 繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺域凾縺ｮ繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ髦ｲ豁｢
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
		mpColliderManager->Draw(); // 繝・ヰ繝・げ逕ｨ縺ｮ蠖薙◆繧雁愛螳壼庄隕門喧
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
	// 繝輔ぉ繝ｼ繝峨う繝ｳ
	if (fade == SceneFade::SceneFade_In)
	{

		mfFadeAlpha -= mfFadeSpeed;
		if (mfFadeAlpha < 0) mfFadeAlpha = 0; 


		if (mfFadeAlpha > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); // 逕ｻ髱｢蜈ｨ菴薙ｒ證苓ｻ｢
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
		}
	}
	// 繝輔ぉ繝ｼ繝峨い繧ｦ繝・
	else if (fade == SceneFade::SceneFade_Out)
	{

		mfFadeAlpha += mfFadeSpeed;
		if (mfFadeAlpha > 255) mfFadeAlpha = 255; // DxLib縺ｮ莉墓ｧ倥↓繧医ｊ繧｢繝ｫ繝輔ぃ蛟､荳企剞縺ｯ255


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
	}
	// 繝ｭ繝ｼ繝我ｸｭ
	else if (fade == SceneFade::SceneFade_Load)
	{
		// 蟆・擂縺ｮ繝ｭ繝ｼ繝臥判髱｢諡｡蠑ｵ逕ｨ繧ｹ繝壹・繧ｹ
	}
}
