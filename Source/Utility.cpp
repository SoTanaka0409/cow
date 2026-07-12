#include "Utility.h"

/*
 * @brief 蠎ｦ・・egree・峨ｒ繝ｩ繧ｸ繧｢繝ｳ・・adian・峨↓螟画鋤縺吶ｋ
 * [蜈･蜉嫋 deg: 蠎ｦ謨ｰ蛟､
 * [蜃ｺ蜉嫋 螟画鋤蠕後・繝ｩ繧ｸ繧｢繝ｳ蛟､ (float)
 * [蜑ｯ菴懃畑] なし
 */
float Utility::DegToRad(float deg)
{
	return deg * DX_PI_F / 180.0f;
}
