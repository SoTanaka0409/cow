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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: カメラパラメータの初期化
 */
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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 描画範囲と背景色の設定、初期座標の適用
 */
void Camera::Initialize()
{
	target_ = nullptr;

	// 遠景モデルの不自然な消失や、UI等近景のクリッピングを防ぐため描画範囲を広めに設定
	SetCameraNearFar(100.0f, 50000.0f);

	// スカイボックス未設定時に画面外に前フレームの残像が描画されるバグを防ぐため、背景をグレーアウト
	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);
	Update();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: カメラ座標・注視点・ライト方向の更新、エフェクトエンジンの同期
 */
void Camera::Update()
{
	// スキル選択UIの操作やデバッグ操作とカメラ移動が競合し、意図せず視点が飛ぶのを防ぐ
	if (Master::SelectSkill) return;
	if (Master::mbIsDebugCamera) return;

	if (target_ == nullptr)
	{
		target_ = ServiceLocator::GetPlayer();
	}

	UpdateRotate();

	if (target_ != nullptr)
	{
		// ターゲット（プレイヤー）の足元ではなく、上半身から頭部を中心に画面へ収めるためのオフセット
		look_at_position_ = target_->GetPosition();
		look_at_position_.y += 340.0f;
	}

	Shake();

	{
		const float distance = 1000.0f;
		VECTOR temp;
		// カメラがターゲットの周囲を常に一定距離で旋回できるよう、角度情報から球面座標を用いて相対位置を算出
		temp.x = distance * cosf(vertical_angle_ / 180.0f * 3.14159265f) * sinf(horizontal_angle_ / 180.0f * DX_PI_F);
		temp.y = distance * sinf(-vertical_angle_ / 180.0f * 3.14159265f);
		temp.z = -(distance * cosf(vertical_angle_ / 180.0f * DX_PI_F) * cosf(horizontal_angle_ / 180.0f * DX_PI_F));

		if (!is_phase_camera_active_)
		{
			position_ = VAdd(temp, look_at_position_);

			// 画面揺れ演出によってカメラのベース座標が永続的にズレてしまうバグを防ぐため、シェイク分は最後に加算
			SetCameraPositionAndTarget_UpVecY(VAdd(position_, shake_position_), VAdd(look_at_position_, shake_position_));
		}
	}

	prev_mouse_x_ = current_mouse_x_;
	prev_mouse_y_ = current_mouse_y_;
	GetMousePoint(&current_mouse_x_, &current_mouse_y_);

	// 3Dエフェクトが画面上で正しい位置・パースで描画されるよう、Effekseer側にカメラ行列を同期
	Effekseer_Sync3DSetting();

	// プレイヤーの視界（カメラの前方）が常に明るく照らされるよう、平行光源の向きをカメラに追従させる
	VECTOR lightDir = VSub(look_at_position_, position_);
	SetLightDirection(lightDir);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: マウス入力に基づく水平・垂直アングルの更新
 */
void Camera::UpdateRotate()
{
	// カメラの天地逆転（ジンバルロック）を防ぐため垂直角を制限し、水平角はシームレスにループさせる
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
		// 画面端への到達によるカメラ操作の突っ掛かりを防ぎ、無限に旋回できるようカーソルを非表示にして中央固定
		SetMouseDispFlag(false);
		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;
		int center_y_ = 200;

		// デバッグ時等に他ウィンドウへマウスを移動できるよう、0キー押下中のみ強制中央固定を解除する
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(center_x_, center_y_);
		}

		int deltaX = mouse_x_ - center_x_;

		if (!is_phase_camera_active_)
		{
			horizontal_angle_ -= deltaX * MOUSE_SENSITIVITY;
		}
	}
}

/*
 * 入力: なし
 * 出力: マウスが移動していればtrue
 * 副作用: なし
 */
bool Camera::IsMouseMoved()
{
	int moveX = abs(current_mouse_x_ - prev_mouse_x_);
	int moveY = abs(current_mouse_y_ - prev_mouse_y_);

	return moveX > 0.05f || moveY > 0.05f;
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
void Camera::Finalize()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 揺れによる位置オフセットの計算とタイマーの進行
 */
void Camera::Shake()
{
	if (shake_time_counter_ < mfShakeTime)
	{
		// ピタッと止まる不自然さを無くすため、時間経過(進行度合い)に応じて揺れの振幅を減衰させる
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

/*
 * 入力: time (継続時間), width (振幅), angleSpeed (角速度), stepTime (1フレームの進行時間)
 * 出力: なし
 * 副作用: 揺れに関する内部パラメータの初期化
 */
void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	shake_time_counter_ = 0.0f;
	mfShakeTime = time;
	shake_width_ = width;
	shake_angle_speed_ = angleSpeed;
	step_time_ = stepTime;
}

/*
 * 入力: phase (現在のフェーズ), ufoPos (UFOの座標), tornadoPos (竜巻の座標)
 * 出力: なし
 * 副作用: 特定のゲームフェーズに応じたカメラの座標・注視点の上書き
 */
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

	// 演出による操作不能状態が長引かないよう、3秒(180フレーム)経過で自動的に通常カメラへ復帰させる
	if (phaseTimer > 180)
	{
		is_phase_camera_active_ = false;
		return;
	}

	is_phase_camera_active_ = true;

	if (phase == (int)GameManager::GamePhase::kMassSpawn)
	{
		// 牛の大量発生フェーズ: UFOを見上げるアングルから徐々に水平に戻る動的演出
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
		// 竜巻接近フェーズ: UFOと竜巻の相対位置をプレイヤーに把握させるため、両者が画面に収まるようカメラを引く
		targetPos = VAdd(ufoPos, VGet(0.0f, 500.0f, -200.0f));
		VECTOR toTornado = VSub(tornadoPos, ufoPos);
		targetLookAt = VAdd(ufoPos, toTornado);
	}

	// 画面の突然の切り替わりによる3D酔いを防ぐため、現在地から目標地点まで線形補間(Lerp)で滑らかに移動させる
	float lerpSpeed = 0.1f;
	position_ = LerpVector(position_, targetPos, lerpSpeed);
	look_at_position_ = LerpVector(look_at_position_, targetLookAt, lerpSpeed);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);
}