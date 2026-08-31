#include "Utility.h"

/// @param deg 度数値
/// @return 変換後のラジアン値（float）
/// @brief DxLibの内部計算（回転処理等）で使用するラジアン単位へ変換する
float Utility::DegToRad(float deg)
{
	return deg * DX_PI_F / 180.0f;
}
