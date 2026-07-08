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
	// ���[�h�ς݂�Effekseer�G�t�F�N�g�A�Z�b�g���\�[�X�����������������
	DeleteEffekseerEffect(effectResourcehandle);
}

void EffekseerEffect::Load()
{
	// �w�肳�ꂽ�p�X����.efk�G�t�F�N�g�f�[�^���������Ƀ��[�h����
	effectResourcehandle = LoadEffekseerEffect(filePath, effectSize);
}

void EffekseerEffect::Update()
{
	if (playingEffectHandle != -1)
	{
		// �Đ����̃G�t�F�N�g�ʒu�A��]�p�x�A�X�P�[���l��Effekseer���C���X�^���X�֖��t���[����������
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
	// �ǂݍ��񂾃G�t�F�N�g���\�[�X���ƂɁA3D��ԂōĐ���J�n����
	playingEffectHandle = PlayEffekseer3DEffect(effectResourcehandle);
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, PlayPos.x, PlayPos.y, PlayPos.z);
}

void EffekseerEffect::Stop()
{
	// ���݂̃G�t�F�N�g�Đ���Ԃ�擾���A�Đ����̏ꍇ�̂݋�����~�������
	int NowPlayEffect = IsEffekseer3DEffectPlaying(playingEffectHandle);
	if (NowPlayEffect != -1)
	{
		StopEffekseer3DEffect(playingEffectHandle);
	}
}

