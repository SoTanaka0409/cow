#include "Cow_Tutorial.h"

Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 30;
	// 遏ｭ譎る俣縺ｧ繝ｬ繝吶Ν繧｢繝・・繧剃ｽ馴ｨ薙＆縺帙ｋ縺溘ａ邨碁ｨ灘､繧帝ｫ倥￥險ｭ螳・
	mfXp = 150;
	SetTagCow(CowMove::kCowT);
}

Cow_Tutorial::~Cow_Tutorial()
{
	// 隕ｪ繧ｯ繝ｩ繧ｹCowMove縺ｧ繝ｪ繧ｽ繝ｼ繧ｹ隗｣謾ｾ縺輔ｌ繧九◆繧√・
	// 螟夐㍾隗｣謾ｾ縺ｫ繧医ｋ繧ｯ繝ｩ繝・す繝･繧帝亟縺舌∋縺上％縺薙〒縺ｯ謇句虚delete縺励↑縺・
}