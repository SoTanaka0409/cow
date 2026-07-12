#include "DebugCamera.h"
#include <cmath>
#include "Master.h"
#include "Camera.h"
#include "InputManager.h"

DebugCamera::DebugCamera()
	: horizontal_angle_(0.0f)
	, vertical_angle_(-30.0f)
	, position_(VGet(0.0f, 1000.0f, 0.0f))
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Initialize()
{
	// �E�J�E��E��E��E��E�̏��E��E��E�ʒu�E��E�Q�E�[�E��E��E��E��E�J�E��E��E��E��E�̈ʒu�E��E��E��E��E��E��E��E�p�E��E��E�A�E�X�E��E��E�[�E�Y�E�ɐ؂�ւ��E�ł��E��E�悤�E�ɂ��E��E�
	if (Master::camera_ != nullptr)
	{
		position_ = Master::camera_->GetPosition();
	}
	else
	{
		position_ = VGet(0.0f, 1000.0f, 0.0f);
	}
	horizontal_angle_ = 0.0f;
	vertical_angle_ = -30.0f;
}

void DebugCamera::Update()
{
	float speed = 20.0f;
	// LSHIFT�E�L�E�[�E��E��E��E��E��E��E��E�Ă��E��E�Ԃ́A�E�f�E�o�E�b�E�O�E�J�E��E��E��E��E�̈ړ��E��E��E�x�E��E��E��E��E��E��E��E��E��E��E�
	if (CheckHitKey(KEY_INPUT_LSHIFT)) speed = 100.0f;

	VECTOR moveVec = VGet(0, 0, 0);
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);   // �E�J�E��E��E��E��E�̏��E��E��E��E�i�E�O�E�i�E�j�E�ړ��E�x�E�N�E�g�E��E�
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f); // �E�J�E��E��E��E��E�̍��E��E��E��E��E�ړ��E�x�E�N�E�g�E��E�
	
	UpdateRotate();
	
	float dist = 1000.0f;
	VECTOR target;
	target.x = position_.x + dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * sinf(horizontal_angle_ * DX_PI_F / 180.0f);
	target.y = position_.y + dist * sinf(-vertical_angle_ * DX_PI_F / 180.0f);
	target.z = position_.z - dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * cosf(horizontal_angle_ * DX_PI_F / 180.0f);

	{
		// �E�J�E��E��E��E��E�̎��E��E��E�x�E�N�E�g�E��E��E��E��E��E�O�E�i�E�ړ��E��E��E��E��E��E��E��E�߂�
		UpMoveVector = VSub(target, position_);

		// �E�J�E��E��E��E��E�̑O�E�i�E�x�E�N�E�g�E��E��E��E�Y�E��E��E�̏�x�E�N�E�g�E��E��E�Ƃ̊O�E�ς�Ƃ�A�E�^�E��E��E��E��E��E��E�̃x�E�N�E�g�E��E��E��E��E��E�߂�
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
	position_ = VAdd(position_, moveVec);

	// �E�ŐV�E�̍��E�W�E��E��E�ƂɃJ�E��E��E��E��E�̒��E��E��E�_�E��E�Čv�E�Z�E��E��E��E�
	target.x = position_.x + dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * sinf(horizontal_angle_ * DX_PI_F / 180.0f);
	target.y = position_.y + dist * sinf(-vertical_angle_ * DX_PI_F / 180.0f);
	target.z = position_.z - dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * cosf(horizontal_angle_ * DX_PI_F / 180.0f);

	// DX�E��E��E�C�E�u�E��E��E��E��E�̃J�E��E��E��E��E�ɁA�E�Čv�E�Z�E��E��E�ꂽ�E�f�E�o�E�b�E�O�E�p�E�J�E��E��E��E��E�̈ʒu�E�ƒ��E��E��E�_�E�𔽉f�E��E��E��E��E��E�
	SetCameraPositionAndTarget_UpVecY(position_, target);
}

void DebugCamera::UpdateRotate()
{
	// �E�p�E�x�E�l�E��E��E�͈͊O�E�i�E�}180�E�x�E�j�E�ɃI�E�[�E�o�E�[�E�t�E��E��E�[�E��E��E�Ȃ��E�悤�E�ɁE���E�b�E�v�E��E��E��E��E��E�s�E��E�
	if (horizontal_angle_ >= 180.0f)
	{
		horizontal_angle_ -= 360.0f;
	}
	if (horizontal_angle_ <= -180.0f)
	{
		horizontal_angle_ += 360.0f;
	}

	if (vertical_angle_ >= 80.0f)
	{
		vertical_angle_ = 80.0f;
	}
	if (vertical_angle_ <= -80.0f)
	{
		vertical_angle_ = -80.0f;
	}

	const float MOUSE_SENSITIVITY = 0.05f;

	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TYPE::kScene3D || Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TYPE::kSceneTutorial)
	{
		int mouse_x_, mouse_y_;
		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;
		int center_y_ = 360;

		// �E��E��E��E��E��E��E��E��E�s�E��E��E�₷�E��E��E��E��E�邽�E�߁A�E�}�E�E�E�X�E�J�E�[�E�\�E��E��E��E��E�ʒ��E�S�E�ɖ��E��E��E��E��E��E�߂��E�i0�E�L�E�[�E�ŉ��E��E�\�E�j
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(center_x_, center_y_);
		}

		int deltaX = mouse_x_ - center_x_;
		int deltaY = mouse_y_ - center_y_;

		horizontal_angle_ -= deltaX * MOUSE_SENSITIVITY;
		vertical_angle_ += deltaY * MOUSE_SENSITIVITY;
	}
}
