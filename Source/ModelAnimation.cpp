#include "DxLib.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(int ModelHandle)
	: mnModelHandle(ModelHandle)
	, mfAnimationTime(0.0f)
	, mnAnimationIndex(-1)
	, mnState(AnimationState::ANIMATION_MAX)
	, mfOldAnimationTime(0.0f)
	, mnOldAnimationIndex(-1)
	, mfAnimBlendRate(1.0f)
	, mfAnimationCount(0.5f)
	, mbLoop(true)
	, mnLoopFinishState(AnimationState::ANIMATION_MAX)
	, mbLoopFinish(false)
{
	// ルートフレームが移動アニメーションで勝手に動いてずれないよう、ローカル行列を固定する
	int moveAnimFrameIndex = MV1SearchFrame(mnModelHandle, "root");
	MV1SetFrameUserLocalMatrix(
		mnModelHandle,
		moveAnimFrameIndex,
		MV1GetFrameLocalMatrix(mnModelHandle, moveAnimFrameIndex)
	);

	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Update()
{
	// アニメーション遷移時のブレンド率を1フレームごとに進行させる
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
				// 非ループアニメーションが終了した場合、次の指定遷移先がなければアニメーション時間を止める
				if (mnLoopFinishState == ANIMATION_MAX)
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

		// 古いアニメーションのブレンド率を下げていき、最終的にフェードアウトさせる
		MV1SetAttachAnimBlendRate(mnModelHandle, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);
	}
}

void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 既に同じアニメーションが選択されている場合は無駄なデタッチ・アタッチを避ける
	if (mnState == state)
	{
		return;
	}

	mnState = state;
	mbLoop = true;
	mnLoopFinishState = AnimationState::ANIMATION_MAX;
	mbLoopFinish = false;

	// 古すぎるアニメーションハンドルが残っている場合は完全に切り離す
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
		// ブレンドしない場合は即座に新規アニメーションへ完全切り替えする
		mfAnimBlendRate = 1.0f;

		if (mnOldAnimationIndex != -1)
		{
			MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
			mnOldAnimationIndex = -1;
		}
	}
}

