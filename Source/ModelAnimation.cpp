#include "DxLib.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(int ModelHandle)
	: mnModelHandle(ModelHandle)
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
	// 繝ｫ繝ｼ繝医ヵ繝ｬ繝ｼ繝縺檎ｧｻ蜍輔い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｧ蜍晄焔縺ｫ蜍輔＞縺ｦ縺壹ｌ縺ｪ縺・ｈ縺・√Ο繝ｼ繧ｫ繝ｫ陦悟・繧貞崋螳壹☆繧・
	int moveAnimFrameIndex = MV1SearchFrame(mnModelHandle, "root");
	MV1SetFrameUserLocalMatrix(
		mnModelHandle,
		moveAnimFrameIndex,
		MV1GetFrameLocalMatrix(mnModelHandle, moveAnimFrameIndex)
	);

	ChangeAnimation(AnimationState::kAnimationNeutral);
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Update()
{
	// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ驕ｷ遘ｻ譎ゅ・繝悶Ξ繝ｳ繝臥紫繧・繝輔Ξ繝ｼ繝縺斐→縺ｫ騾ｲ陦後＆縺帙ｋ
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
		fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
		mfAnimationTime += mfAnimationCount;

		if (mfAnimationTime > fAnimTotaltime)
		{
			if (!mbLoop)
			{
				// 髱槭Ν繝ｼ繝励い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺檎ｵゆｺ・＠縺溷ｴ蜷医∵ｬ｡縺ｮ謖・ｮ夐・遘ｻ蜈医′縺ｪ縺代ｌ縺ｰ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ譎る俣繧呈ｭ｢繧√ｋ
				if (mnLoopFinishState == kAnimationMax)
				{
					mbLoopFinish = true;
					return;
				}
				ChangeAnimation(mnLoopFinishState);
				SetAnimationBlend(false);
				fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
			}
			mfAnimationTime = 0.0f;
		}

		MV1SetAttachAnimTime(mnModelHandle, mnAnimationIndex, mfAnimationTime);
		MV1SetAttachAnimBlendRate(mnModelHandle, mnAnimationIndex, mfAnimBlendRate);
	}

	if (mnOldAnimationIndex != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnOldAnimationIndex);

		if (mfOldAnimationTime > fAnimTotaltime)
		{
			mfOldAnimationTime = 0.0f;
		}

		// 蜿､縺・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝悶Ξ繝ｳ繝臥紫繧剃ｸ九￡縺ｦ縺・″縲∵怙邨ら噪縺ｫ繝輔ぉ繝ｼ繝峨い繧ｦ繝医＆縺帙ｋ
		MV1SetAttachAnimBlendRate(mnModelHandle, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);
	}
}

void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 譌｢縺ｫ蜷後§繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺碁∈謚槭＆繧後※縺・ｋ蝣ｴ蜷医・辟｡鬧・↑繝・ち繝・メ繝ｻ繧｢繧ｿ繝・メ繧帝∩縺代ｋ
	if (mnState == state)
	{
		return;
	}

	mnState = state;
	mbLoop = true;
	mnLoopFinishState = AnimationState::kAnimationMax;
	mbLoopFinish = false;

	// 蜿､縺吶℃繧九い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繝上Φ繝峨Ν縺梧ｮ九▲縺ｦ縺・ｋ蝣ｴ蜷医・螳悟・縺ｫ蛻・ｊ髮｢縺・
	if (mnOldAnimationIndex != -1)
	{
		MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
		mnOldAnimationIndex = -1;
	}

	mnOldAnimationIndex = mnAnimationIndex;
	mfOldAnimationTime = mfAnimationTime;

	mnAnimationIndex = MV1AttachAnim(mnModelHandle, (int)state);
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
		// 繝悶Ξ繝ｳ繝峨＠縺ｪ縺・ｴ蜷医・蜊ｳ蠎ｧ縺ｫ譁ｰ隕上い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｸ螳悟・蛻・ｊ譖ｿ縺医☆繧・
		mfAnimBlendRate = 1.0f;

		if (mnOldAnimationIndex != -1)
		{
			MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
			mnOldAnimationIndex = -1;
		}
	}
}

