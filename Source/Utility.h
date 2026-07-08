#pragma once
#include "DxLib.h"

// ゲーム内で共通して使用されるスクリーンサイズ定数やユーティリティ関数を定義する静的クラス
class Utility
{
public:
	static const int SCREEN_WIDTH = 1600;  // ゲームウィンドウの解像度（横幅）
	static const int SCREEN_HEIGHT = 900;  // ゲームウィンドウの解像度（高さ）

	// UI用共通定数
	static const int UI_BASE_X = 20;       // UI項目(LIMIT, SCORE, COMBO)のX座標
	static const int UI_DIGIT_X = 220;     // UI数値(0~9)のX座標

	static const int UI_PANEL_W = 200;     // パネル系UIの共通幅
	static const int UI_PANEL_H = 100;     // パネル系UIの共通高さ

	static const int UI_TIMER_Y = 0;       // タイマーUIのY座標
	static const int UI_SCORE_Y = 100;     // スコアUIのY座標
	static const int UI_COMBO_Y = 200;     // コンボUIのY座標

	static const int UI_VACUUM_Y = 700;    // バキュームゲージのY座標
	static const int UI_VACUUM_W = 300;    // バキュームゲージの幅
	static const int UI_VACUUM_H = 50;     // バキュームゲージの高さ

	static const int UI_LEVEL_Y = 800;     // レベルUIのY座標
	static const int UI_LEVEL_W = 300;     // レベルUIの幅
	static const int UI_LEVEL_H = 30;      // レベルUIの高さ

	static VECTOR StageSize;               // ステージ領域のX・Z方向の最大値範囲を定義する境界ベクトル

	/*
	 * @brief 度（Degree）をラジアン（Radian）に変換する
	 * [入力] deg: 度数値
	 * [出力] 変換後のラジアン値 (float)
	 * [副作用] なし
	 */
	static float DegToRad(float deg);

	/*
	 * @brief 安全にポインタをdeleteし、nullptrを代入する
	 * [入力] ptr: 破棄するポインタの参照
	 * [出力] なし
	 * [副作用] メモリ解放とnullptr代入
	 * ※テンプレート関数のためヘッダーに実装を残す必要がある
	 */
	template<typename T>
	static void SafeDelete(T*& ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
};
