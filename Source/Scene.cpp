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
		mpObjectManager->DeleteAll2D(); // 2Dテクスチャオブジェクト群を破棄し、シーン切り替え時のメモリリークを防ぐ
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
		mpColliderManager->Draw();
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

/*
 * @brief シーンフェード演出時の黒スクリーンを描画する
 * [入力] fade: 進行するフェード状態
 * [出力] なし
 * [副作用] アルファブレンドモードの変更、黒い四角形の描画
 */
void Scene::Fade(SceneFade fade)
{
	if (fade == SceneFade::SceneFade_In)
	{
		mfFadeAlpha -= mfFadeSpeed;
		if (mfFadeAlpha < 0) mfFadeAlpha = 0;

		if (mfFadeAlpha > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else if (fade == SceneFade::SceneFade_Out)
	{
		mfFadeAlpha += mfFadeSpeed;
		if (mfFadeAlpha > 255) mfFadeAlpha = 255;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else if (fade == SceneFade::SceneFade_Load)
	{
	}
}
