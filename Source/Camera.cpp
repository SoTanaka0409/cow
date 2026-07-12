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
	: horizontal_angle_(0.0f)
	, vertical_angle_(-55.0f)
	, is_phase_camera_active_(false)
	, position_(VGet(0.0f, 0.0f, 0.0f))
	, look_at_position_(VGet(0.0f, 0.0f, 0.0f))
	, target_(nullptr)
	, shake_time_(0)
	, shake_time_count_(0)
	, shake_angle_(0.0f)
	, shake_time_counter_(0.0f)
	, mfShakeTime(0.0f)
	, shake_width_(0.0f)
	, shake_angle_speed_(0.0f)
	, step_time_(0.0f)
	, shake_position_(VGet(0.0f, 0.0f, 0.0f))
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	target_ = nullptr;
	
	// �E�J�E��E��E��E��E�̃N�E��E��E�b�E�s�E��E��E�O�E��E��E��E��E�i�E�`�E��E�\�E�͈́j�E��E��E�100?�E��E�50000�E�̍L�E�͈͂ɐݒ�
	SetCameraNearFar(100.0f, 50000.0f);

	// �E�`�E�悳�E��E�Ȃ��E�w�E�i�E��E��E��E��E��E�N�E��E��E�A�E��E��E��E�ۂ̃f�E�t�E�H�E��E��E�g�E�F�E��E�D�E�F�E�ɐݒ�
	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);
	Update();
}

void Camera::Update()
{
	// �E�X�E�L�E��E��E�J�E�[�E�h�E�I�E��EA�E�܂��E�̓f�E�o�E�b�E�O�E�̎��E�R�E�ړ��E�J�E��E��E��E��E��E��E��E�E��̓Q�E�[�E��E��E�J�E��E��E��E��E�̍X�E�V�E��E�X�E�L�E�b�E�v
	if (Master::SelectSkill) return;
	if (Master::mbIsDebugCamera) return;

	if (target_ == nullptr)
	{
		target_ = ServiceLocator::GetPlayer();
	}
	
	UpdateRotate();
	
	if (target_ != nullptr)
	{
		// �E�J�E��E��E��E��E�̒��E��E��E�_�E��E�v�E��E��E�C�E��E��E�[�E�L�E��E��E��E��E�N�E�^�E�[�E�̒��E�S�E��E��E��E�ɐݒ肷�E��E�
		look_at_position_ = target_->GetPosition();
		look_at_position_.y += 340.0f;
	}
	
	Shake();

	{
		const float distance = 1000.0f;
		VECTOR temp;
		// �E��E��E��E��E�E�E��E��E��E��E�p�E�x�E�l�E�i�E�x�E��E��E�@�E�j�E��E��E��E�W�E�A�E��E��E�ɕϊ��E��E��E�ăJ�E��E��E��E��E��E�3D�E��E��E�W�E�I�E�t�E�Z�E�b�E�g�E��E�v�E�Z
		temp.x = distance * cosf(vertical_angle_ / 180.0f * 3.14159265f) * sinf(horizontal_angle_ / 180.0f * DX_PI_F);
		temp.y = distance * sinf(-vertical_angle_ / 180.0f * 3.14159265f);
		temp.z = -(distance * cosf(vertical_angle_ / 180.0f * DX_PI_F) * cosf(horizontal_angle_ / 180.0f * DX_PI_F));
		
		if (!is_phase_camera_active_)
		{
			position_ = VAdd(temp, look_at_position_);
			
			// �E�Z�E�o�E��E��E��E��E�J�E��E��E��E��E��E��E�W�E��E��E��E�ђ��E��E��E�_�E�ɁA�E��E�ʐU�E��E��E�ɂ��E�I�E�t�E�Z�E�b�E�g�E��E��E�W�E��E��E��E�Z�E��E��E�Ĕ��E�f
			SetCameraPositionAndTarget_UpVecY(VAdd(position_, shake_position_), VAdd(look_at_position_, shake_position_));
		}
	}

	prev_mouse_x_ = current_mouse_x_;
	prev_mouse_y_ = current_mouse_y_;
	GetMousePoint(&current_mouse_x_, &current_mouse_y_);

	// �E�`�E��E�G�E�t�E�F�E�N�E�g�E�̈ʒu�E��E��E��E��E��E��E�3D�E�J�E��E��E��E��E��E��E��E�p�E�ƍ��E��E��E�悤�E�AEffekseer�E��E��E��E�3D�E��E�Ԑݒ�Ɠ��E��E��E��E��E��E�
	Effekseer_Sync3DSetting();

	// �E�J�E��E��E��E��E��E��E�璍��E��E�_�E�֌��E��E��E��E��E�x�E�N�E�g�E��E��E�𕽍s�E��E��E��E��E�̌��E��E��E�Ƃ��E�Đݒ肵�E�A�E��E��E��E��E�\�E��E��E��E�s�E��E�
	VECTOR lightDir = VSub(look_at_position_, position_);
	SetLightDirection(lightDir);
}

