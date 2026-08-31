#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

class EffectManager
{
public:
	/// @brief Effekseerの初期化処理を呼び出す
	/// @details メンバ変数の初期化、Effekseer初期化
	EffectManager();
	~EffectManager();

	/// @brief 描画デバイスの設定とEffekseerの起動を行う
	/// @details Effekseerのシステムが初期化される
	void Initalize();

	/// @brief 再生中の全エフェクトの状態を更新する
	/// @details エフェクトのアニメーションが進む
	void Update();

	/// @brief 空間上に存在する全てのエフェクトを描画する
	/// @details 画面にエフェクトが描画される
	void Draw();

	void PlayCowDeathEffect(VECTOR pos);

private:
	VECTOR play_pos_; ///< 座標や位置情報を管理する値

	const int kEffectParticleLimit = 20000; ///< ゲーム内で使用する固定値
	const float kEffectSize = 1.0f;         ///< ゲーム内で使用する固定値
	const int kEffectPlayerInterval = 300;  ///< ゲーム内で使用する固定値
	const float kEffectMoveSpeed = 0.2f;    ///< ゲーム内で使用する固定値

	int effect_resource_handle_;  ///< 描画やリソース管理に使用するハンドル
	int playing_effect_handle_;   ///< 描画やリソース管理に使用するハンドル
	int cow_death_effect_handle_; ///< 描画やリソース管理に使用するハンドル
};
