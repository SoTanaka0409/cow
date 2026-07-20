#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

class EffectManager
{
public:
	/*
	 * Effekseerの初期化処理を呼び出す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] メンバ変数の初期化、Effekseer初期化
	 */
	EffectManager();
	~EffectManager();

	/*
	 * 描画デバイスの設定とEffekseerの起動を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] Effekseerのシステムが初期化される
	 */
	void Initalize();

	/*
	 * 再生中の全エフェクトの状態を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] エフェクトのアニメーションが進む
	 */
	void Update();

	/*
	 * 空間上に存在する全てのエフェクトを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面にエフェクトが描画される
	 */
	void Draw();

	void PlayCowDeathEffect(VECTOR pos);

private:
	VECTOR play_pos_;

	const int kEffectParticleLimit = 20000;
	const float kEffectSize = 1.0f;
	const int kEffectPlayerInterval = 300;
	const float kEffectMoveSpeed = 0.2f;

	int effect_resource_handle_;
	int playing_effect_handle_;
	int cow_death_effect_handle_;
};
