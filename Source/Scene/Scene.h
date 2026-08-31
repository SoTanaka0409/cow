#pragma once
#include"DxLib.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

class ObjectManager;
class ColliderManager;

class Scene
{
public:
	enum SceneFade
	{
		kSceneFadeIn,
		kSceneFadeOut,
		kSceneFadeNone,
		kSceneFadeLoad,
	};

public:
	/// @brief シーン共通の変数を安全な初期値にするため
	/// @details メンバ変数の初期化
	Scene();

	/// @brief 派生クラスのデストラクタを正しく呼び出しメモリリークを防ぐため
	/// @details シーンオブジェクトの破棄
	virtual ~Scene();

	/// @brief 派生クラス固有のリソース確保や初期設定を行うため
	/// @details リソースや内部状態の初期化
	virtual void Initialize() = 0;

	/// @brief 画面の明転・暗転演出でシーン遷移を滑らかに見せるため
	/// @param fade フェードの進行状態
	/// @details アルファ値の更新と黒矩形の描画
	virtual void Fade(SceneFade fade);

	/// @brief フレームごとに全オブジェクトの状態を進めるため
	/// @details 各マネージャーのUpdate関数の呼び出し
	virtual void Update();

	/// @brief 更新された状態に基づき画面表示を更新するため
	/// @details 各マネージャーのDraw関数の呼び出し
	virtual void Draw();

	/// @brief 使用したリソースを解放しメモリリークを防ぐため
	/// @details オブジェクト等の解放処理
	virtual void Finalize() = 0;

	/// @brief 外部からオブジェクトを操作・取得できるようにするため
	/// @return ObjectManagerのポインタ
	ObjectManager* GetObjectManager() { return object_manager_; }

	/// @brief 外部から衝突判定の設定を行えるようにするため
	/// @return ColliderManagerのポインタ
	ColliderManager* GetCollisionManager() { return collider_manager_; }

	/// @brief 現在のフェード不透明度を外部から参照するため
	/// @return フェード不透明度
	float GetFadeAlpha() const { return fade_alpha_; }

	/// @brief 外部からフェードの進行具合を強制的に変更するため
	/// @param alpha 設定するフェード不透明度
	/// @details fade_alpha_の変更
	void SetFadeAlpha(float alpha) { fade_alpha_ = alpha; }

	SceneFade fade_state_ = kSceneFadeNone; ///< 現在の状態や種別を管理する値
	int next_scene_ = 0;                    ///< 内部状態を管理する値

	Fever* fever_;                  ///< 内部状態を管理する値
	AnimalManager* animal_manager_; ///< 関連機能を管理するマネージャー
	CowManager* cow_manager_;       ///< 関連機能を管理するマネージャー
	GameManager* game_manager_;     ///< 関連機能を管理するマネージャー

private:
	ObjectManager* object_manager_;     ///< 関連機能を管理するマネージャー
	ColliderManager* collider_manager_; ///< 当たり判定の管理に使用する情報
	float fade_alpha_;                  ///< 内部状態を管理する値
	float fade_speed_;                  ///< 移動や回転の計算に使用する値
};
