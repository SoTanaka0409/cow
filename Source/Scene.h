#pragma once
#include"DxLib.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

class ObjectManager;
class ColliderManager;

// 各ゲームシーンの抽象基底クラス
class Scene
{
public:
	// フェード状態
	enum SceneFade
	{
		SceneFade_In,   // 暗転から明転
		SceneFade_Out,  // 明転から暗転
		SceneFade_None, // フェードなし
		SceneFade_Load, // ロード中
	};
	
public:
	Scene();
	virtual ~Scene();

	/*
	 * @brief シーン初期化
	 * [入力] なし [出力] なし [副作用] リソース初期化
	 */
	virtual void Initialize() = 0;

	/*
	 * @brief フェード描画
	 * [入力] fade: フェード状態 [出力] なし [副作用] 画面描画、アルファ値更新
	 */
	virtual void Fade(SceneFade fade);

	/*
	 * @brief シーン内要素の更新
	 * [入力] なし [出力] なし [副作用] 各マネージャーのUpdate呼び出し
	 */
	virtual void Update();

	/*
	 * @brief シーン内要素の描画
	 * [入力] なし [出力] なし [副作用] 各マネージャーのDraw呼び出し
	 */
	virtual void Draw();

	/*
	 * @brief シーン終了処理
	 * [入力] なし [出力] なし [副作用] オブジェクト等の解放
	 */
	virtual void Finalize() = 0;

	// ゲッター・セッター群
	ObjectManager* GetObjectManager() { return objectManager; }
	ColliderManager* GetCollisionManager() { return colliderManager; }
	float GetFadeAlpha() const { return fadeAlpha; }
	void SetFadeAlpha(float alpha) { fadeAlpha = alpha; }
	
	SceneFade fadeState = SceneFade_None; // フェード状態
	int nextScene = 0;                    // 次のシーンID

	Fever* fever;                        // フィーバー管理
	AnimalManager* animalManager;        // アニマル管理
	CowManager* cowManager;              // 牛管理
	GameManager* gameManager;            // ゲーム進行管理

private:
	ObjectManager* objectManager;        // オブジェクト管理
	ColliderManager* colliderManager;    // コライダー管理
	float fadeAlpha;                     // フェード不透明度 (0-255)
	float fadeSpeed;                     // フェード速度
};
