#pragma once

#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "AnimalMove.h"
#include "CreatureManager.h"

// 繧ｹ繝・・繧ｸ荳翫・荳闊ｬ繧｢繝九・繝ｫ・育央莉･螟厄ｼ峨・蜍慕噪逕滓・縲∵峩譁ｰ縲√ち繧ｰ蛻､螳壹∬ｧ｣謾ｾ縺ｪ縺ｩ繧剃ｸ諡ｬ邂｡逅・☆繧九け繝ｩ繧ｹ
class AnimalManager : public CreatureManager<AnimalMove, AnimalMove::TagAnimal>
{
public:
	AnimalManager();

	/*
	 * @brief 謖・ｮ壹＆繧後◆遞ｮ鬘槭・蜍慕黄繧偵Λ繝ｳ繝繝縺ｪ菴咲ｽｮ縺ｫ隍・焚逕滓・縺吶ｋ
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ, pos: 蜃ｺ迴ｾ荳ｭ必要渕貅門ｺｧ讓・ scale: 繝｢繝・Ν縺ｮ諡｡螟ｧ邇・ tag: 蜍慕黄縺ｮ遞ｮ鬘櫁ｭ伜挨繧ｿ繧ｰ, count: 逕滓・蛟区焚
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜍慕噪繝｡繝｢繝ｪ遒ｺ菫昴＆繧後◆蜍慕黄繧ｪ繝悶ず繧ｧ繧ｯ繝医′邂｡逅・Μ繧ｹ繝医↓霑ｽ蜉縺輔ｌ繧九√∪縺溘・繝励・繝ｫ縺九ｉ蠕ｩ蟶ｰ縺吶ｋ
	 */
	void SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::TagAnimal tag, int count);

protected:
	AnimalMove::TagAnimal GetTag(AnimalMove* creature) override { return creature->GetTagAnimal(); }
};