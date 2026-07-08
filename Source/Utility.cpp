#include "Utility.h"

/*
 * @brief 度（Degree）をラジアン（Radian）に変換する
 * [入力] deg: 度数値
 * [出力] 変換後のラジアン値 (float)
 * [副作用] なし
 */
float Utility::DegToRad(float deg)
{
	return deg * DX_PI_F / 180.0f;
}
