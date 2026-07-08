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
	// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽﾊ置・ｽ・ｽQ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ位置・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽp・ｽ・ｽ・ｽA・ｽX・ｽ・ｽ・ｽ[・ｽY・ｽﾉ切ゑｿｽﾖゑｿｽ・ｽﾅゑｿｽ・ｽ・ｽ謔､・ｽﾉゑｿｽ・ｽ・ｽ
	if (Master::mpCamera != nullptr)
	{
		position_ = Master::mpCamera->GetPosition();
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
	// LSHIFT・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾄゑｿｽ・ｽ・ｽﾔは、・ｽf・ｽo・ｽb・ｽO・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ移難ｿｽ・ｽ・ｽ・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	if (CheckHitKey(KEY_INPUT_LSHIFT)) speed = 100.0f;

	VECTOR moveVec = VGet(0, 0, 0);
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);   // ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽi・ｽO・ｽi・ｽj・ｽﾚ難ｿｽ・ｽx・ｽN・ｽg・ｽ・ｽ
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f); // ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ搾ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽﾚ難ｿｽ・ｽx・ｽN・ｽg・ｽ・ｽ
	
	UpdateRotate();
	
	float dist = 1000.0f;
	VECTOR target;
	target.x = position_.x + dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * sinf(horizontal_angle_ * DX_PI_F / 180.0f);
	target.y = position_.y + dist * sinf(-vertical_angle_ * DX_PI_F / 180.0f);
	target.z = position_.z - dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * cosf(horizontal_angle_ * DX_PI_F / 180.0f);

	{
		// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ趣ｿｽ・ｽ・ｽ・ｽx・ｽN・ｽg・ｽ・ｽ・ｽ・ｽ・ｽ・ｽO・ｽi・ｽﾚ難ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾟゑｿｽ
		UpMoveVector = VSub(target, position_);

		// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ前・ｽi・ｽx・ｽN・ｽg・ｽ・ｽ・ｽ・ｽY・ｽ・ｽ・ｽﾌ擾ｿｽx・ｽN・ｽg・ｽ・ｽ・ｽﾆの外・ｽﾏゑｿｽﾆゑｿｽA・ｽ^・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾌベ・ｽN・ｽg・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾟゑｿｽ
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

	// ・ｽﾅ新・ｽﾌ搾ｿｽ・ｽW・ｽ・ｽ・ｽﾆにカ・ｽ・ｽ・ｽ・ｽ・ｽﾌ抵ｿｽ・ｽ・ｽ・ｽ_・ｽ・ｽﾄ計・ｽZ・ｽ・ｽ・ｽ・ｽ
	target.x = position_.x + dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * sinf(horizontal_angle_ * DX_PI_F / 180.0f);
	target.y = position_.y + dist * sinf(-vertical_angle_ * DX_PI_F / 180.0f);
	target.z = position_.z - dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * cosf(horizontal_angle_ * DX_PI_F / 180.0f);

	// DX・ｽ・ｽ・ｽC・ｽu・ｽ・ｽ・ｽ・ｽ・ｽﾌカ・ｽ・ｽ・ｽ・ｽ・ｽﾉ、・ｽﾄ計・ｽZ・ｽ・ｽ・ｽ黷ｽ・ｽf・ｽo・ｽb・ｽO・ｽp・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ位置・ｽﾆ抵ｿｽ・ｽ・ｽ・ｽ_・ｽｽ映・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	SetCameraPositionAndTarget_UpVecY(position_, target);
}

void DebugCamera::UpdateRotate()
{
	// ・ｽp・ｽx・ｽl・ｽ・ｽ・ｽﾍ囲外・ｽi・ｽ}180・ｽx・ｽj・ｽﾉオ・ｽ[・ｽo・ｽ[・ｽt・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽﾈゑｿｽ・ｽ謔､・ｽﾉ・ｿｽ・ｽb・ｽv・ｽ・ｽ・ｽ・ｽ・ｽ・ｽs・ｽ・ｽ
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

	if (Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TYPE::SCENE_3D || Master::mpSceneManager->GetSceneType() == SceneManager::SCENE_TYPE::SCENE_TUTORIAL)
	{
		int mouse_x_, mouse_y_;
		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;
		int center_y_ = 360;

		// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽs・ｽ・ｽ・ｽ竄ｷ・ｽ・ｽ・ｽ・ｽ・ｽ驍ｽ・ｽﾟ、・ｽ}・ｽE・ｽX・ｽJ・ｽ[・ｽ\・ｽ・ｽ・ｽ・ｽ・ｽﾊ抵ｿｽ・ｽS・ｽﾉ厄ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾟゑｿｽ・ｽi0・ｽL・ｽ[・ｽﾅ会ｿｽ・ｽ・ｽﾂ能・ｽj
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
