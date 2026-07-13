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
	 * 最初のシーンを設定し、ゲームの初期状態を構築する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] next_scene_type_をタイトルシーンに変更する
	 */
	void Initialize();
	/*
	 * アクティブなシーンの描画処理を委譲する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面にシーンのグラフィックを描画する
	 */
	void Draw();
	/*
	 * アクティブなシーンの状態を1フレーム進める
	 * [入力] なし
	 * [出力] なし
	 * [副作用] シーン内のオブジェクト状態を更新する
	 */
	void Update();
	/*
	 * 終了時にアクティブなシーンのメモリリークを防ぐため破棄する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] current_scene_のメモリを解放する
	 */
	void Finalize();
	/*
	 * 遷移要求がある場合、古いシーンを破棄して新しいシーンを生成する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] current_scene_を入れ替える
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
