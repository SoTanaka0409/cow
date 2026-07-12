#include "LoadingScene.h"
#include "DxLib.h"
#include "Master.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameConstants.h"

LoadingScene::LoadingScene()
	: loading_timer_(0)
	, load_started_(false)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Initialize()
{
	loading_timer_ = 0;
	load_started_ = false;
}

void LoadingScene::Update()
{
	loading_timer_++;

	// 1繝輔Ξ繝ｼ繝逶�E�縺�E�謠冗判繧貞━蜈医�E�縲・繝輔Ξ繝ｼ繝逶�E�莉･髯阪〒髱槫酔譛溘Ο繝ｼ繝峨�E�髢句�E�九�E繧・
	// ・磯幕蟋狗峩蠕後�E GetASyncLoadNum() 縺瑚ｪ�E�縺�E�縺�E�0繧定ｿ斐�E縺薙�E繧帝亟縺撰�E�・
	if (!load_started_ && loading_timer_ >= 2)
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

		load_started_ = true;
	}

	// 繝ｭ繝ｼ繝峨′髢句�E�九！E��後※縺翫�E�縲√°縺�E�蜈ｨ繝輔ぃ繧�E�繝ｫ縺�E�隱�E�縺�E�霎ｼ縺�E�縺悟ｮ御�E�・�E�縺�E�縺・◁E��蛾・遘ｻ縺吶�E�E
	if (load_started_ && GetASyncLoadNum() == 0 && loading_timer_ > 60)
	{
		// SceneManager縺�E�逶�E�謗･驕ｷ遘ｻ繧呈欠遉ｺ縺吶�E��E・NextScene縺�E�縺�E�莉｣蜈･縺�E�縺代〒縺�E�驕ｷ遘ｻ縺励↑縺・�E�・
		Master::mpSceneManager->SetNextScene(SceneManager::kScene3D);
	}
}

void LoadingScene::Draw()
{
	// 鮟定レ譎ｯ繧呈緒逕ｻ
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);

	// NOW LOADING... 縺�E�轤�E�貊�EぁE��九Γ繝ｼ繧�E�繝ｧ繝ｳ
	if ((loading_timer_ / 20) % 2 == 0)
	{
		DrawString(1600 / 2 - 60, 900 / 2, "NOW LOADING...", GetColor(255, 255, 255));
	}
}

void LoadingScene::Finalize()
{
}
