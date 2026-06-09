#pragma once
#include "InputManager.h"

// 3Dモデルのアニメーション状態を表す列挙型
enum AnimationState
{
	ANIMATION_NEUTRAL = 0,    // 待機状態
	ANIMATION_RUN,            // 走り状態
	ANIMATION_ATTACKJUMP,     // 空中攻撃状態
	ANIMATION_JUMP_IN,        // ジャンプ開始状態
	ANIMATION_JUMP_LOOP,      // ジャンプ滞空状態
	ANIMATION_JUMP_OUT,       // 着地状態
	ANIMATION_ATTACK,         // 攻撃状態
	ANIMATION_ATTACKSLIDE,    // スライディング攻撃状態
	ANIMATION_SLIDE,          // スライディング状態
	ANIMATION_ATTACKMAGIC,    // 魔法攻撃状態

	// 各キャラクター固有または新規追加のアニメーション状態
	ANIMATION_WALKING,        // 歩き状態
	ANIMATION_DYING,          // 死亡状態
	ANIMATION_DROP_KICK,      // ドロップキック状態

	ANIMATION_MAX             // 列挙型の最大数定義用
};

// アニメーション状態とDXライブラリの登録ハンドルのマッピング情報構造体
struct AnimationInfo
{
	AnimationState mState;    // アニメーション状態
	int mnAnimationHandle;    // アタッチされているアニメーションのハンドル
};
