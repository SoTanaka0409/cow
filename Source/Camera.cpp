#include "ServiceLocator.h"
#include "Player3D.h"
#include "Camera.h"
#include <cmath>
#include "Master.h"
#include "ObjectManager.h"
#include "Scene3D.h"
#include "Object3D.h"
#include "Scene.h"
#include "GameManager.h"

Camera::Camera()
	: mfHorizontalAngle(0.0f)
	, mfVerticalAngle(-55.0f)
	, mbIsPhaseCameraActive(false)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, mpTarget(nullptr)
	, mnShakeTime(0)
	, mnShakeTimeCount(0)
	, mfShakeAngle(0.0f)
	, mfShakeTimeCounter(0.0f)
	, mfShakeTime(0.0f)
	, mfShakeWidth(0.0f)
	, mfShakeAngleSpeed(0.0f)
	, mfStepTime(0.0f)
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	mpTarget = nullptr;
	
	// �J�����̃N���b�s���O�����i�`��\�͈́j���100?��50000�̍L�͈͂ɐݒ�
	SetCameraNearFar(100.0f, 50000.0f);

	// �`�悳��Ȃ��w�i������N���A����ۂ̃f�t�H���g�F��D�F�ɐݒ�
	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);
	Update();
}

void Camera::Update()
{
	// �X�L���J�[�h�I�𒆁A�܂��̓f�o�b�O�̎��R�ړ��J�������쒆�̓Q�[���J�����̍X�V��X�L�b�v
	if (Master::SelectSkill) return;
	if (Master::mbIsDebugCamera) return;

	if (mpTarget == nullptr)
	{
		mpTarget = ServiceLocator::GetPlayer();
	}
	
	UpdateRotate();
	
	if (mpTarget != nullptr)
	{
		// �J�����̒����_��v���C���[�L�����N�^�[�̒��S����ɐݒ肷��
		mvLookAtPosition = mpTarget->GetPosition();
		mvLookAtPosition.y += 340.0f;
	}
	
	Shake();

	{
		const float distance = 1000.0f;
		VECTOR temp;
		// �����E�����p�x�l�i�x���@�j����W�A���ɕϊ����ăJ������3D���W�I�t�Z�b�g��v�Z
		temp.x = distance * cosf(mfVerticalAngle / 180.0f * 3.14159265f) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
		temp.y = distance * sinf(-mfVerticalAngle / 180.0f * 3.14159265f);
		temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
		
		if (!mbIsPhaseCameraActive)
		{
			mvPosition = VAdd(temp, mvLookAtPosition);
			
			// �Z�o�����J�������W����ђ����_�ɁA��ʐU���ɂ��I�t�Z�b�g���W����Z���Ĕ��f
			SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
		}
	}

	mPrevMouseX = mMouseX;
	mPrevMouseY = mMouseY;
	GetMousePoint(&mMouseX, &mMouseY);

	// �`��G�t�F�N�g�̈ʒu�������3D�J��������p�ƍ����悤�AEffekseer����3D��Ԑݒ�Ɠ�������
	Effekseer_Sync3DSetting();

	// �J�������璍���_�֌������x�N�g���𕽍s�����̌����Ƃ��Đݒ肵�A�����\����s��
	VECTOR lightDir = VSub(mvLookAtPosition, mvPosition);
	SetLightDirection(lightDir);
}

void Camera::UpdateRotate()
{
	// �J����������]�p����ѐ�����]�p�̃I�[�o�[�t���[�ی�Ɣ͈͐���
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
		// �X�L���I�𒆂łȂ��ꍇ�̓Q�[���v���C�p�̃}�E�X�L���v�`����s��
		SetMouseDispFlag(false);
		GetMousePoint(&mouseX, &mouseY);

		int centerX = 640;
		int centerY = 200;

		// 0�L�[�Ń}�E�X�̍S����f�o�b�O�ړI�ňꎞ����ł���悤�ɂ���
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(centerX, centerY);
		}

		int deltaX = mouseX - centerX;

		if (!mbIsPhaseCameraActive)
		{
			// ���E�̃}�E�X�ړ��ʂ�J�����̐�������p�i���[�p�j�ɒ~�ϔ��f
			mfHorizontalAngle -= deltaX * MOUSE_SENSITIVITY;
		}
	}
}

