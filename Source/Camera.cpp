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
	
	// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌク・ｽ・ｽ・ｽb・ｽs・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ・ｽi・ｽ`・ｽ・ｽﾂ能・ｽﾍ囲）・ｽ・ｽ・ｽ100?・ｽ・ｽ50000・ｽﾌ広・ｽﾍ囲に設抵ｿｽ
	SetCameraNearFar(100.0f, 50000.0f);

	// ・ｽ`・ｽ謔ｳ・ｽ・ｽﾈゑｿｽ・ｽw・ｽi・ｽ・ｽ・ｽ・ｽ・ｽ・ｽN・ｽ・ｽ・ｽA・ｽ・ｽ・ｽ・ｽﾛのデ・ｽt・ｽH・ｽ・ｽ・ｽg・ｽF・ｽ・ｽD・ｽF・ｽﾉ設抵ｿｽ
	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);
	Update();
}

void Camera::Update()
{
	// ・ｽX・ｽL・ｽ・ｽ・ｽJ・ｽ[・ｽh・ｽI・ｽ・A・ｽﾜゑｿｽ・ｽﾍデ・ｽo・ｽb・ｽO・ｽﾌ趣ｿｽ・ｽR・ｽﾚ難ｿｽ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・・ｿｽﾍゲ・ｽ[・ｽ・ｽ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ更・ｽV・ｽ・ｽX・ｽL・ｽb・ｽv
	if (Master::SelectSkill) return;
	if (Master::mbIsDebugCamera) return;

	if (target_ == nullptr)
	{
		target_ = ServiceLocator::GetPlayer();
	}
	
	UpdateRotate();
	
	if (target_ != nullptr)
	{
		// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ抵ｿｽ・ｽ・ｽ・ｽ_・ｽ・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽL・ｽ・ｽ・ｽ・ｽ・ｽN・ｽ^・ｽ[・ｽﾌ抵ｿｽ・ｽS・ｽ・ｽ・ｽ・ｽﾉ設定す・ｽ・ｽ
		look_at_position_ = target_->GetPosition();
		look_at_position_.y += 340.0f;
	}
	
	Shake();

	{
		const float distance = 1000.0f;
		VECTOR temp;
		// ・ｽ・ｽ・ｽ・ｽ・ｽE・ｽ・ｽ・ｽ・ｽ・ｽp・ｽx・ｽl・ｽi・ｽx・ｽ・ｽ・ｽ@・ｽj・ｽ・ｽ・ｽ・ｽW・ｽA・ｽ・ｽ・ｽﾉ変奇ｿｽ・ｽ・ｽ・ｽﾄカ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ3D・ｽ・ｽ・ｽW・ｽI・ｽt・ｽZ・ｽb・ｽg・ｽ・ｽv・ｽZ
		temp.x = distance * cosf(vertical_angle_ / 180.0f * 3.14159265f) * sinf(horizontal_angle_ / 180.0f * DX_PI_F);
		temp.y = distance * sinf(-vertical_angle_ / 180.0f * 3.14159265f);
		temp.z = -(distance * cosf(vertical_angle_ / 180.0f * DX_PI_F) * cosf(horizontal_angle_ / 180.0f * DX_PI_F));
		
		if (!is_phase_camera_active_)
		{
			position_ = VAdd(temp, look_at_position_);
			
			// ・ｽZ・ｽo・ｽ・ｽ・ｽ・ｽ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ・ｽﾑ抵ｿｽ・ｽ・ｽ・ｽ_・ｽﾉ、・ｽ・ｽﾊ振・ｽ・ｽ・ｽﾉゑｿｽ・ｽI・ｽt・ｽZ・ｽb・ｽg・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ・ｽZ・ｽ・ｽ・ｽﾄ費ｿｽ・ｽf
			SetCameraPositionAndTarget_UpVecY(VAdd(position_, shake_position_), VAdd(look_at_position_, shake_position_));
		}
	}

	prev_mouse_x_ = current_mouse_x_;
	prev_mouse_y_ = current_mouse_y_;
	GetMousePoint(&current_mouse_x_, &current_mouse_y_);

	// ・ｽ`・ｽ・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ位置・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ3D・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽp・ｽﾆ搾ｿｽ・ｽ・ｽ・ｽ謔､・ｽAEffekseer・ｽ・ｽ・ｽ・ｽ3D・ｽ・ｽﾔ設抵ｿｽﾆ難ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	Effekseer_Sync3DSetting();

	// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ迺搾ｿｽ・ｽ・ｽ_・ｽﾖ鯉ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽx・ｽN・ｽg・ｽ・ｽ・ｽｽ行・ｽ・ｽ・ｽ・ｽ・ｽﾌ鯉ｿｽ・ｽ・ｽ・ｽﾆゑｿｽ・ｽﾄ設定し・ｽA・ｽ・ｽ・ｽ・ｽ・ｽ\・ｽ・ｽ・ｽ・ｽs・ｽ・ｽ
	VECTOR lightDir = VSub(look_at_position_, position_);
	SetLightDirection(lightDir);
}

