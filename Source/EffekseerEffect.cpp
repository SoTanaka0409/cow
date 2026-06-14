#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include "Master.h"

EffekseerEffect::EffekseerEffect(const char* filename, VECTOR initPos, float EffectSize)
	: PlayPos(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, effectResourcehandle(-1)
	, filePath(filename)
	, effectSize(EffectSize)
	, PlayCount(0)
	, playingEffectHandle(-1)
	, LoopFlag(true)
	, mvScale(VGet(1.0f, 1.0f, 1.0f))
{
	Load();
}

EffekseerEffect::~EffekseerEffect()
{
	// ロード済みのEffekseerエフェクトアセットリソースをメモリから解放する
	DeleteEffekseerEffect(effectResourcehandle);
}

void EffekseerEffect::Load()
{
	// 指定されたパスから.efkエフェクトデータをメモリ上にロードする
	effectResourcehandle = LoadEffekseerEffect(filePath, effectSize);
}

void EffekseerEffect::Update()
{
	if (playingEffectHandle != -1)
	{
		// 再生中のエフェクト位置、回転角度、スケール値をEffekseer側インスタンスへ毎フレーム同期する
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, PlayPos.x, PlayPos.y, PlayPos.z);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, mvRotation.x, mvRotation.y, mvRotation.z);
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, mvScale.x, mvScale.y, mvScale.z);
	}
}

void EffekseerEffect::Draw()
{
}

void EffekseerEffect::Play()
{
	// 読み込んだエフェクトリソースをもとに、3D空間で再生を開始する
	playingEffectHandle = PlayEffekseer3DEffect(effectResourcehandle);
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, PlayPos.x, PlayPos.y, PlayPos.z);
}

void EffekseerEffect::Stop()
{
	// 現在のエフェクト再生状態を取得し、再生中の場合のみ強制停止をかける
	int NowPlayEffect = IsEffekseer3DEffectPlaying(playingEffectHandle);
	if (NowPlayEffect != -1)
	{
		StopEffekseer3DEffect(playingEffectHandle);
	}
}

