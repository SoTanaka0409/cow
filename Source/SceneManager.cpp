#include"SceneManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"TitleScene.h"
#include"TutorialScene.h"
#include"ResultScene.h"
#include "Rule.h"

SceneManager::SceneManager()
	: sceneType(SCENE_TYPE::SCENE_NONE)
	, nextSceneType(SCENE_TYPE::SCENE_NONE)
	, currentScene(nullptr)
	, SceneHard(false)
	, SceneNormal(false)
{
}

SceneManager::~SceneManager()
{
	// メモリリークを防ぐためアクティブなシーンの解放を保証する
	Finalize();
}

void SceneManager::Initialize()
{
	// 仕様上起動時はタイトル画面から開始する
	nextSceneType = SCENE_TYPE::SCENE_TITLE;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
}

void SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw();
	}
}

/*
 * 現在のアクティブシーンを安全に解放し、後片付けを行う
 * [入力] なし
 * [出力] なし
 * [副作用] currentScene の Finalize 呼び出し、およびメモリ delete
 */
void SceneManager::Finalize()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

/*
 * 遷移先指定が異なる場合、現在シーンを破棄し次のシーンをインスタンス化する
 * [入力] なし
 * [出力] なし
 * [副作用] シーンオブジェクトのメモリ解放と新規割り当て、初期化
 */
void SceneManager::ChangeSceneIfNeeded()
{
	// 遷移先が変わっていない場合はシーン切り替えを行わない
	if (sceneType == nextSceneType)
	{
		return;
	}

	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}

	sceneType = nextSceneType;

	switch (sceneType)
	{
	case SCENE_TYPE::SCENE_3D:
		currentScene = new Scene3D();
		break;
	case SCENE_TYPE::SCENE_TITLE:
		currentScene = new TitleScene();
		break;
	case SCENE_TYPE::SCENE_TUTORIAL:
		currentScene = new TutorialScene();
		break;
	case SCENE_TYPE::SCENE_RESULT:
		currentScene = new ResultScene();
		break;
	case SCENE_TYPE::SCENE_RULE:
		currentScene = new Rule();
		break;
	default:
		break;
	}

	if (currentScene != nullptr)
	{
		currentScene->Initialize();
	}
}
