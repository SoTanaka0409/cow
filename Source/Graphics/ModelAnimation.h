#pragma once
#include <string>
#include <vector>
#include "ModelUtility.h"

/// @brief MV1モデルのボーンアニメーション制御と、状態遷移時のモーション補間（ブレンド）をカプセル化し、モデルの描画と状態管理を分離するクラス
class ModelAnimation
{
public:
	/// @brief 生成後に外部でモデルが破棄された場合、アクセス違反でクラッシュするため、モデル本体とライフサイクルを厳密に同期させること
	/// @param modelHandle DXライブラリのモデルハンドル [出力] なし [副作用] 内部状態の初期化
	ModelAnimation(int modelHandle);
	~ModelAnimation();

	/// @brief 可変フレームレート環境下でのモーション進行速度のブレを防ぐため、内部でシステム経過時間（DeltaTime）を乗算して時間を進めること
	/// @details なし [出力] なし [副作用] DXライブラリ側のモデル姿勢の更新
	void Update();

	/// @brief 毎フレーム連続で呼び出すとブレンド率が初期化され続けモーションが固まるバグが発生するため、状態変化時のみ呼ぶようガードすること
	/// @param state 次の状態
	/// @param index アニメーション番号 [出力] なし [副作用] ブレンド処理の開始
	void ChangeAnimation(AnimationState state, int index = 1);

	void SetLoop(bool isLoop) { is_loop_ = isLoop; }
	void SetLoopFinishState(AnimationState state) { loop_finish_state_ = state; }
	void SetAnimationBlend(bool isBlend);

	AnimationState GetNowState() { return state_; }
	bool IsLoopFinish() { return is_loop_finish_; }

	void SetAnimationCount(float count) { animation_count_ = count; }

private:
	int model_handle_;                 ///< DXライブラリのモデルハンドル
	float animation_time_;             ///< 現在のアニメーション再生時間
	float animation_count_;            ///< アニメーションの再生速度倍率
	int animation_index_;              ///< 現在のアニメーション番号
	float old_animation_time_;         ///< ブレンド前の旧アニメーション再生時間
	int old_animation_index_;          ///< ブレンド前の旧アニメーション番号
	float anim_blend_rate_;            ///< モーション補間のブレンド率
	AnimationState state_;             ///< 現在のアニメーション状態
	bool is_loop_;                     ///< ループ再生するかどうかのフラグ
	AnimationState loop_finish_state_; ///< 非ループ再生終了後の遷移先状態
	bool is_loop_finish_;              ///< アニメーションが終了したかどうかのフラグ
};
