#pragma once
#include <string>
#include <vector>
#include "ModelUtility.h"

// 3Dモデル（MV1形式）のボーンアニメーション制御とブレンド処理を行うクラス
class ModelAnimation
{
public:
	/*
	 * @brief アニメーション制御対象のモデルハンドルを登録する
	 * [入力] modelHandle: DXライブラリのモデルハンドル
	 * [出力] なし
	 * [副作用] 各種内部状態変数の初期化、デフォルトアニメーションへの切り替え
	 */
	ModelAnimation(int modelHandle);
	~ModelAnimation();

	/*
	 * @brief アニメーションの時間を進行させ、切り替え時のブレンド補間を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] DXライブラリのモデルハンドルに対して現在のアニメーション状態・時刻を反映
	 */
	void Update();

	/*
	 * @brief 再生するアニメーションを切り替える
	 * [入力] state: 切り替え先のアニメーション状態, index: アニメーションのインデックス番号
	 * [出力] なし
	 * [副作用] 前のアニメーションとのブレンド処理が開始される
	 */
	void ChangeAnimation(AnimationState state, int index = 1);

	void SetLoop(bool isLoop) { mbLoop = isLoop; }
	void SetLoopFinishState(AnimationState state) { mnLoopFinishState = state; }
	void SetAnimationBlend(bool isBlend);

	AnimationState GetNowState() { return mnState; }
	bool IsLoopFinish() { return mbLoopFinish; }

	void SetAnimationCount(float count) { mfAnimationCount = count; }

private:
	int mnModelHandle;                  // DXライブラリの3Dモデルハンドル
	float mfAnimationTime;              // 現在再生中のアニメーション時刻（秒）
	float mfAnimationCount;             // 再生時間を進める速度係数
	int mnAnimationIndex;               // 現在のアニメーションのインデックス
	float mfOldAnimationTime;           // ブレンド移行前の古いアニメーションの時刻（秒）
	int mnOldAnimationIndex;            // 古いアニメーションのインデックス
	float mfAnimBlendRate;              // 新旧アニメーションのブレンド比率 (0.0?1.0)
	AnimationState mnState;             // 現在のアニメーション状態
	bool mbLoop;                        // ループ再生するかどうか
	AnimationState mnLoopFinishState;   // 非ループアニメーション終了後に遷移する状態
	bool mbLoopFinish;                  // アニメーションが1周完了したかどうか
};

