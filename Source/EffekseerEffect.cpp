#include "EffekseerEffect.h"
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
	// ・ｽ・ｽ・ｽ[・ｽh・ｽﾏみゑｿｽEffekseer・ｽG・ｽt・ｽF・ｽN・ｽg・ｽA・ｽZ・ｽb・ｽg・ｽ・ｽ・ｽ\・ｽ[・ｽX・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	DeleteEffekseerEffect(effect_resource_handle_);
}

void EffekseerEffect::Load()
{
	// ・ｽw・ｽ閧ｳ・ｽ黷ｽ・ｽp・ｽX・ｽ・ｽ・ｽ・ｽ.efk・ｽG・ｽt・ｽF・ｽN・ｽg・ｽf・ｽ[・ｽ^・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾉ・ｿｽ・ｽ[・ｽh・ｽ・ｽ・ｽ・ｽ
	effect_resource_handle_ = LoadEffekseerEffect(file_path_, effectSize);
}

void EffekseerEffect::Update()
{
	if (playingEffectHandle != -1)
	{
		// ・ｽﾄ撰ｿｽ・ｽ・ｽ・ｽﾌエ・ｽt・ｽF・ｽN・ｽg・ｽﾊ置・ｽA・ｽ・ｽ]・ｽp・ｽx・ｽA・ｽX・ｽP・ｽ[・ｽ・ｽ・ｽl・ｽ・ｽEffekseer・ｽ・ｽ・ｽC・ｽ・ｽ・ｽX・ｽ^・ｽ・ｽ・ｽX・ｽﾖ厄ｿｽ・ｽt・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
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
	// ・ｽﾇみ搾ｿｽ・ｽｾエ・ｽt・ｽF・ｽN・ｽg・ｽ・ｽ・ｽ\・ｽ[・ｽX・ｽ・ｽ・ｽﾆに、3D・ｽ・ｽﾔで再撰ｿｽ・ｽ・ｽJ・ｽn・ｽ・ｽ・ｽ・ｽ
	playingEffectHandle = PlayEffekseer3DEffect(effect_resource_handle_);
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, play_pos_.x, play_pos_.y, play_pos_.z);
}

void EffekseerEffect::Stop()
{
	// ・ｽ・ｽ・ｽﾝのエ・ｽt・ｽF・ｽN・ｽg・ｽﾄ撰ｿｽ・ｽ・ｽﾔゑｿｽ謫ｾ・ｽ・ｽ・ｽA・ｽﾄ撰ｿｽ・ｽ・ｽ・ｽﾌ場合・ｽﾌみ具ｿｽ・ｽ・ｽ・ｽ・ｽ~・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	int NowPlayEffect = IsEffekseer3DEffectPlaying(playingEffectHandle);
	if (NowPlayEffect != -1)
	{
		StopEffekseer3DEffect(playingEffectHandle);
	}
}