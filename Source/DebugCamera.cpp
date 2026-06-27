#include "DebugCamera.h"
#include <cmath>
#include "Master.h"
#include "Camera.h"
#include "InputManager.h"

DebugCamera::DebugCamera()
	: horizontalAngle(0.0f)
	, verticalAngle(-30.0f)
	, mvPosition(VGet(0.0f, 1000.0f, 0.0f))
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Initialize()
{
	// カメラの初期位置をゲーム中カメラの位置から引き継ぎ、スムーズに切り替えできるようにする
	if (Master::camera != nullptr)
	{
		mvPosition = Master::camera->GetPosition();
	}
	else
	{
		mvPosition = VGet(0.0f, 1000.0f, 0.0f);
	}
	horizontalAngle = 0.0f;
	verticalAngle = -30.0f;
}

void DebugCamera::Update()
{
	float speed = 20.0f;
	// LSHIFTキーが押されている間は、デバッグカメラの移動速度を高速化する
	if (CheckHitKey(KEY_INPUT_LSHIFT)) speed = 100.0f;

	VECTOR moveVec = VGet(0, 0, 0);
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);   // カメラの上方向（前進）移動ベクトル
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f); // カメラの左方向移動ベクトル
	
	UpdateRotate();
	
	float dist = 1000.0f;
	VECTOR target;
	target.x = mvPosition.x + dist * cosf(verticalAngle * DX_PI_F / 180.0f) * sinf(horizontalAngle * DX_PI_F / 180.0f);
	target.y = mvPosition.y + dist * sinf(-verticalAngle * DX_PI_F / 180.0f);
	target.z = mvPosition.z - dist * cosf(verticalAngle * DX_PI_F / 180.0f) * cosf(horizontalAngle * DX_PI_F / 180.0f);

	{
		// カメラの視線ベクトルから前進移動成分を求める
		UpMoveVector = VSub(target, mvPosition);

		// カメラの前進ベクトルとY軸の上ベクトルとの外積をとり、真左方向のベクトルを求める
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

	// 最新の座標をもとにカメラの注視点を再計算する
	target.x = mvPosition.x + dist * cosf(verticalAngle * DX_PI_F / 180.0f) * sinf(horizontalAngle * DX_PI_F / 180.0f);
	target.y = mvPosition.y + dist * sinf(-verticalAngle * DX_PI_F / 180.0f);
	target.z = mvPosition.z - dist * cosf(verticalAngle * DX_PI_F / 180.0f) * cosf(horizontalAngle * DX_PI_F / 180.0f);

	// DXライブラリのカメラに、再計算されたデバッグ用カメラの位置と注視点を反映させる
	SetCameraPositionAndTarget_UpVecY(mvPosition, target);
}

void DebugCamera::UpdateRotate()
{
	// 角度値が範囲外（±180度）にオーバーフローしないようにラップ処理を行う
	if (horizontalAngle >= 180.0f)
	{
		horizontalAngle -= 360.0f;
	}
	if (horizontalAngle <= -180.0f)
	{
		horizontalAngle += 360.0f;
	}

	if (verticalAngle >= 80.0f)
	{
		verticalAngle = 80.0f;
	}
	if (verticalAngle <= -80.0f)
	{
		verticalAngle = -80.0f;
	}

	const float MOUSE_SENSITIVITY = 0.05f;

	if (Master::sceneManager->GetSceneType() == SceneManager::SCENE_TYPE::SCENE_3D || Master::sceneManager->GetSceneType() == SceneManager::SCENE_TYPE::SCENE_TUTORIAL)
	{
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);

		int centerX = 640;
		int centerY = 360;

		// 視線操作を行いやすくするため、マウスカーソルを画面中心に毎回引き戻す（0キーで解除可能）
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(centerX, centerY);
		}

		int deltaX = mouseX - centerX;
		int deltaY = mouseY - centerY;

		horizontalAngle -= deltaX * MOUSE_SENSITIVITY;
		verticalAngle += deltaY * MOUSE_SENSITIVITY;
	}
}

