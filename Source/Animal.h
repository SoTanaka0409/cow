#pragma once
#include "AnimalMove.h"

// 莉墓ｧ伜宛邏・ 繝励Ξ繧､繝､繝ｼ縺ｮ繧ｿ繝ｼ繧ｲ繝・ヨ蟇ｾ雎｡縺ｨ縺ｪ繧狗央莉･螟悶・荳闊ｬ蜍慕黄繧貞ｮ夂ｾｩ縺吶ｋ
class Animal : public AnimalMove
{
public:
	/*
	 * @brief 蜍慕黄繧ｪ繝悶ず繧ｧ繧ｯ繝医・逕滓・縺ｨ蛻晄悄驟咲ｽｮ
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ, initPos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｪ繝悶ず繧ｧ繧ｯ繝医・蛻晄悄蛹悶♀繧医・蜀・Κ繝代Λ繝｡繝ｼ繧ｿ縺ｮ險ｭ螳・
	 */
	Animal(std::string filename, VECTOR initPos);
	virtual ~Animal();

	/*
	 * @brief 繧ｪ繝悶ず繧ｧ繧ｯ繝医・豈弱ヵ繝ｬ繝ｼ繝譖ｴ譁ｰ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蠎ｧ讓吶ｄ繧ｳ繝ｩ繧､繝繝ｼ縺ｪ縺ｩ縺ｮ蜀・Κ迥ｶ諷区峩譁ｰ
	 */
	void Update() override;
};
