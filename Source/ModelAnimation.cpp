#include "DxLib.h"
#include "ModelAnimation.h"

/*
 * 入力: ModelHandle (管理対象となる3Dモデルのハンドル)
 * 出力: なし
 * 副作用: アニメーション変数の初期化、ルートボーンのローカル座標固定
 */
ModelAnimation::ModelAnimation(int ModelHandle)
	: model_handle_(ModelHandle)
	, animation_time_(0.0f)
	, animation_index_(-1)
	, state_(AnimationState::kAnimationMax)
	, old_animation_time_(0.0f)
	, old_animation_index_(-1)
	, anim_blend_rate_(1.0f)
	, animation_count_(0.5f)
	, is_loop_(true)
	, loop_finish_state_(AnimationState::kAnimationMax)
	, is_loop_finish_(false)
{
	// アニメーション再生時にモデルの基準座標が勝手に移動してしまう(ルートモーションの暴走)のを防ぐため、
	// "root"ボーンのローカル行列を初期状態に完全固定する
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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ブレンド率の更新、再生時間の進行、DxLib側へのアニメーション適用
 */
void ModelAnimation::Update()
{
	// 状態遷移時にモーションが瞬間的に切り替わりカクつくのを防ぐため、0.1(10フレーム)掛けて滑らかにブレンドする
	if (anim_blend_rate_ < 1.0f)
	{
		anim_blend_rate_ += 0.1f;
		if (anim_blend_rate_ > 1.0f)
		{
			anim_blend_rate_ = 1.0f;
		}
	}

	float fAnimTotaltime = 0.0f;

	if (animation_index_ != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);
		animation_time_ += animation_count_;

		if (animation_time_ > fAnimTotaltime)
		{
			if (!is_loop_)
			{
				// 非ループ(単発)アニメーション終了時、自動的に待機状態などへシームレスに遷移させるための処理
				if (loop_finish_state_ == kAnimationMax)
				{
					is_loop_finish_ = true;
					return;
				}
				ChangeAnimation(loop_finish_state_);

				// 終了から次状態への遷移時は、不自然な逆再生ブレンドが起きないよう即座に切り替える
				SetAnimationBlend(false);
				fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);
			}
			animation_time_ = 0.0f;
		}

		MV1SetAttachAnimTime(model_handle_, animation_index_, animation_time_);
		MV1SetAttachAnimBlendRate(model_handle_, animation_index_, anim_blend_rate_);
	}

	if (old_animation_index_ != -1)
	{
		fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, old_animation_index_);

		if (old_animation_time_ > fAnimTotaltime)
		{
			old_animation_time_ = 0.0f;
		}

		// 新旧2つのアニメーションの合成比率を常に合計100%に保ち、モデルが縮んだり破綻したりするのを防ぐ
		MV1SetAttachAnimBlendRate(model_handle_, old_animation_index_, 1.0f - anim_blend_rate_);
	}
}

/*
 * 入力: state (遷移先のアニメーション状態), index (未使用)
 * 出力: なし
 * 副作用: 古いアニメーションの破棄と、新規アニメーションのアタッチ
 */
void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 既に同じ状態だった場合、再生位置が0にリセットされてモーションが初期化されるバグを防ぐ
	if (state_ == state)
	{
		return;
	}

	state_ = state;
	is_loop_ = true;
	loop_finish_state_ = AnimationState::kAnimationMax;
	is_loop_finish_ = false;

	// DxLibのアタッチ上限(VRAM圧迫やブレンド計算破綻)を防ぐため、2世代前のアニメーションは完全に破棄する
	if (old_animation_index_ != -1)
	{
		MV1DetachAnim(model_handle_, old_animation_index_);
		old_animation_index_ = -1;
	}

	old_animation_index_ = animation_index_;
	old_animation_time_ = animation_time_;

	animation_index_ = MV1AttachAnim(model_handle_, (int)state);
	animation_time_ = 0.0f;
}

/*
 * 入力: isblend (ブレンドを有効にするか)
 * 出力: なし
 * 副作用: ブレンド率の強制上書きおよび旧アニメーションのデタッチ
 */
void ModelAnimation::SetAnimationBlend(bool isblend)
{
	if (isblend)
	{
		anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);
	}
	else
	{
		// ダメージ時や死亡時など、モーションの滑らかさよりも即時性を優先すべき演出のためにブレンドを完全カットする
		anim_blend_rate_ = 1.0f;

		if (old_animation_index_ != -1)
		{
			MV1DetachAnim(model_handle_, old_animation_index_);
			old_animation_index_ = -1;
		}
	}
}
