#pragma once
#include"Scene.h"

// シーン遷移を管理する
class SceneManager
{
public:
	enum SCENE_TYPE
	{
		kSceneNone = 0,
		kSceneTitle,
		kSceneRule,
		kSceneTutorial,
		kSceneLevel,
		kSceneGame,
		kGameSceneHard,
		kSceneResult,
		kSceneResultWin,
		kSceneNormalResult,
		kSceneLoading,
		kGameScene
	};
public:
	SceneManager();
	~SceneManager();

	// 最初のシーンを設定し、ゲームの初期状態を作る
	void Initialize();

	// 現在のシーンを描画する
	void Draw();

	// 現在のシーンを1フレーム進める
	void Update();

	// 現在のシーンを破棄してメモリリークを防ぐ
	void Finalize();

	// 遷移要求があれば現在のシーンを破棄し、次のシーンを生成する
	void ChangeSceneIfNeeded();
	void SetNextScene(SCENE_TYPE next) { next_scene_type_ = next; }
	void SetSceneHard(bool Hard) { scene_hard_ = Hard; }
	bool GetSceneHard() { return scene_hard_; }
	void SetSceneNormal(bool Normal) { scene_normal_ = Normal; }
	bool GetSceneNormal() { return scene_normal_; }
	Scene* GetCurrentScene() { return current_scene_; }
	SCENE_TYPE GetSceneType() const { return scene_type_; }
private:
	SCENE_TYPE scene_type_;
	SCENE_TYPE next_scene_type_;
	Scene* current_scene_;
	bool scene_hard_;
	bool scene_normal_;
};
