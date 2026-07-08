#pragma once
#include "CowMove.h"
#include "dxlib.h"

// 繝√Η繝ｼ繝医Μ繧｢繝ｫ騾ｲ陦檎畑縺ｮ迚ｹ谿願ｪｿ謨ｴ・医せ繧ｳ繧｢鬮倥ａ遲会ｼ峨ｒ陦後≧蟆ら畑繧ｯ繝ｩ繧ｹ
class Cow_Tutorial : public CowMove
{
public:
	/*
	 * @brief 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ蝗ｺ譛峨ヱ繝ｩ繝｡繝ｼ繧ｿ繧定ｨｭ螳壹＠蛻晄悄蛹悶☆繧・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝輔ぃ繧､繝ｫ繝代せ, initPos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	Cow_Tutorial(std::string filename, VECTOR initPos);

	/*
	 * @brief 繝・せ繝医Λ繧ｯ繧ｿ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	virtual ~Cow_Tutorial();
};