bool Camera::IsMouseMoved()
{
	int moveX = abs(mMouseX - mPrevMouseX);
	int moveY = abs(mMouseY - mPrevMouseY);

	return moveX > 0.05f || moveY > 0.05f;
}

void Camera::Finalize()
{
}

void Camera::Shake()
{
	if (mfShakeTimeCounter < mfShakeTime)
	{
		// �����g(sinf)�Ǝ��Ԍo�߂ɂ��t�F�[�h�A�E�g�{�����Z���ăJ�����̗h�炵�ʂ���߂�
		mvShakePosition.y = sinf(mfShakeAngle) * (1.0f - (mfShakeTimeCounter / mfShakeTime)) * mfShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		mfShakeAngle += mfShakeAngleSpeed * mfStepTime;
		mfShakeTimeCounter += mfStepTime;
	}
	else
	{
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	mfShakeTimeCounter = 0.0f;
	mfShakeTime = time;
	mfShakeWidth = width;
	mfShakeAngleSpeed = angleSpeed;
	mfStepTime = stepTime;
}

void Camera::UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos)
{
	if (Master::mbIsDebugCamera) return;

	static int lastPhase = -1;
	static int phaseTimer = 0;
	if (lastPhase != phase)
	{
		phaseTimer = 0;
		lastPhase = phase;
	}
	phaseTimer++;

	VECTOR targetPos;
	VECTOR targetLookAt;

	auto LerpVector = [](VECTOR start, VECTOR end, float t) -> VECTOR {
		return VAdd(start, VScale(VSub(end, start), t));
	};

	if (phase == (int)GameManager::GamePhase::Normal)
	{
		mbIsPhaseCameraActive = false;
		return;
	}

	// ���o�̊J�n����3�b�i180�t���[���j���o�߂�����A�����I�ɕW���J�����ւƖ߂�
	if (phaseTimer > 180)
	{
		mbIsPhaseCameraActive = false;
		return;
	}
	
	mbIsPhaseCameraActive = true;

	if (phase == (int)GameManager::GamePhase::MassSpawn)
	{
		// ����ʔ������o�F�J��������������A�����グ��i�J�����s�b�`�p����Ɍ�����j
		targetPos = VAdd(ufoPos, VGet(0.0f, 150.0f, -300.0f));
		
		if (phaseTimer < 180)
		{
			float lookUpAmount = 400.0f * (1.0f - (float)phaseTimer / 120.0f);
			targetLookAt = VAdd(ufoPos, VGet(0.0f, 100.0f + lookUpAmount, 0.0f));
		}
		else
		{
			targetLookAt = VAdd(ufoPos, VGet(0.0f, 100.0f, 0.0f));
		}
	}
	else if (phase == (int)GameManager::GamePhase::TornadoCrisis)
	{
		// �����������o�F�J�������x��グ�đS�̂���n���A�����_�𗳊��ɂ���
		targetPos = VAdd(ufoPos, VGet(0.0f, 500.0f, -200.0f));
		VECTOR toTornado = VSub(tornadoPos, ufoPos);
		targetLookAt = VAdd(ufoPos, toTornado);
	}

	// ���݂̃J�����p�����[�^����`��ԁiLerp�j��p���ĖڕW�l�փX���[�Y�ɑJ�ڂ�����
	float lerpSpeed = 0.1f; 
	mvPosition = LerpVector(mvPosition, targetPos, lerpSpeed);
	mvLookAtPosition = LerpVector(mvLookAtPosition, targetLookAt, lerpSpeed);

	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);
}
