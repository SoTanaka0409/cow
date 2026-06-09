#pragma once
#include"Scene.h"

// シーン（画面）の遷移およびライフサイクル（初期化・更新・描画・終了）を管理するクラス
class SceneManager
{
public:
	// 定義されているシーン（画面）のID
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_TITLE,             // タイトル画面
		SCENE_RULE,              // ルール・操作説明画面
		SCENE_TUTORIAL,          // チュートリアル画面
		SCENE_LEVEL,             // （予約・未使用）
		SCENE_GAME,              // （予約・未使用）
		SCENE_3DHARD,            // ハード難易度（予約・未使用）
		SCENE_RESULT,            // リザルト画面
		SCENE_RESULTWIN,         // （予約・未使用）
		SCENE_NORMALRESULTSCENE, // （予約・未使用）
		SCENE_3D                 // ゲーム本編の3Dステージ画面
	};

public:
	SceneManager();
	~SceneManager();

	/*
	 * @brief シーンマネージャーの初期設定を行い、最初のシーン（タイトル）へ遷移する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mnNextSceneTypeの更新および最初のChangeSceneIfNeeded実行
	 */
	void Initialize();

	/*
	 * @brief 現在アクティブなシーンの描画処理を呼び出す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面への描画処理
	 */
	void Draw();

	/*
	 * @brief 現在アクティブなシーンの更新処理を呼び出す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 現在のシーン状態の更新
	 */
	void Update();

	/*
	 * @brief アクティブなシーンオブジェクトを安全に解放・終了する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mpCurrentSceneのFinalize実行とメモリ解放
	 */
	void Finalize();

	/*
	 * @brief 遷移要求（mnSceneType != mnNextSceneType）がある場合、古いシーンを破棄し新しいシーンを生成・初期化する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 古いシーンのFinalize/deleteの実行、新しいシーンオブジェクトの生成(new)とInitialize実行
	 */
	void ChangeSceneIfNeeded();

	// ゲッター・セッター群
	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }
	void SetSceneHard(bool Hard) { SceneHard = Hard; }
	bool GetSceneHard() { return SceneHard; }
	void SetSceneNormal(bool Normal) { SceneNormal = Normal; }
	bool GetSceneNormal() { return SceneNormal; }
	Scene* GetCurrentScene() { return mpCurrentScene; }
	SCENE_TYPE GetSceneType() const { return mnSceneType; }

private:
	SCENE_TYPE mnSceneType;      // 現在アクティブなシーンのID
	SCENE_TYPE mnNextSceneType;  // 次に遷移する予定のシーンID
	Scene* mpCurrentScene;       // 現在アクティブなシーンのインスタンスへのポインタ

	bool SceneHard;              // ハードモードフラグ
	bool SceneNormal;            // ノーマルモードフラグ
};
