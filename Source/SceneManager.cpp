#include"SceneManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"TitleScene.h"
#include"TutorialScene.h"
#include"ResultScene.h"
#include "Rule.h"
#include "LoadingScene.h"

SceneManager::SceneManager()
	: mnSceneType(SCENE_TYPE::SCENE_NONE)
	, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
	, mpCurrentScene(nullptr)
	, SceneHard(false)
	, SceneNormal(false)
{
}

SceneManager::~SceneManager()
{
	// 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ繧帝亟縺舌◆繧√い繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ縺ｮ隗｣謾ｾ繧剃ｿ晁ｨｼ縺吶ｋ
	Finalize();
}

void SceneManager::Initialize()
{
	// 莉墓ｧ倅ｸ願ｵｷ蜍墓凾縺ｯ繧ｿ繧､繝医Ν逕ｻ髱｢縺九ｉ髢句ｧ九☆繧・
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Update();
	}
}

void SceneManager::Draw()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Draw();
	}
}

/*
 * 迴ｾ蝨ｨ縺ｮ繧｢繧ｯ繝・ぅ繝悶す繝ｼ繝ｳ繧貞ｮ牙・縺ｫ隗｣謾ｾ縺励∝ｾ檎援莉倥￠繧定｡後≧
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mpCurrentScene 縺ｮ Finalize 蜻ｼ縺ｳ蜃ｺ縺励√♀繧医・繝｡繝｢繝ｪ delete
 */
void SceneManager::Finalize()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Finalize();
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}
}

/*
 * 驕ｷ遘ｻ蜈域欠螳壹′逡ｰ縺ｪ繧句ｴ蜷医∫樟蝨ｨ繧ｷ繝ｼ繝ｳ繧堤ｴ譽・＠谺｡縺ｮ繧ｷ繝ｼ繝ｳ繧偵う繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ蛹悶☆繧・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 繧ｷ繝ｼ繝ｳ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝｡繝｢繝ｪ隗｣謾ｾ縺ｨ譁ｰ隕丞牡繧雁ｽ薙※縲∝・譛溷喧
 */
void SceneManager::ChangeSceneIfNeeded()
{
	// 驕ｷ遘ｻ蜈医′螟峨ｏ縺｣縺ｦ縺・↑縺・ｴ蜷医・繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺医ｒ陦後ｏ縺ｪ縺・
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}

	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Finalize();
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}

	mnSceneType = mnNextSceneType;

	switch (mnSceneType)
	{
	case SCENE_TYPE::SCENE_LOADING:
		mpCurrentScene = new LoadingScene();
		break;
	case SCENE_TYPE::SCENE_3D:
		mpCurrentScene = new Scene3D();
		break;
	case SCENE_TYPE::SCENE_TITLE:
		mpCurrentScene = new TitleScene();
		break;
	case SCENE_TYPE::SCENE_TUTORIAL:
		mpCurrentScene = new TutorialScene();
		break;
	case SCENE_TYPE::SCENE_RESULT:
		mpCurrentScene = new ResultScene();
		break;
	case SCENE_TYPE::SCENE_RULE:
		mpCurrentScene = new Rule();
		break;
	default:
		break;
	}

	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Initialize();
	}
}
