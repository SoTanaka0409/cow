#include "DxLib.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(int ModelHandle)
	: model_handle_(ModelHandle)
	, mfAnimationTime(0.0f)
	, mnAnimationIndex(-1)
	, mnState(AnimationState::kAnimationMax)
	, mfOldAnimationTime(0.0f)
	, mnOldAnimationIndex(-1)
	, mfAnimBlendRate(1.0f)
	, mfAnimationCount(0.5f)
	, mbLoop(true)
	, mnLoopFinishState(AnimationState::kAnimationMax)
	, mbLoopFinish(false)
{
	// 郢晢ｽｫ郢晢ｽｼ郢晏現繝ｵ郢晢ｽｬ郢晢ｽｼ郢晢｣ｰ邵ｺ讙趣ｽｧ・ｻ陷崎ｼ斐＞郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｧ陷肴刋辟皮ｸｺ・ｫ陷崎ｼ費ｼ樒ｸｺ・ｦ邵ｺ螢ｹ・檎ｸｺ・ｪ邵ｺ・・育ｸｺ・ﾂ竏墅溽ｹ晢ｽｼ郢ｧ・ｫ郢晢ｽｫ髯ｦ謔溘・郢ｧ雋槫ｴ玖楜螢ｹ笘・ｹｧ・
	int moveAnimFrameIndex = MV1SearchFrame(model_handle_, "root");
	MV1SetFrameUserLocalMatrix(
		model_handle_,
		moveAnimFrameIndex,
		MV1GetFrameLocalMatrix(model_handle_, moveAnimFrameIndex)
	);

	ChangeAnimation(AnimationState::kAnimationNeutral);
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Update()
{
	// 郢ｧ・｢郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ鬩包ｽｷ驕假ｽｻ隴弱ｅ・郢晄じﾎ樒ｹ晢ｽｳ郢晁・邏ｫ郢ｧ・郢晁ｼ釆樒ｹ晢ｽｼ郢晢｣ｰ邵ｺ譁絶・邵ｺ・ｫ鬨ｾ・ｲ髯ｦ蠕鯉ｼ・ｸｺ蟶呻ｽ・
	if (mfAnimBlendRate < 1.0f)
	{
		mfAnimBlendRate += 0.1f;
		if (mfAnimBlendRate > 1.0f)
		{
			mfAnimBlendRate = 1.0f;
		}
	}

	float fAnimTotaltime = 0.0f;

	if (mnAnimationIndex != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, mnAnimationIndex);
		mfAnimationTime += mfAnimationCount;

		if (mfAnimationTime > fAnimTotaltime)
		{
			if (!mbLoop)
			{
				// 鬮ｱ讒ｭﾎ晉ｹ晢ｽｼ郢晏干縺・ｹ昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ讙趣ｽｵ繧・ｽｺ・・邵ｺ貅ｷ・ｰ・ｴ陷ｷ蛹ｻﾂ竏ｵ・ｬ・｡邵ｺ・ｮ隰悶・・ｮ螟舌・驕假ｽｻ陷亥現窶ｲ邵ｺ・ｪ邵ｺ莉｣・檎ｸｺ・ｰ郢ｧ・｢郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ隴弱ｋ菫｣郢ｧ蜻茨ｽｭ・｢郢ｧ竏夲ｽ・
				if (mnLoopFinishState == kAnimationMax)
				{
					mbLoopFinish = true;
					return;
				}
				ChangeAnimation(mnLoopFinishState);
				SetAnimationBlend(false);
				fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, mnAnimationIndex);
			}
			mfAnimationTime = 0.0f;
		}

		MV1SetAttachAnimTime(model_handle_, mnAnimationIndex, mfAnimationTime);
		MV1SetAttachAnimBlendRate(model_handle_, mnAnimationIndex, mfAnimBlendRate);
	}

	if (mnOldAnimationIndex != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, mnOldAnimationIndex);

		if (mfOldAnimationTime > fAnimTotaltime)
		{
			mfOldAnimationTime = 0.0f;
		}

		// 陷ｿ・､邵ｺ・縺・ｹ昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｮ郢晄じﾎ樒ｹ晢ｽｳ郢晁・邏ｫ郢ｧ蜑・ｽｸ荵晢ｿ｡邵ｺ・ｦ邵ｺ・窶ｳ邵ｲ竏ｵ諤咎お繧牙飭邵ｺ・ｫ郢晁ｼ斐♂郢晢ｽｼ郢晏ｳｨ縺・ｹｧ・ｦ郢晏現・・ｸｺ蟶呻ｽ・
		MV1SetAttachAnimBlendRate(model_handle_, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);
	}
}

void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 隴鯉ｽ｢邵ｺ・ｫ陷ｷ蠕個ｧ郢ｧ・｢郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ遒≫・隰壽ｧｭ・・ｹｧ蠕娯ｻ邵ｺ・・玖撻・ｴ陷ｷ蛹ｻ・霎滂ｽ｡鬯ｧ・竊醍ｹ昴・縺｡郢昴・繝｡郢晢ｽｻ郢ｧ・｢郢ｧ・ｿ郢昴・繝｡郢ｧ蟶昶茜邵ｺ莉｣・・
	if (mnState == state)
	{
		return;
	}

	mnState = state;
	mbLoop = true;
	mnLoopFinishState = AnimationState::kAnimationMax;
	mbLoopFinish = false;

	// 陷ｿ・､邵ｺ蜷ｶ邃・ｹｧ荵昴＞郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ郢昜ｸ莞ｦ郢晏ｳｨﾎ晉ｸｺ譴ｧ・ｮ荵昶夢邵ｺ・ｦ邵ｺ・・玖撻・ｴ陷ｷ蛹ｻ・陞ｳ謔溘・邵ｺ・ｫ陋ｻ・・企ｫｮ・｢邵ｺ・
	if (mnOldAnimationIndex != -1)
	{
		MV1DetachAnim(model_handle_, mnOldAnimationIndex);
		mnOldAnimationIndex = -1;
	}

	mnOldAnimationIndex = mnAnimationIndex;
	mfOldAnimationTime = mfAnimationTime;

	mnAnimationIndex = MV1AttachAnim(model_handle_, (int)state);
	mfAnimationTime = 0.0f;
}

void ModelAnimation::SetAnimationBlend(bool isblend)
{
	if (isblend)
	{
		mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else
	{
		// 郢晄じﾎ樒ｹ晢ｽｳ郢晏ｳｨ・邵ｺ・ｪ邵ｺ・・ｰ・ｴ陷ｷ蛹ｻ・陷奇ｽｳ陟趣ｽｧ邵ｺ・ｫ隴・ｽｰ髫穂ｸ翫＞郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ・ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ・ｸ陞ｳ謔溘・陋ｻ・・願ｭ厄ｽｿ邵ｺ蛹ｻ笘・ｹｧ・
		mfAnimBlendRate = 1.0f;

		if (mnOldAnimationIndex != -1)
		{
			MV1DetachAnim(model_handle_, mnOldAnimationIndex);
			mnOldAnimationIndex = -1;
		}
	}
}

