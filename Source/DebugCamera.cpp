#include "DebugCamera.h"
#include <cmath>
#include "Master.h"
#include "Camera.h"
#include "InputManager.h"

/*
 * 入力: なし
 * 出力: なし
 * 副作用: メンバ変数の初期化
 */
DebugCamera::DebugCamera()
	: horizontal_angle_(0.0f)
	, vertical_angle_(-30.0f)
	, position_(VGet(0.0f, 1000.0f, 0.0f))
{
}

DebugCamera::~DebugCamera()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: デバッグカメラの初期座標・角度の設定
 */
void DebugCamera::Initialize()
{
	// デバッグモード起動時に視点が原点へ飛ぶのを防ぐため、プレイ中のゲームカメラの座標をそのまま引き継ぐ
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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: キー入力による座標更新と描画エンジン(DxLib)へのカメラ行列適用
 */
void DebugCamera::Update()
{
	float speed = 20.0f;

	// 広大な3Dマップを迅速に巡回・レベルデザイン確認できるよう、左Shift押下で移動速度を大幅に上げる
	if (CheckHitKey(KEY_INPUT_LSHIFT)) speed = 100.0f;

	VECTOR moveVec = VGet(0, 0, 0);
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);

	UpdateRotate();

	float dist = 1000.0f;
	VECTOR target;
	target.x = position_.x + dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * sinf(horizontal_angle_ * DX_PI_F / 180.0f);
	target.y = position_.y + dist * sinf(-vertical_angle_ * DX_PI_F / 180.0f);
	target.z = position_.z - dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * cosf(horizontal_angle_ * DX_PI_F / 180.0f);

	{
		// 常にカメラの向いている方向を基準(ローカル軸)として前後左右に移動させるため、外積を用いて方向ベクトルを算出する
		UpMoveVector = VSub(target, position_);
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

	// 移動処理によってカメラ座標と注視点の相対距離が崩れると視界が歪むため、移動後の座標を基に注視点を再計算する
	target.x = position_.x + dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * sinf(horizontal_angle_ * DX_PI_F / 180.0f);
	target.y = position_.y + dist * sinf(-vertical_angle_ * DX_PI_F / 180.0f);
	target.z = position_.z - dist * cosf(vertical_angle_ * DX_PI_F / 180.0f) * cosf(horizontal_angle_ * DX_PI_F / 180.0f);

	SetCameraPositionAndTarget_UpVecY(position_, target);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: マウス移動量に基づくカメラ角度の更新と、OSのカーソル位置の強制変更
 */
void DebugCamera::UpdateRotate()
{
	// 天地逆転(ジンバルロック)による操作不能状態や、角度値のオーバーフローによる浮動小数点誤差を防ぐためのクランプ処理
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
		int mouse_x_, mouse_y_;
		GetMousePoint(&mouse_x_, &mouse_y_);

		int center_x_ = 640;
		int center_y_ = 360;

		// カーソルがOSの画面端に到達して旋回不能になるのを防ぐため、毎フレーム画面中央にマウス位置を固定し無限旋回を可能にする
		// デバッグ中に他ツール(プロファイラやエディタ等)を操作できるよう、0キー押下中のみ強制固定を一時解除する
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
