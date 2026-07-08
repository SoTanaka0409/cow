#pragma once
#include "CowMove.h"

// 繧ｿ繧ｰCow_2逕ｨ繧ｯ繝ｩ繧ｹ・医せ繧ｳ繧｢遲峨・繝代Λ繝｡繝ｼ繧ｿ縺梧ｨ呎ｺ悶→逡ｰ縺ｪ繧九◆繧√・諡｡蠑ｵ・・
class Cow_2 : public CowMove
{
public:
	/*
	 * @brief 蛻晄悄蛹・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝代せ, initPos: 蛻晄悄驟咲ｽｮ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蛟句挨縺ｮ繧ｹ繧ｳ繧｢繝ｻ繧ｳ繝ｩ繧､繝繝ｼ蜊雁ｾ・・險ｭ螳・
	 */
	Cow_2(std::string filename, VECTOR initPos);
	virtual ~Cow_2();
};
