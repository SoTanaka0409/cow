#pragma once
#include "CowMove.h"

// 繧ｿ繧ｰCow_1縺ｮ讓呎ｺ悶く繝｣繝ｩ繧ｯ繧ｿ繝ｼ螳夂ｾｩ・亥渕譛ｬ陦悟虚縺ｯCowMove縺ｫ貅匁侠・・
class Cow : public CowMove
{
public:
	/*
	 * @brief 繧ｪ繝悶ず繧ｧ繧ｯ繝亥・譛溷喧
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝代せ, initPos: 蛻晄悄驟咲ｽｮ蠎ｧ讓・ num: 蝓ｺ遉弱せ繧ｳ繧｢蛟､
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｹ繧ｳ繧｢縺ｨ邨碁ｨ灘､縺ｮ險ｭ螳・
	 */
	Cow(std::string filename, VECTOR initPos, float num);
	virtual ~Cow();
};
