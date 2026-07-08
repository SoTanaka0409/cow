#include "LoadingScene.h"
#include "DxLib.h"
#include "Master.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameConstants.h"

LoadingScene::LoadingScene()
	: mLoadingTimer(0)
	, mbLoadStarted(false)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Initialize()
{
	mLoadingTimer = 0;
	mbLoadStarted = false;
}

void LoadingScene::Update()
{
	mLoadingTimer++;

	// 1フレーム目は描画を優先し、2フレーム目以降で非同期ロードを開始する
	// （開始直後に GetASyncLoadNum() が誤って0を返すことを防ぐ）
	if (!mbLoadStarted && mLoadingTimer >= 2)
	{
		SetUseASyncLoadFlag(TRUE);

		Master::mpResourceManager->PreloadModel(GameConstants::COW_DEFAULT.modelPath);
		Master::mpResourceManager->PreloadModel(GameConstants::COW_GOLD.modelPath);
		Master::mpResourceManager->PreloadModel(GameConstants::ANIMAL_CHICKEN.modelPath);
		Master::mpResourceManager->PreloadModel(GameConstants::ANIMAL_BEAR.modelPath);
		Master::mpResourceManager->PreloadModel(GameConstants::ANIMAL_SHEEP.modelPath);
		Master::mpResourceManager->PreloadModel("Resource/3D/ufo2/uploads_files_2595751_UFO.mv1");
		Master::mpResourceManager->PreloadModel("Resource/3D/NewFence/fence1.mv1");
		Master::mpResourceManager->PreloadModel("Resource/3D/SkyBox/SkyBox.mv1");

		SetUseASyncLoadFlag(FALSE);

		mbLoadStarted = true;
	}

	// ロードが開始されており、かつ全ファイルの読み込みが完了していたら遷移する
	if (mbLoadStarted && GetASyncLoadNum() == 0 && mLoadingTimer > 60)
	{
		// SceneManagerに直接遷移を指示する（mNextSceneへの代入だけでは遷移しない）
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
	}
}

void LoadingScene::Draw()
{
	// 黒背景を描画
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);

	// NOW LOADING... の点滅アニメーション
	if ((mLoadingTimer / 20) % 2 == 0)
	{
		DrawString(1600 / 2 - 60, 900 / 2, "NOW LOADING...", GetColor(255, 255, 255));
	}
}

void LoadingScene::Finalize()
{
}
