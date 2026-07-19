#include "ServiceLocator.h"

#include "Player3D.h"

#include "Camera.h"

#include <cmath>

#include "Master.h"

#include "ObjectManager.h"

#include "GameScene.h"

#include "Object3D.h"

#include "Scene.h"

#include "GameManager.h"

// 副作用：カメラパラメータおよびカメラシェイク制御変数の初期化

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

	, old_shake_time_(0.0f)

	, shake_width_(0.0f)

	, shake_angle_speed_(0.0f)

	, step_time_(0.0f)

	, shake_position_(VGet(0.0f, 0.0f, 0.0f))

{

}

Camera::~Camera()

{

}

// 副作用：DxLibのニア・ファークリップ、背景色の適用、初期位置の反映

void Camera::Initialize()

{

	target_ = nullptr;

	horizontal_angle_ = 0.0f;

	vertical_angle_ = -55.0f;

	is_phase_camera_active_ = false;

	position_ = VGet(0.0f, 0.0f, 0.0f);

	look_at_position_ = VGet(0.0f, 0.0f, 0.0f);

	shake_time_counter_ = 0.0f;

	shake_position_ = VGet(0.0f, 0.0f, 0.0f);

	// 遠景モデルの不自然な消失や、UI表示等による手前ポリゴンの欠け（クリッピング）を防ぐための描画深度設定

	SetCameraNearFar(100.0f, 50000.0f);

	// スカイボックス未ロード状態の時に、画面外に前フレームの描画残像が残るバグを防ぐための背景クリアカラー

	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);

	Update();

}

// 副作用：カメラ座標および注視点の設定、平行光源方向の更新、Effekseerカメラ行列の同期

void Camera::Update()

{

	// スキル選択時やデバッグカメラ起動時に、マウス移動により視点が変わるのを防ぐための入力カット

	if (Master::SelectSkill) return;

	if (Master::is_debug_camera_) return;

	if (target_ == nullptr)

	{

		target_ = ServiceLocator::GetPlayer();

	}

	UpdateRotate();

	if (target_ != nullptr)

	{

		// 追従対象（プレイヤー）の足元ではなく、UFOのコックピット付近を中心に捉えるための注視点補正

		look_at_position_ = target_->GetPosition();

		look_at_position_.y += 340.0f;

	}

	Shake();

	UpdatePositionAndTarget();

	prev_mouse_x_ = current_mouse_x_;

	prev_mouse_y_ = current_mouse_y_;

	GetMousePoint(&current_mouse_x_, &current_mouse_y_);

	UpdateEffekseerAndLight();

}

void Camera::UpdatePositionAndTarget()

{

	const float distance = 1000.0f;

	VECTOR temp;

	// カメラを一定距離（1000px）保ちつつ、注視点を中心に球を軌道を描くための極座標変換計算

	temp.x = distance * cosf(vertical_angle_ / 180.0f * 3.14159265f) * sinf(horizontal_angle_ / 180.0f * DX_PI_F);

	temp.y = distance * sinf(-vertical_angle_ / 180.0f * 3.14159265f);

	temp.z = -(distance * cosf(vertical_angle_ / 180.0f * DX_PI_F) * cosf(horizontal_angle_ / 180.0f * DX_PI_F));

	if (!is_phase_camera_active_)

	{

		position_ = VAdd(temp, look_at_position_);

		// 竜巻シェイク等の微小移動を最後に加算し、カメラのローカル座標系が上にズレてしまうのを防ぐ

		SetCameraPositionAndTarget_UpVecY(VAdd(position_, shake_position_), VAdd(look_at_position_, shake_position_));

	}

}

void Camera::UpdateEffekseerAndLight()

{

	// 3Dエフェクトがカメラの回転移動に追従し、ゲーム画面上で正しい位置に描画されるように同期する

	Effekseer_Sync3DSetting();

	// プレイヤーがどの視点から見ても影の落ち方が暗くなりすぎないよう、カメラの視点からライトの照射角を逆算する

	VECTOR lightDir = VSub(look_at_position_, position_);

	SetLightDirection(lightDir);

}

