# -*- coding: utf-8 -*-
import os

effekseereffect_cpp = """#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include "Master.h"

EffekseerEffect::EffekseerEffect(const char* filename, VECTOR initPos, float kEffectSize)
	: play_pos_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, effect_resource_handle_(-1)
	, file_path_(filename)
	, effectSize(kEffectSize)
	, PlayCount(0)
	, playingEffectHandle(-1)
	, LoopFlag(true)
	, mvScale(VGet(1.0f, 1.0f, 1.0f))
{
	Load();
}

EffekseerEffect::~EffekseerEffect()
{
	// ロード済みのEffekseerエフェクトリソースを破棄する
	DeleteEffekseerEffect(effect_resource_handle_);
}

void EffekseerEffect::Load()
{
	// 指定されたパスからエフェクトデータをロードする
	effect_resource_handle_ = LoadEffekseerEffect(file_path_, effectSize);
}

void EffekseerEffect::Update()
{
	if (playingEffectHandle != -1)
	{
		// 再生中のエフェクト位置、回転角度、スケール値を更新する
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, play_pos_.x, play_pos_.y, play_pos_.z);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rotation_.x, rotation_.y, rotation_.z);
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, mvScale.x, mvScale.y, mvScale.z);
	}
}

void EffekseerEffect::Draw()
{
}

void EffekseerEffect::Play()
{
	// ロードしたリソースを元に再生を開始する
	playingEffectHandle = PlayEffekseer3DEffect(effect_resource_handle_);
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, play_pos_.x, play_pos_.y, play_pos_.z);
}

void EffekseerEffect::Stop()
{
	// 再生状態を取得し、再生中の場合のみ停止する
	int NowPlayEffect = IsEffekseer3DEffectPlaying(playingEffectHandle);
	if (NowPlayEffect != -1)
	{
		StopEffekseer3DEffect(playingEffectHandle);
	}
}
"""

with open('Source/EffekseerEffect.cpp', 'w', encoding='utf-8-sig') as f:
    f.write(effekseereffect_cpp)
