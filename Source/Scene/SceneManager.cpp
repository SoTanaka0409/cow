#include "SceneManager.h"
#include "GameScene.h"
#include "Scene.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "ResultScene.h"
#include "Rule.h"
#include "LoadingScene.h"
/// @brief メンバ変数の初期状態を安全に定義するため
/// @details メンバ変数が初期化される
SceneManager::SceneManager()
	: scene_type_(SCENE_TYPE::kSceneNone)
	, next_scene_type_(SCENE_TYPE::kSceneNone)
	, current_scene_(nullptr)
	, scene_hard_(false)
	, scene_normal_(false)
{
}
/// @brief インスタンス破棄時にアクティブなシーンを安全に解放するため
SceneManager::~SceneManager()
{
	Finalize();
}
/// @brief アプリケーション起動時の初期状態を構成するため
/// @details next_scene_type_ が kSceneTitle に変更される
void SceneManager::Initialize()
{
	next_scene_type_ = SCENE_TYPE::kSceneTitle;
	ChangeSceneIfNeeded();
}
/// @brief フレームごとの状態更新を委譲するため
/// @details current_scene_ の内部状態が更新される
void SceneManager::Update()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Update();
	}
}
/// @brief フレームごとの描画処理を委譲するため
/// @details 画面に描画される
void SceneManager::Draw()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Draw();
	}
}
/// @brief シーン終了時や切り替え時にリソースリークを防ぐため
/// @details current_scene_ が破棄され nullptr になる
void SceneManager::Finalize()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Finalize();
		delete current_scene_;
		current_scene_ = nullptr;
	}
}
/// @brief 要求されたシーンへの遷移を遅延実行し、安全に切り替えるため
/// @details current_scene_ が新しいシーンに置き換わる
void SceneManager::ChangeSceneIfNeeded()
{
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
	case SCENE_TYPE::kGameScene:
		current_scene_ = new GameScene();
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
