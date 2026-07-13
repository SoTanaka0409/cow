#pragma once
#include "InputManager.h"

// 3Dモデルのアニメーション状態を表す列挙型
enum AnimationState
{
	kAnimationNeutral = 0,    // 待機状態
	kAnimationRun,            // 走り状態
	kAnimationAttackJump,     // 空中攻撃状態
	kAnimationJumpIn,         // ジャンプ開始状態
	kAnimationJumpLoop,       // ジャンプ滞空状態
	kAnimationJumpOut,        // 着地状態
	kAnimationAttack,         // 攻撃状態
	kAnimationAttackSlide,    // スライディング攻撃状態
	kAnimationSlide,          // スライディング状態
	kAnimationAttackMagic,    // 魔法攻撃状態

	// 各キャラクター固有または新規追加のアニメーション状態
	kAnimationWalking,        // 歩き状態
	kAnimationDying,          // 死亡状態
	kAnimationDropKick,       // ドロップキック状態

	kAnimationMax             // 型の最大数定義用
};

// アニメーション状態とDXライブラリの登録ハンドルのマッピング情報構造体
struct AnimationInfo
{
	AnimationState state;      // アニメーション状態
	int animation_handle;      // アタッチされているアニメーションのハンドル
};
