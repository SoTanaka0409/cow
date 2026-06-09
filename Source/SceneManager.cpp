#include"SceneManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"TitleScene.h"
#include"TutorialScene.h"
#include"ResultScene.h"
#include "Rule.h"

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
	Finalize(); // インスタンス破棄時にアクティブなシーンの解放を保証する
}

void SceneManager::Initialize()
{
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE; // 起動時はタイトル画面から開始
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
 * @brief 現在のアクティブシーンを安全に解放し、後片付けを行う
 * [入力] なし
 * [出力] なし
 * [副作用] mpCurrentScene の Finalize 呼び出し、およびメモリ delete
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
 * @brief 遷移先指定が異なる場合、現在シーンを破棄し次のシーンをインスタンス化する
 * [入力] なし
 * [出力] なし
 * [副作用] シーンオブジェクトのメモリ解放・新規割り当て(new)、および初期化(Initialize)
 */
void SceneManager::ChangeSceneIfNeeded()
{
	// 遷移先が変わっていない場合はシーン切り替えを行わない
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
