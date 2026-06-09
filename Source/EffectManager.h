#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

// Effekseerエフェクトエンジンのグローバルな初期化、毎フレームの更新・描画サイクルを管理するクラス
class EffectManager
{
public:
	/*
	 * @brief メンバ変数を初期化し、Effekseer初期化メソッドを呼び出す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	EffectManager();
	~EffectManager();

	/*
	 * @brief Direct3D設定、最大パーティクル数設定、スクリーンリセットコールバック等を登録してEffekseerを起動する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] Effekseerライブラリのグローバルな初期化
	 */
	void Initalize();

	/*
	 * @brief Effekseer全体の毎フレームの3Dエフェクト再生状態を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 再生中の全Effekseerエフェクトの進行更新
	 */
	void Update();

	/*
	 * @brief 再生中のすべてのEffekseerエフェクトを3D空間上にレンダリングする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

private:
	VECTOR PlayPos; // テスト再生用の座標（未使用）

	// パラメータ定数
	const int EffectParticleLimit = 20000;  // 画面上に同時に表示可能な最大パーティクル数
	const float EffectSize = 1.0f;          // 基準エフェクトサイズ
	const int EffectPlayerInterval = 300;   // エフェクト再生の間隔フレーム数
	const float EffectMoveSpeed = 0.2f;     // エフェクトの移動速度設定

	// エフェクト再生管理ハンドル
	int effectResourcehandle;               // ロードされたエフェクトのリソースハンドル（未使用）
	int playingEffecthandle;                // 再生中のエフェクトインスタンスハンドル（未使用）
};
