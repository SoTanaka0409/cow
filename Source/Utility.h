#pragma once
#include "DxLib.h"

// ゲーム内で共通して使用されるスクリーンサイズ定数やユーティリティ関数を定義する静的クラス
class Utility
{
public:
	static const int SCREEN_WIDTH = 1600;  // ゲームウィンドウの解像度（横幅）
	static const int SCREEN_HEIGHT = 900;  // ゲームウィンドウの解像度（高さ）

	// UI用
	static const int UI_BASE_X = 20;       // UI項目(LIMIT, SCORE, COMBO)のX座標
	static const int UI_DIGIT_X = 220;     // UI数値(0~9)のX座標

	static VECTOR StageSize;               // ステージ領域のX・Z方向の最大値範囲を定義する境界ベクトル

	/*
	 * @brief 度（Degree）をラジアン（Radian）に変換する
	 * [入力] deg: 度数値
	 * [出力] 変換後のラジアン値 (float)
	 * [副作用] なし
	 */
	static float DegToRad(float deg)
	{
		return deg * DX_PI_F / 180.0f;
	}
};

