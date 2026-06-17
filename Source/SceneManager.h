#pragma once
#include"Scene.h"

// シーン遷移およびライフサイクルを管理するクラス
class SceneManager
{
public:
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_TITLE,
		SCENE_RULE,
		SCENE_TUTORIAL,
		SCENE_LEVEL,
		SCENE_GAME,
		SCENE_3DHARD,
		SCENE_RESULT,
		SCENE_RESULTWIN,
		SCENE_NORMALRESULTSCENE,
		SCENE_3D
	};

public:
	SceneManager();
	~SceneManager();

	/*
	 * 初期設定を行い、最初のシーン（タイトル）へ遷移する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mnNextSceneTypeの更新およびシーン切り替え
	 */
	void Initialize();

	/*
	 * 現在アクティブなシーンの描画処理を呼び出す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面への描画処理
	 */
	void Draw();

	/*
	 * 現在アクティブなシーンの更新処理を呼び出す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 現在のシーン状態の更新
	 */
	void Update();

	/*
	 * アクティブなシーンオブジェクトを安全に解放・終了する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mpCurrentSceneのメモリ解放
	 */
	void Finalize();

	/*
	 * 遷移要求がある場合、古いシーンを破棄し新しいシーンを生成・初期化する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] シーンオブジェクトのメモリ解放と新規生成
	 */
	void ChangeSceneIfNeeded();

	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }
	void SetSceneHard(bool Hard) { SceneHard = Hard; }
	bool GetSceneHard() { return SceneHard; }
	void SetSceneNormal(bool Normal) { SceneNormal = Normal; }
	bool GetSceneNormal() { return SceneNormal; }
	Scene* GetCurrentScene() { return mpCurrentScene; }
	SCENE_TYPE GetSceneType() const { return mnSceneType; }

private:
	SCENE_TYPE mnSceneType;
	SCENE_TYPE mnNextSceneType;
	Scene* mpCurrentScene;

	bool SceneHard;
	bool SceneNormal;
};