// 副作用：コントローラーやマウス入力に基づいたカメラ旋回角度の更新、マウスカーソルの中央固定

void Camera::UpdateRotate()

{

	// 天地逆転現象（ジンバルロック）を防ぐためにピッチ角（仰俯角）を制限し、ヨー角は360度シームレスにループさせる

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

	if (Master::scene_manager_->GetSceneType() == SceneManager::SCENE_TYPE::kGameScene || Master::scene_manager_->GetSceneType() == SceneManager::SCENE_TYPE::kSceneTutorial)

	{

		// 画面端へのマウス到達でカメラ回転がストップする現象を回避するため、ゲーム中はカーソルを不可視化して画面中央へクランプする

		SetMouseDispFlag(false);

		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;

		int center_y_ = 200;

		// デバッグ時に他画面へスムーズにカーソルを移動できるよう、キーボードの「0」を押している間は一時的に中央固定を解除する

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

// 出力：マウスが規定ピクセル以上移動しているかどうかの真偽値

bool Camera::IsMouseMoved()

{

	int moveX = abs(current_mouse_x_ - prev_mouse_x_);

	int moveY = abs(current_mouse_y_ - prev_mouse_y_);

	return moveX > 0.05f || moveY > 0.05f;

}

void Camera::Finalize()

{

}

// 副作用：サイン波に基づくシェイクオフセット座標の更新、減衰比率の加算

void Camera::Shake()

{

	if (shake_time_counter_ < old_shake_time_)

	{

		// 揺れ処理による本来のカメラ座標(position_)の意図せぬ座標ズレ（汚染）を防ぐため、描画直前にのみオフセット座標として加算する

		shake_position_.y = sinf(shake_angle_) * (1.0f - (shake_time_counter_ / old_shake_time_)) * shake_width_;

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

// 入力：time=継続時間(s), width=最大揺れ幅, angleSpeed=波形の角速度, stepTime=1フレームの進行時間(s)

// 副作用：シェイク制御パラメータの初期化

void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)

{

	shake_time_counter_ = 0.0f;

	old_shake_time_ = time;

	shake_width_ = width;

	shake_angle_speed_ = angleSpeed;

	step_time_ = stepTime;

}

// 入力：phase=現在のゲーム進行段階, ufoPos=UFOの座標, tornadoPos=竜巻の座標

// 副作用：指定フェーズに対応するアングル補間、Lerpによる座標変化

void Camera::UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos)

{

	if (Master::is_debug_camera_) return;

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

	// 演出中の操作不能（カメラ固定）によるテンポ悪化を防ぐため、最大3秒（180f）経過で強制的に通常俯瞰カメラへ差し戻す

	if (phaseTimer > 180)

	{

		is_phase_camera_active_ = false;

		return;

	}

	is_phase_camera_active_ = true;

	if (phase == (int)GameManager::GamePhase::kMassSpawn)

	{

		// 牛の大発生フェーズ：イベント開始時はUFOの巨大ビームを見上げる劇的なアングルにし、時間経過でプレイヤーの視線に緩やかに戻す

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

		// 竜巻襲来フェーズ：危険を早期に認識させるため、自機（UFO）と竜巻の双方が1つの画面に収まるようカメラを斜め上空へ引く

		targetPos = VAdd(ufoPos, VGet(0.0f, 500.0f, -200.0f));

		VECTOR toTornado = VSub(tornadoPos, ufoPos);

		targetLookAt = VAdd(ufoPos, toTornado);

	}

	// 瞬時にカメラを切り替えることによる急激な画面変化（3D酔い）を防止するため、現在のカメラ位置から目標位置へ線形補間（Lerp）で滑らかに移動させる

	float lerpSpeed = 0.1f * Master::GetDeltaTimeScaler();

	position_ = LerpVector(position_, targetPos, lerpSpeed);

	look_at_position_ = LerpVector(look_at_position_, targetLookAt, lerpSpeed);

	SetCameraPositionAndTarget_UpVecY(position_, look_at_position_);

}
