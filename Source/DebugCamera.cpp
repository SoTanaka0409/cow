#include "DebugCamera.h"
#include <cmath>
#include "Master.h"
#include "Camera.h"
#include "InputManager.h"

DebugCamera::DebugCamera()
	: mfHorizontalAngle(0.0f)
	, mfVerticalAngle(-30.0f)
	, mvPosition(VGet(0.0f, 1000.0f, 0.0f))
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Initialize()
{
	// �J�����̏����ʒu��Q�[�����J�����̈ʒu��������p���A�X���[�Y�ɐ؂�ւ��ł���悤�ɂ���
	if (Master::mpCamera != nullptr)
	{
		mvPosition = Master::mpCamera->GetPosition();
	}
	else
	{
		mvPosition = VGet(0.0f, 1000.0f, 0.0f);
	}
	mfHorizontalAngle = 0.0f;
	mfVerticalAngle = -30.0f;
}

void DebugCamera::Update()
{
	float speed = 20.0f;
	// LSHIFT�L�[��������Ă���Ԃ́A�f�o�b�O�J�����̈ړ����x�����������
	if (CheckHitKey(KEY_INPUT_LSHIFT)) speed = 100.0f;

	VECTOR moveVec = VGet(0, 0, 0);
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);   // �J�����̏�����i�O�i�j�ړ��x�N�g��
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f); // �J�����̍������ړ��x�N�g��
	
	UpdateRotate();
	
	float dist = 1000.0f;
	VECTOR target;
	target.x = mvPosition.x + dist * cosf(mfVerticalAngle * DX_PI_F / 180.0f) * sinf(mfHorizontalAngle * DX_PI_F / 180.0f);
	target.y = mvPosition.y + dist * sinf(-mfVerticalAngle * DX_PI_F / 180.0f);
	target.z = mvPosition.z - dist * cosf(mfVerticalAngle * DX_PI_F / 180.0f) * cosf(mfHorizontalAngle * DX_PI_F / 180.0f);

	{
		// �J�����̎����x�N�g������O�i�ړ���������߂�
		UpMoveVector = VSub(target, mvPosition);

		// �J�����̑O�i�x�N�g����Y���̏�x�N�g���Ƃ̊O�ς�Ƃ�A�^�������̃x�N�g������߂�
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));

		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}
	
	if (CheckHitKey(KEY_INPUT_A))
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
	}

	if (VSquareSize(moveVec) > 0.0f)
	{
		moveVec = VScale(VNorm(moveVec), speed);
	}
	mvPosition = VAdd(mvPosition, moveVec);

	// �ŐV�̍��W���ƂɃJ�����̒����_��Čv�Z����
	target.x = mvPosition.x + dist * cosf(mfVerticalAngle * DX_PI_F / 180.0f) * sinf(mfHorizontalAngle * DX_PI_F / 180.0f);
	target.y = mvPosition.y + dist * sinf(-mfVerticalAngle * DX_PI_F / 180.0f);
	target.z = mvPosition.z - dist * cosf(mfVerticalAngle * DX_PI_F / 180.0f) * cosf(mfHorizontalAngle * DX_PI_F / 180.0f);

	// DX���C�u�����̃J�����ɁA�Čv�Z���ꂽ�f�o�b�O�p�J�����̈ʒu�ƒ����_�𔽉f������
	SetCameraPositionAndTarget_UpVecY(mvPosition, target);
}

void DebugCamera::UpdateRotate()
{
	// �p�x�l���͈͊O�i�}180�x�j�ɃI�[�o�[�t���[���Ȃ��悤�Ƀ��b�v������s��
	if (mfHorizontalAngle >= 180.0f)
	{
		mfHorizontalAngle -= 360.0f;
	}
	if (mfHorizontalAngle <= -180.0f)
	{
		mfHorizontalAngle += 360.0f;
	}

	if (mfVerticalAngle >= 80.0f)
	{
		mfVerticalAngle = 80.0f;
	}
	if (mfVerticalAngle <= -80.0f)
	{
		mfVerticalAngle = -80.0f;
	}

	const float MOUSE_SENSITIVITY = 0.05f;

	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TYPE::SCENE_3D || Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TYPE::SCENE_TUTORIAL)
	{
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);

		int centerX = 640;
		int centerY = 360;

		// ���������s���₷�����邽�߁A�}�E�X�J�[�\�����ʒ��S�ɖ�������߂��i0�L�[�ŉ���\�j
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(centerX, centerY);
		}

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		mfHorizontalAngle -= deltaX * MOUSE_SENSITIVITY;
		mfVerticalAngle += deltaY * MOUSE_SENSITIVITY;
	}
}

