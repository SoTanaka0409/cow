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
	: horizontalAngle(0.0f)
	, verticalAngle(-55.0f)
	, isPhaseCameraActive(false)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, target(nullptr)
	, shakeTimeFrames(0)
	, shakeTimeCount(0)
	, shakeAngle(0.0f)
	, shakeTimeCounter(0.0f)
	, shakeTime(0.0f)
	, shakeWidth(0.0f)
	, shakeAngleSpeed(0.0f)
	, stepTime(0.0f)
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	target = nullptr;
	
	// カメラのクリッピング距離（描画可能範囲）を近100?遠50000の広範囲に設定
	SetCameraNearFar(100.0f, 50000.0f);

	// 描画されない背景部分をクリアする際のデフォルト色を灰色に設定
	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);
	Update();
}

void Camera::Update()
{
	// スキルカード選択中、またはデバッグの自由移動カメラ操作中はゲームカメラの更新をスキップ
	if (Master::selectSkill) return;
	if (Master::isDebugCamera) return;

	if (target == nullptr)
	{
		target = ServiceLocator::GetPlayer();
	}
	
	UpdateRotate();
	
	if (target != nullptr)
	{
		// カメラの注視点をプレイヤーキャラクターの中心やや上に設定する
		mvLookAtPosition = target->GetPosition();
		mvLookAtPosition.y += 340.0f;
	}
	
	Shake();

	{
		const float distance = 1000.0f;
		VECTOR temp;
		// 水平・垂直角度値（度数法）をラジアンに変換してカメラの3D座標オフセットを計算
		temp.x = distance * cosf(verticalAngle / 180.0f * 3.14159265f) * sinf(horizontalAngle / 180.0f * DX_PI_F);
		temp.y = distance * sinf(-verticalAngle / 180.0f * 3.14159265f);
		temp.z = -(distance * cosf(verticalAngle / 180.0f * DX_PI_F) * cosf(horizontalAngle / 180.0f * DX_PI_F));
		
		if (!isPhaseCameraActive)
		{
			mvPosition = VAdd(temp, mvLookAtPosition);
			
			// 算出したカメラ座標および注視点に、画面振動によるオフセット座標を加算して反映
			SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
		}
	}

	prevMouseX = currentMouseX;
	prevMouseY = currentMouseY;
	GetMousePoint(&currentMouseX, &currentMouseY);

	// 描画エフェクトの位置や方向が3Dカメラ視野角と合うよう、Effekseer側の3D空間設定と同期する
	Effekseer_Sync3DSetting();

	// カメラから注視点へ向かうベクトルを平行光源の向きとして設定し、順光表現を行う
	VECTOR lightDir = VSub(mvLookAtPosition, mvPosition);
	SetLightDirection(lightDir);
}

void Camera::UpdateRotate()
{
	// カメラ水平回転角および垂直回転角のオーバーフロー保護と範囲制限
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
		// スキル選択中でない場合はゲームプレイ用のマウスキャプチャを行う
		SetMouseDispFlag(false);
		GetMousePoint(&mouseX, &mouseY);

		int centerX = 640;
		int centerY = 200;

		// 0キーでマウスの拘束をデバッグ目的で一時解除できるようにする
		if (!CheckHitKey(KEY_INPUT_0))
		{
			SetMousePoint(centerX, centerY);
		}

		int deltaX = mouseX - centerX;

		if (!isPhaseCameraActive)
		{
			// 左右のマウス移動量をカメラの水平旋回角（ヨー角）に蓄積反映
			horizontalAngle -= deltaX * MOUSE_SENSITIVITY;
		}
	}
}

bool Camera::IsMouseMoved()
{
	int moveX = abs(currentMouseX - prevMouseX);
	int moveY = abs(currentMouseY - prevMouseY);

	return moveX > 0.05f || moveY > 0.05f;
}

void Camera::Finalize()
{
}

void Camera::Shake()
{
	if (shakeTimeCounter < shakeTime)
	{
		// 正弦波(sinf)と時間経過によるフェードアウト倍率を乗算してカメラの揺らし量を求める
		mvShakePosition.y = sinf(shakeAngle) * (1.0f - (shakeTimeCounter / shakeTime)) * shakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		shakeAngle += shakeAngleSpeed * stepTime;
		shakeTimeCounter += stepTime;
	}
	else
	{
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

void Camera::SetupShake(float time, float width, float angleSpeed, float stepTime)
{
	shakeTimeCounter = 0.0f;
	shakeTime = time;
	shakeWidth = width;
	shakeAngleSpeed = angleSpeed;
	stepTime = stepTime;
}

void Camera::UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos)
{
	if (Master::isDebugCamera) return;

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
		isPhaseCameraActive = false;
		return;
	}

	// 演出の開始から3秒（180フレーム）が経過したら、自動的に標準カメラへと戻す
	if (phaseTimer > 180)
	{
		isPhaseCameraActive = false;
		return;
	}
	
	isPhaseCameraActive = true;

	if (phase == (int)GameManager::GamePhase::MassSpawn)
	{
		// 牛大量発生演出：カメラを引き下げつつ、空を見上げる（カメラピッチ角を下に向ける）
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
		// 竜巻発生演出：カメラ高度を上げて全体を見渡し、注視点を竜巻にする
		targetPos = VAdd(ufoPos, VGet(0.0f, 500.0f, -200.0f));
		VECTOR toTornado = VSub(tornadoPos, ufoPos);
		targetLookAt = VAdd(ufoPos, toTornado);
	}

	// 現在のカメラパラメータを線形補間（Lerp）を用いて目標値へスムーズに遷移させる
	float lerpSpeed = 0.1f; 
	mvPosition = LerpVector(mvPosition, targetPos, lerpSpeed);
	mvLookAtPosition = LerpVector(mvLookAtPosition, targetLookAt, lerpSpeed);

	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);
}
