#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"
#include "CreatureManager.h"

// 繧ｹ繝・・繧ｸ荳翫・迚帙◆縺｡縺ｮ蜍慕噪逕滓・縲∵峩譁ｰ縲√ち繧ｰ蛻､螳壹∽ｸ企剞邂｡逅・∬ｧ｣謾ｾ縺ｪ縺ｩ繧剃ｸ諡ｬ邂｡逅・☆繧九け繝ｩ繧ｹ
class CowManager : public CreatureManager<CowMove, CowMove::TagCow>
{
public:
	CowManager();

	/*
	 * @brief 謖・ｮ壹＆繧後◆遞ｮ鬘槭・迚帙ｒ繝ｩ繝ｳ繝繝縺ｪ菴咲ｽｮ縺ｫ隍・焚逕滓・縺吶ｋ・域怙螟ｧ30蛹ｹ蛻ｶ髯舌≠繧奇ｼ・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ, pos: 蜃ｺ迴ｾ荳ｭ必要渕貅門ｺｧ讓・ scale: 繝｢繝・Ν縺ｮ諡｡螟ｧ邇・ tag: 迚帙・遞ｮ鬘櫁ｭ伜挨繧ｿ繧ｰ, count: 逕滓・蛟区焚, is_fever: 繝輔ぅ繝ｼ繝舌・荳ｭ縺九←縺・°
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜍慕噪繝｡繝｢繝ｪ遒ｺ菫昴＆繧後◆迚帙が繝悶ず繧ｧ繧ｯ繝医′邂｡逅・Μ繧ｹ繝医↓霑ｽ蜉縺輔ｌ繧九°縲√・繝ｼ繝ｫ縺九ｉ蠕ｩ蟶ｰ縺吶ｋ
	 */
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever = false);

protected:
	CowMove::TagCow GetTag(CowMove* creature) override { return creature->GetTagCow(); }
};