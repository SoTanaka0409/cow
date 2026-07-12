#pragma once
#include"DxLib.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

class ObjectManager;
class ColliderManager;

// 吁E??ームシーンの抽象基底クラス
class Scene
{
public:
	// フェード状?E
	enum SceneFade
	{
		kSceneFadeIn,   // 暗転から明転
		kSceneFadeOut,  // 明転から暗転
		kSceneFadeNone, // フェードなし
		kSceneFadeLoad, // ロード中
	};
	
public:
	Scene();
	virtual ~Scene();

	/*
	 * @brief シーン初期匁E
	 * [入力] なし[出力] なし[副作用] リソース初期匁E
	 */
	virtual void Initialize() = 0;

	/*
	 * @brief フェード描画
	 * [入力] fade: フェード状?E[出力] なし[副作用] 画面描画、アルファ値更新
	 */
	virtual void Fade(SceneFade fade);

	/*
	 * @brief シーン?E??素の更新
	 * [入力] なし[出力] なし[副作用] 吁E?Eネ?EジャーのUpdate呼び出ぁE
	 */
	virtual void Update();

	/*
	 * @brief シーン?E??素の描画
	 * [入力] なし[出力] なし[副作用] 吁E?Eネ?EジャーのDraw呼び出ぁE
	 */
	virtual void Draw();

	/*
	 * @brief シーン終?E?E?E
	 * [入力] なし[出力] なし[副作用] オブジェクト等?E解放
	 */
	virtual void Finalize() = 0;

	// ゲチE??ー・セチE??ー群
	ObjectManager* GetObjectManager() { return object_manager_; }
	ColliderManager* GetCollisionManager() { return collider_manager_; }
	float GetFadeAlpha() const { return fade_alpha_; }
	void SetFadeAlpha(float alpha) { fade_alpha_ = alpha; }
	
	SceneFade fade_state_ = kSceneFadeNone; // フェード状?E
	int next_scene_ = 0;                    // 次のシーンID

	Fever* fever_;                        // フィーバ?E管?E
	AnimalManager* animal_manager_;        // アニ?Eル管?E
	CowManager* cow_manager_;              // 牛管?E
	GameManager* game_manager_;            // ゲーム進行管?E

private:
	ObjectManager* object_manager_;        // オブジェクト管?E
	ColliderManager* collider_manager_;    // コライダー管?E
	float fade_alpha_;                     // フェード不透?E度 (0-255)
	float fade_speed_;                     // フェード速度
};