void Camera::UpdateRotate()
{
	// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ]・ｽp・ｽ・ｽ・ｽ・ｽﾑ撰ｿｽ・ｽ・ｽ・ｽ・ｽ]・ｽp・ｽﾌオ・ｽ[・ｽo・ｽ[・ｽt・ｽ・ｽ・ｽ[・ｽﾛ鯉ｿｽﾆ範囲撰ｿｽ・ｽ・ｽ
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
		// ・ｽX・ｽL・ｽ・ｽ・ｽI・ｽ・ﾅなゑｿｽ・ｽ鼾・ｿｽﾍゲ・ｽ[・ｽ・ｽ・ｽv・ｽ・ｽ・ｽC・ｽp・ｽﾌマ・ｽE・ｽX・ｽL・ｽ・ｽ・ｽv・ｽ`・ｽ・ｽ・ｽ・ｽs・ｽ・ｽ
		SetMouseDispFlag(false);
		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;
		int center_y_ = 200;

		// 0・ｽL・ｽ[・ｽﾅマ・ｽE・ｽX・ｽﾌ拘・ｽ・ｽ・ｽ・ｽf・ｽo・ｽb・ｽO・ｽﾚ的・ｽﾅ一時・ｽ・ｽ・ｽ・ｽﾅゑｿｽ・ｽ・ｽ謔､・ｽﾉゑｿｽ・ｽ・ｽ
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(center_x_, center_y_);
		}

		int deltaX = mouse_x_ - center_x_;

		if (!is_phase_camera_active_)
		{
			// ・ｽ・ｽ・ｽE・ｽﾌマ・ｽE・ｽX・ｽﾚ難ｿｽ・ｽﾊゑｿｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾌ撰ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽp・ｽi・ｽ・ｽ・ｽ[・ｽp・ｽj・ｽﾉ蓄・ｽﾏ費ｿｽ・ｽf
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
		// ・ｽ・ｽ・ｽ・ｽ・ｽg(sinf)・ｽﾆ趣ｿｽ・ｽﾔ経・ｽﾟにゑｿｽ・ｽt・ｽF・ｽ[・ｽh・ｽA・ｽE・ｽg・ｽ{・ｽ・ｽ・ｽ・ｽ・ｽZ・ｽ・ｽ・ｽﾄカ・ｽ・ｽ・ｽ・ｽ・ｽﾌ揺・ｽ轤ｵ・ｽﾊゑｿｽ・ｽ・ｽﾟゑｿｽ
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

	if (phase == (int)GameManager::GamePhase::Normal)
	{
		is_phase_camera_active_ = false;
		return;
	}

	// ・ｽ・ｽ・ｽo・ｽﾌ開・ｽn・ｽ・ｽ・ｽ・ｽ3・ｽb・ｽi180・ｽt・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽj・ｽ・ｽ・ｽo・ｽﾟゑｿｽ・ｽ・ｽ・ｽ・ｽA・ｽ・ｽ・ｽ・ｽ・ｽI・ｽﾉ標・ｽ・ｽ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽﾖと戻ゑｿｽ
	if (phaseTimer > 180)
	{
		is_phase_camera_active_ = false;
		return;
	}
	
	is_phase_camera_active_ = true;

	if (phase == (int)GameManager::GamePhase::MassSpawn)
	{
		// ・ｽ・ｽ・ｽ・ｽﾊ費ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽF・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾂつ、・ｽ・ｽ・ｽ・ｽ・ｽ繧ｰ・ｽ・ｽi・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽs・ｽb・ｽ`・ｽp・ｽ・ｽ・ｽ・ｽﾉ鯉ｿｽ・ｽ・ｽ・ｽ・ｽj
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
		// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽF・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽx・ｽ・ｽ繧ｰ・ｽﾄ全・ｽﾌゑｿｽ・ｽ・ｽn・ｽ・ｽ・ｽA・ｽ・ｽ・ｽ・ｽ・ｽ_・ｽｳ奇ｿｽ・ｽﾉゑｿｽ・ｽ・ｽ
		targetPos = VAdd(ufoPos, VGet(0.0f, 500.0f, -200.0f));
		VECTOR toTornado = VSub(tornadoPos, ufoPos);
		targetLookAt = VAdd(ufoPos, toTornado);
	}

	// ・ｽ・ｽ・ｽﾝのカ・ｽ・ｽ・ｽ・ｽ・ｽp・ｽ・ｽ・ｽ・ｽ・ｽ[・ｽ^・ｽ・ｽ・ｽ・ｽ`・ｽ・ｽﾔ（Lerp・ｽj・ｽ・ｽp・ｽ・ｽ・ｽﾄ目標・ｽl・ｽﾖス・ｽ・ｽ・ｽ[・ｽY・ｽﾉ遷・ｽﾚゑｿｽ・ｽ・ｽ・ｽ・ｽ
	float lerpSpeed = 0.1f; 
	position_ = LerpVector(position_, targetPos, lerpSpeed);
	look_at_position_ = LerpVector(look_at_position_, targetLookAt, lerpSpeed);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);
}