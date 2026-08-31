#pragma once
#include "DxLib.h"

/// @brief ゲーム内で共有して使用されるスクリーンサイズ定数・ユーティリティ関数を定義する静的クラス
class Utility
{
public:
	static const int kScreenWidth  = 1600; ///< ゲームウィンドウの解像度（横幅）
	static const int kScreenHeight = 900;  ///< ゲームウィンドウの解像度（高さ）

	/// @brief UI用共通定数
	static const int kUiBaseX   = 20;  ///< UI見出し(LIMIT, SCORE, COMBO)のX座標
	static const int kUiDigitX  = 220; ///< UI数値(0~9)のX座標

	static const int kUiPanelW  = 200; ///< パネル系UIの共通幅
	static const int kUiPanelH  = 100; ///< パネル系UIの共通高さ

	static const int kUiTimerY  = 0;   ///< タイマーUIのY座標
	static const int kUiScoreY  = 100; ///< スコアUIのY座標
	static const int kUiComboY  = 200; ///< コンボUIのY座標

	static const int kUiVacuumY = 700; ///< バキュームゲージのY座標
	static const int kUiVacuumW = 300; ///< バキュームゲージの幅
	static const int kUiVacuumH = 50;  ///< バキュームゲージの高さ

	static const int kUiLevelY  = 800; ///< レベルUIのY座標
	static const int kUiLevelW  = 300; ///< レベルUIの幅
	static const int kUiLevelH  = 30;  ///< レベルUIの高さ

	static VECTOR StageSize; ///< ステージ境界のX・Z方向の最大値範囲を定義する補助ベクトル
	

	/// @brief 度（Degree）をラジアン（Radian）に変換する
	/// @param deg 度数値
	/// @return 変換後のラジアン値 (float)
	/// @details なし
	static float DegToRad(float deg);

	/// @brief 安全にポインタをdeleteしnullptrを代入する
	/// @param ptr 解放するポインタの参照
	/// @details メモリ解放とnullptr代入
	/// @details ※テンプレート関数のためヘッダに実装を残す必要がある
	template<typename T>
	static void SafeDelete(T*& ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
};
