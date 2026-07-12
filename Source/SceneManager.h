#pragma once
#include"Scene.h"

// シーン遷移およびライフサイクルを管琁E��るクラス
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
		kScene3DHard,
		kSceneResult,
		kSceneResultWin,
		kSceneNormalResult,
		kSceneLoading,
		kScene3D
	};

public:
	SceneManager();
	~SceneManager();

	/*
	 * 初期設定を行い、最初�Eシーン�E�タイトル�E�へ遷移する
	 * [入力] なぁE
	 * [出力] なぁE
	 * [副作用] mnNextSceneTypeの更新およびシーン刁E��替ぁE
	 */
	void Initialize();

	/*
	 * 現在アクチE��ブなシーンの描画処琁E��呼び出ぁE
	 * [入力] なぁE
	 * [出力] なぁE
	 * [副作用] 画面への描画処琁E
	 */
	void Draw();

	/*
	 * 現在アクチE��ブなシーンの更新処琁E��呼び出ぁE
	 * [入力] なぁE
	 * [出力] なぁE
	 * [副作用] 現在のシーン状態�E更新
	 */
	void Update();

	/*
	 * アクチE��ブなシーンオブジェクトを安�Eに解放・終亁E��めE
	 * [入力] なぁE
	 * [出力] なぁE
	 * [副作用] mpCurrentSceneのメモリ解放
	 */
	void Finalize();

	/*
	 * 遷移要求がある場合、古ぁE��ーンを破棁E��新しいシーンを生成�E初期化すめE
	 * [入力] なぁE
	 * [出力] なぁE
	 * [副作用] シーンオブジェクト�Eメモリ解放と新規生戁E
	 */
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
