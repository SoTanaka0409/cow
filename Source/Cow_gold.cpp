#include "ServiceLocator.h"
#include "Cow_gold.h"
#include "CapsuleCollider.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Bait.h"
#include "Wall.h"

Cow_gold::Cow_gold(std::string filename, VECTOR initPos, TagFever kFever)
	: CowMove(filename, initPos)
	, fever_(kFever)
	, death_count_(0)
	, death_timer_(1200)
{
	SetTagCow(CowMove::kCowGold);
	mfXp = 20;
	mfScore = 30;
	// 繝励Ξ繧､繝､繝ｼ縺梧黒迯ｲ縺励ｄ縺吶＞繧医≧縺ｫ蠖薙◆繧雁愛螳壹ｒ螟ｧ縺阪ａ縺ｫ險ｭ螳・
	collider_radius_ = 150.0f;
}

Cow_gold::~Cow_gold()
{
}

void Cow_gold::Reset(VECTOR pos)
{
	CowMove::Reset(pos);
	death_count_ = 0;
}

void Cow_gold::Update()
{
	death_count_++;
	CowMove::Update();

	// 繝輔ぅ繝ｼ繝舌・邨ゆｺ・凾繧・ｯｿ蜻ｽ雜・℃縺ｧ逕ｻ髱｢蜀・↓谿九ｊ邯壹￠繧九・繧帝亟縺舌◆繧∵ｶ域ｻ・＆縺帙ｋ
	auto fv = ServiceLocator::GetFever();
	if (fever_ == kFever && (fv == nullptr || fv->IsFever() == false || death_count_ >= death_timer_))
	{
		Die(DEATH_LIMIT);
	}
}

void Cow_gold::Die(DeathReason reason)
{
	// 莠碁㍾隗｣謾ｾ繧・ｸ肴紛蜷医ｒ髦ｲ縺舌◆繧√∵里縺ｫ豸域ｻ・・逅・ｸｭ縺ｪ繧牙ｼｾ縺・
	if (mDeleteFlag) return;
	CowMove::Die(reason);

	// 繝励Ξ繧､繝､繝ｼ縺ｮ逶ｴ謗･逧・い繧ｯ繧ｷ繝ｧ繝ｳ・亥精蠑輔・繧ｨ繧ｵ・峨〒謐慕佐縺輔ｌ縺溷ｴ蜷医・縺ｿ逋ｺ蜍輔＆縺帙ｋ
	if (reason == DEATH_VACUUM || reason == DEATH_BAIT)
	{
		if (this->fever_ == kNoFever)
		{
			if (auto fv = ServiceLocator::GetFever())
			{
				fv->StartFever(mpTargetPlayer);
			}
		}
	}
}