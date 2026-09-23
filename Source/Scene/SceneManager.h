#pragma once
#include"Scene.h"

/// @brief シーン遷移を管理する
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

	/// @brief 最初のシーンを設定し、ゲームの初期状態を作る
	void Initialize();

	/// @brief 現在のシーンを描画する
	void Draw();

	/// @brief 現在のシーンを1フレーム進める
	void Update();

	/// @brief 現在のシーンを破棄してメモリリークを防ぐ
	void Finalize();

	/// @brief 遷移要求があれば現在のシーンを破棄し、次のシーンを生成する
	void ChangeSceneIfNeeded();
	/// @brief NextSceneの設定
	void SetNextScene(SCENE_TYPE next) { next_scene_type_ = next; }
	/// @brief SceneHardの設定
	void SetSceneHard(bool Hard) { scene_hard_ = Hard; }
	/// @brief SceneHardの取得
	bool GetSceneHard() const { return scene_hard_; }
	/// @brief SceneNormalの設定
	void SetSceneNormal(bool Normal) { scene_normal_ = Normal; }
	/// @brief SceneNormalの取得
	bool GetSceneNormal() const { return scene_normal_; }
	/// @brief CurrentSceneの取得
	Scene* GetCurrentScene() { return current_scene_; }
	/// @brief SceneTypeの取得
	SCENE_TYPE GetSceneType() const { return scene_type_; }
private:
	SCENE_TYPE scene_type_;      ///< 現在の状態や種別を管理する値
	SCENE_TYPE next_scene_type_; ///< 現在の状態や種別を管理する値
	Scene* current_scene_;       ///< 内部状態を管理する値
	bool scene_hard_;            ///< 内部状態を管理する値
	bool scene_normal_;          ///< 内部状態を管理する値
};
