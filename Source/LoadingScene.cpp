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

	// 1繝輔Ξ繝ｼ繝逶ｮ縺ｯ謠冗判繧貞━蜈医＠縲・繝輔Ξ繝ｼ繝逶ｮ莉･髯阪〒髱槫酔譛溘Ο繝ｼ繝峨ｒ髢句ｧ九☆繧・
	// ・磯幕蟋狗峩蠕後↓ GetASyncLoadNum() 縺瑚ｪ､縺｣縺ｦ0繧定ｿ斐☆縺薙→繧帝亟縺撰ｼ・
	if (!mbLoadStarted && mLoadingTimer >= 2)
	{
		SetUseASyncLoadFlag(TRUE);

		Master::mpResourceManager->PreloadModel(GameConstants::kCowDefault.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kCowGold.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kAnimalChicken.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kAnimalBear.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kAnimalSheep.model_path);
		Master::mpResourceManager->PreloadModel("Resource/3D/ufo2/uploads_files_2595751_UFO.mv1");
		Master::mpResourceManager->PreloadModel("Resource/3D/NewFence/fence1.mv1");
		Master::mpResourceManager->PreloadModel("Resource/3D/SkyBox/SkyBox.mv1");

		SetUseASyncLoadFlag(FALSE);

		mbLoadStarted = true;
	}

	// 繝ｭ繝ｼ繝峨′髢句ｧ九＆繧後※縺翫ｊ縲√°縺､蜈ｨ繝輔ぃ繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺悟ｮ御ｺ・＠縺ｦ縺・◆繧蛾・遘ｻ縺吶ｋ
	if (mbLoadStarted && GetASyncLoadNum() == 0 && mLoadingTimer > 60)
	{
		// SceneManager縺ｫ逶ｴ謗･驕ｷ遘ｻ繧呈欠遉ｺ縺吶ｋ・・NextScene縺ｸ縺ｮ莉｣蜈･縺縺代〒縺ｯ驕ｷ遘ｻ縺励↑縺・ｼ・
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
	}
}

void LoadingScene::Draw()
{
	// 鮟定レ譎ｯ繧呈緒逕ｻ
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);

	// NOW LOADING... 縺ｮ轤ｹ貊・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if ((mLoadingTimer / 20) % 2 == 0)
	{
		DrawString(1600 / 2 - 60, 900 / 2, "NOW LOADING...", GetColor(255, 255, 255));
	}
}

void LoadingScene::Finalize()
{
}
