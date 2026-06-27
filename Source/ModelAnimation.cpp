#include "DxLib.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(int ModelHandle)
	: modelHandle(ModelHandle)
	, animationTime(0.0f)
	, animationIndex(-1)
	, state(AnimationState::ANIMATION_MAX)
	, oldAnimationTime(0.0f)
	, oldAnimationIndex(-1)
	, animBlendRate(1.0f)
	, animationCount(0.5f)
	, loop(true)
	, loopFinishState(AnimationState::ANIMATION_MAX)
	, loopFinish(false)
{
	// ルートフレームが移動アニメーションで勝手に動いてずれないよう、ローカル行列を固定する
	int moveAnimFrameIndex = MV1SearchFrame(modelHandle, "root");
	MV1SetFrameUserLocalMatrix(
		modelHandle,
		moveAnimFrameIndex,
		MV1GetFrameLocalMatrix(modelHandle, moveAnimFrameIndex)
	);

	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Update()
{
	// アニメーション遷移時のブレンド率を1フレームごとに進行させる
	if (animBlendRate < 1.0f)
	{
		animBlendRate += 0.1f;
		if (animBlendRate > 1.0f)
		{
			animBlendRate = 1.0f;
		}
	}

	float fAnimTotaltime = 0.0f;

	if (animationIndex != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);
		animationTime += animationCount;

		if (animationTime > fAnimTotaltime)
		{
			if (!loop)
			{
				// 非ループアニメーションが終了した場合、次の指定遷移先がなければアニメーション時間を止める
				if (loopFinishState == ANIMATION_MAX)
				{
					loopFinish = true;
					return;
				}
				ChangeAnimation(loopFinishState);
				SetAnimationBlend(false);
				fAnimTotaltime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);
			}
			animationTime = 0.0f;
		}

		MV1SetAttachAnimTime(modelHandle, animationIndex, animationTime);
		MV1SetAttachAnimBlendRate(modelHandle, animationIndex, animBlendRate);
	}

	if (oldAnimationIndex != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(modelHandle, oldAnimationIndex);

		if (oldAnimationTime > fAnimTotaltime)
		{
			oldAnimationTime = 0.0f;
		}

		// 古いアニメーションのブレンド率を下げていき、最終的にフェードアウトさせる
		MV1SetAttachAnimBlendRate(modelHandle, oldAnimationIndex, 1.0f - animBlendRate);
	}
}

void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 既に同じアニメーションが選択されている場合は無駄なデタッチ・アタッチを避ける
	if (state == state)
	{
		return;
	}

	state = state;
	loop = true;
	loopFinishState = AnimationState::ANIMATION_MAX;
	loopFinish = false;

	// 古すぎるアニメーションハンドルが残っている場合は完全に切り離す
	if (oldAnimationIndex != -1)
	{
		MV1DetachAnim(modelHandle, oldAnimationIndex);
		oldAnimationIndex = -1;
	}

	oldAnimationIndex = animationIndex;
	oldAnimationTime = animationTime;

	animationIndex = MV1AttachAnim(modelHandle, (int)state);
	animationTime = 0.0f;
}

void ModelAnimation::SetAnimationBlend(bool isblend)
{
	if (isblend)
	{
		animBlendRate = (oldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else
	{
		// ブレンドしない場合は即座に新規アニメーションへ完全切り替えする
		animBlendRate = 1.0f;

		if (oldAnimationIndex != -1)
		{
			MV1DetachAnim(modelHandle, oldAnimationIndex);
			oldAnimationIndex = -1;
		}
	}
}

