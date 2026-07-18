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
	/*
	 * シーン共通の変数を安全な初期値にするため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] メンバ変数の初期化
	 */
	Scene();

	/*
	 * 派生クラスのデストラクタを正しく呼び出しメモリリークを防ぐため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] シーンオブジェクトの破棄
	 */
	virtual ~Scene();

	/*
	 * 派生クラス固有のリソース確保や初期設定を行うため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] リソースや内部状態の初期化
	 */
	virtual void Initialize() = 0;

	/*
	 * 画面の明転・暗転演出でシーン遷移を滑らかに見せるため
	 * [入力] fade: フェードの進行状態
	 * [出力] なし
	 * [副作用] アルファ値の更新と黒矩形の描画
	 */
	virtual void Fade(SceneFade fade);

	/*
	 * フレームごとに全オブジェクトの状態を進めるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各マネージャーのUpdate関数の呼び出し
	 */
	virtual void Update();

	/*
	 * 更新された状態に基づき画面表示を更新するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各マネージャーのDraw関数の呼び出し
	 */
	virtual void Draw();

	/*
	 * 使用したリソースを解放しメモリリークを防ぐため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] オブジェクト等の解放処理
	 */
	virtual void Finalize() = 0;

	/*
	 * 外部からオブジェクトを操作・取得できるようにするため
	 * [入力] なし
	 * [出力] ObjectManagerのポインタ
	 * [副作用] なし
	 */
	ObjectManager* GetObjectManager() { return object_manager_; }

	/*
	 * 外部から衝突判定の設定を行えるようにするため
	 * [入力] なし
	 * [出力] ColliderManagerのポインタ
	 * [副作用] なし
	 */
	ColliderManager* GetCollisionManager() { return collider_manager_; }

	/*
	 * 現在のフェード不透明度を外部から参照するため
	 * [入力] なし
	 * [出力] フェード不透明度
	 * [副作用] なし
	 */
	float GetFadeAlpha() const { return fade_alpha_; }

	/*
	 * 外部からフェードの進行具合を強制的に変更するため
	 * [入力] alpha: 設定するフェード不透明度
	 * [出力] なし
	 * [副作用] fade_alpha_の変更
	 */
	void SetFadeAlpha(float alpha) { fade_alpha_ = alpha; }

	SceneFade fade_state_ = kSceneFadeNone;
	int next_scene_ = 0;

	Fever* fever_;
	AnimalManager* animal_manager_;
	CowManager* cow_manager_;
	GameManager* game_manager_;

private:
	ObjectManager* object_manager_;
	ColliderManager* collider_manager_;
	float fade_alpha_;
	float fade_speed_;
};
