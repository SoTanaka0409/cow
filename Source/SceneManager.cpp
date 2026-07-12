#include"SceneManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"TitleScene.h"
#include"TutorialScene.h"
#include"ResultScene.h"
#include "Rule.h"
#include "LoadingScene.h"

SceneManager::SceneManager()
	: scene_type_(SCENE_TYPE::kSceneNone)
	, next_scene_type_(SCENE_TYPE::kSceneNone)
	, current_scene_(nullptr)
	, scene_hard_(false)
	, scene_normal_(false)
{
}

SceneManager::~SceneManager()
{
	// メモリリークを防ぐためアクチE��ブなシーンの解放を保証する
	Finalize();
}

void SceneManager::Initialize()
{
	// 仕様上起動時はタイトル画面から開始すめE
	next_scene_type_ = SCENE_TYPE::kSceneTitle;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Update();
	}
}

void SceneManager::Draw()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Draw();
	}
}

/*
 * 現在のアクチE��ブシーンを安�Eに解放し、後片付けを行う
 * [入力] なぁE
 * [出力] なぁE
 * [副作用] current_scene_ の Finalize 呼び出し、およ�Eメモリ delete
 */
void SceneManager::Finalize()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Finalize();
		delete current_scene_;
		current_scene_ = nullptr;
	}
}

/*
 * 遷移先指定が異なる場合、現在シーンを破棁E��次のシーンをインスタンス化すめE
 * [入力] なぁE
 * [出力] なぁE
 * [副作用] シーンオブジェクト�Eメモリ解放と新規割り当て、�E期化
 */
void SceneManager::ChangeSceneIfNeeded()
{
	// 遷移先が変わってぁE��ぁE��合�Eシーン刁E��替えを行わなぁE
	if (scene_type_ == next_scene_type_)
	{
		return;
	}

	if (current_scene_ != nullptr)
	{
		current_scene_->Finalize();
		delete current_scene_;
		current_scene_ = nullptr;
	}

	scene_type_ = next_scene_type_;

	switch (scene_type_)
	{
	case SCENE_TYPE::kSceneLoading:
		current_scene_ = new LoadingScene();
		break;
	case SCENE_TYPE::kScene3D:
		current_scene_ = new Scene3D();
		break;
	case SCENE_TYPE::kSceneTitle:
		current_scene_ = new TitleScene();
		break;
	case SCENE_TYPE::kSceneTutorial:
		current_scene_ = new TutorialScene();
		break;
	case SCENE_TYPE::kSceneResult:
		current_scene_ = new ResultScene();
		break;
	case SCENE_TYPE::kSceneRule:
		current_scene_ = new Rule();
		break;
	default:
		break;
	}

	if (current_scene_ != nullptr)
	{
		current_scene_->Initialize();
	}
}
