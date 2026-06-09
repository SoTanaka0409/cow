#pragma once
#include"DxLib.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

class ObjectManager;
class ColliderManager;

// タイトル、ゲーム本編、リザルトなどの各ゲームシーンが継承する抽象基底クラス
class Scene
{
public:
	// フェードイン・フェードアウト状態の定義
	enum SceneFade
	{
		SceneFade_In,   // 暗転状態から画面が徐々に明るくなる
		SceneFade_Out,  // 画面が徐々に暗転する
		SceneFade_None, // フェード処理なし（通常時）
		SceneFade_Load, // ロード中のフェード状態
	};
	
public:
	Scene();
	virtual ~Scene();

	/*
	 * @brief 各シーンごとの初期化処理を行う純粋仮想関数
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各シーン依存のオブジェクト生成、リソース初期設定
	 */
	virtual void Initialize() = 0;

	/*
	 * @brief シーン遷移時のフェードイン・アウト描画処理を行う
	 * [入力] fade: フェードの遷移状態指定
	 * [出力] なし
	 * [副作用] 暗転用の黒い矩形を描画、フェードアルファ値(mfFadeAlpha)を更新
	 */
	virtual void Fade(SceneFade fade);

	/*
	 * @brief シーン内のコライダー・オブジェクトおよびゲームシステムの更新処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 登録コライダー・オブジェクトのUpdateの実行
	 */
	virtual void Update();

	/*
	 * @brief シーン内のオブジェクト・コライダーの描画処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] バックバッファへのオブジェクト描画
	 */
	virtual void Draw();

	/*
	 * @brief シーン終了時の後処理・アセット破棄を行う純粋仮想関数
	 * [入力] なし
	 * [出力] なし
	 * [副作用] シーン個別の動的オブジェクトの解放
	 */
	virtual void Finalize() = 0;

	// ゲッター・セッター群
	ObjectManager* GetObjectManager() { return mpObjectManager; }
	ColliderManager* GetCollisionManager() { return mpColliderManager; }
	float GetFadeAlpha() const { return mfFadeAlpha; }
	void SetFadeAlpha(float alpha) { mfFadeAlpha = alpha; }
	
	SceneFade mFadeState = SceneFade_None; // 現在のフェードフェーズ
	int mNextScene = 0;                    // 遷移先のシーンID

	Fever* mpFever;                        // フィーバーモード管理システムへのポインタ
	AnimalManager* mpAnimalManager;        // ステージ上の一般アニマルの管理マネージャー
	CowManager* mpCowManager;              // ステージ上の牛の管理マネージャー
	GameManager* mpGameManager;            // ゲーム本編の終了条件やタイマーなどを制御するシステム

private:
	ObjectManager* mpObjectManager;        // 3D/2Dのゲームオブジェクトを一括更新・描画・管理するマネージャー
	ColliderManager* mpColliderManager;    // シーン内のコライダー判定・更新を処理するマネージャー
	float mfFadeAlpha;                     // フェード暗転用の不透明度 (0 - 255)
	float mfFadeSpeed;                     // 1フレームあたりのフェードアルファ変化速度
};