void Camera::UpdateRotate()
{
	// �E�J�E��E��E��E��E��E��E��E��E��E�]�E�p�E��E��E��E�ѐ��E��E��E��E�]�E�p�E�̃I�E�[�E�o�E�[�E�t�E��E��E�[�E�ی�Ɣ͈͐��E��E�
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
		// �E�X�E�L�E��E��E�I�E��EłȂ��E�ꍁE��̓Q�E�[�E��E��E�v�E��E��E�C�E�p�E�̃}�E�E�E�X�E�L�E��E��E�v�E�`�E��E��E��E�s�E��E�
		SetMouseDispFlag(false);
		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;
		int center_y_ = 200;

		// 0�E�L�E�[�E�Ń}�E�E�E�X�E�̍S�E��E��E��E�f�E�o�E�b�E�O�E�ړI�E�ňꎞ�E��E��E��E�ł��E��E�悤�E�ɂ��E��E�
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(center_x_, center_y_);
		}

		int deltaX = mouse_x_ - center_x_;

		if (!is_phase_camera_active_)
		{
			// �E��E��E�E�E�̃}�E�E�E�X�E�ړ��E�ʂ�J�E��E��E��E��E�̐��E��E��E��E��E��E�p�E�i�E��E��E�[�E�p�E�j�E�ɒ~�E�ϔ��E�f
			horizontal_angle_ -= deltaX * MOUSE_SENSITIVITY;
		}
	}
}

bool Camera::IsMouseMoved()
{
	int moveX = abs(current_mouse_x_ - prev_mouse_x_);
	int moveY = abs(current_mouse_y_ - prev_mouse_y_);

	return moveX > 0.05f || moveY > 0.05f;
}

void Camera::Finalize()
{
}

void Camera::Shake()
{
	if (shake_time_counter_ < mfShakeTime)
	{
		// �E��E��E��E��E�g(sinf)�E�Ǝ��E�Ԍo�E�߂ɂ��E�t�E�F�E�[�E�h�E�A�E�E�E�g�E�{�E��E��E��E��E�Z�E��E��E�ăJ�E��E��E��E��E�̗h�E�炵�E�ʂ��E��E�߂�
		shake_position_.y = sinf(shake_angle_) * (1.0f - (shake_time_counter_ / mfShakeTime)) * shake_width_;
		shake_position_.x = 0.0f;
		shake_position_.z = 0.0f;

		shake_angle_ += shake_angle_speed_ * step_time_;
		shake_time_counter_ += step_time_;
	}
	else
	{
		shake_position_ = VGet(0.0f, 0.0f, 0.0f);
	}
}

void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	shake_time_counter_ = 0.0f;
	mfShakeTime = time;
	shake_width_ = width;
	shake_angle_speed_ = angleSpeed;
	step_time_ = stepTime;
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

	if (phase == (int)GameManager::GamePhase::kNormal)
	{
		is_phase_camera_active_ = false;
		return;
	}

	// �E��E��E�o�E�̊J�E�n�E��E��E��E�3�E�b�E�i180�E�t�E��E��E�[�E��E��E�j�E��E��E�o�E�߂��E��E��E��E�A�E��E��E��E��E�I�E�ɕW�E��E��E�J�E��E��E��E��E�ւƖ߂�
	if (phaseTimer > 180)
	{
		is_phase_camera_active_ = false;
		return;
	}
	
	is_phase_camera_active_ = true;

	if (phase == (int)GameManager::GamePhase::kMassSpawn)
	{
		// �E��E��E��E�ʔ��E��E��E��E��E�o�E�F�E�J�E��E��E��E��E��E��E��E��E��E��E��E��E��E�A�E��E��E��E��E�グ�E��E�i�E�J�E��E��E��E��E�s�E�b�E�`�E�p�E��E��E��E�Ɍ��E��E��E��E�j
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
	else if (phase == (int)GameManager::GamePhase::kTornadoCrisis)
	{
		// �E��E��E��E��E��E��E��E��E��E��E�o�E�F�E�J�E��E��E��E��E��E��E�x�E��E�グ�E�đS�E�̂��E��E�n�E��E��E�A�E��E��E��E��E�_�E�𗳊��E�ɂ��E��E�
		targetPos = VAdd(ufoPos, VGet(0.0f, 500.0f, -200.0f));
		VECTOR toTornado = VSub(tornadoPos, ufoPos);
		targetLookAt = VAdd(ufoPos, toTornado);
	}

	// �E��E��E�݂̃J�E��E��E��E��E�p�E��E��E��E��E�[�E�^�E��E��E��E�`�E��E�ԁiLerp�E�j�E��E�p�E��E��E�ĖڕW�E�l�E�փX�E��E��E�[�E�Y�E�ɑJ�E�ڂ��E��E��E��E�
	float lerpSpeed = 0.1f; 
	position_ = LerpVector(position_, targetPos, lerpSpeed);
	look_at_position_ = LerpVector(look_at_position_, targetLookAt, lerpSpeed);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);
}