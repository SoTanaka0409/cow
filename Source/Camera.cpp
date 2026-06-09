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
	
	// カメラのクリッピング距離（描画可能範囲）を近100〜遠50000の広範囲に設定
	SetCameraNearFar(100.0f, 50000.0f);

	// 描画されない背景部分をクリアする際のデフォルト色を灰色に設定
	SetBackgroundColor(128, 128, 128);

	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);
	Update();
}

void Camera::Update()
{
	// スキルカード選択中、またはデバッグの自由移動カメラ操作中はゲームカメラの更新をスキップ
	if (Master::SelectSkill) return;
	if (Master::mbIsDebugCamera) return;

	if (mpTarget == nullptr)
	{
		mpTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	}
	
	UpdateRotate();
	
	if (mpTarget != nullptr)
	{
		// カメラの注視点をプレイヤーキャラクターの中心やや上に設定する
		mvLookAtPosition = mpTarget->GetPosition();
		mvLookAtPosition.y += 340.0f;
	}
	
	Shake();

	{
		const float distance = 1000.0f;
		VECTOR temp;
		// 水平・垂直角度値（度数法）をラジアンに変換してカメラの3D座標オフセットを計算
		temp.x = distance * cosf(mfVerticalAngle / 180.0f * 3.14159265f) * sinf(mfHorizontalAngle / 180.0f * DX_PI_F);
		temp.y = distance * sinf(-mfVerticalAngle / 180.0f * 3.14159265f);
		temp.z = -(distance * cosf(mfVerticalAngle / 180.0f * DX_PI_F) * cosf(mfHorizontalAngle / 180.0f * DX_PI_F));
		
		if (!mbIsPhaseCameraActive)
		{
			mvPosition = VAdd(temp, mvLookAtPosition);
			
			// 算出したカメラ座標および注視点に、画面振動によるオフセット座標を加算して反映
			SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
		}
	}

	mPrevMouseX = mMouseX;
	mPrevMouseY = mMouseY;
	GetMousePoint(&mMouseX, &mMouseY);

	// 描画エフェクトの位置や方向が3Dカメラ視野角と合うよう、Effekseer側の3D空間設定と同期する
	Effekseer_Sync3DSetting();

	// カメラから注視点へ向かうベクトルを平行光源の向きとして設定し、順光表現を行う
	VECTOR lightDir = VSub(mvLookAtPosition, mvPosition);
	SetLightDirection(lightDir);
}

void Camera::UpdateRotate()
{
	// カメラ水平回転角および垂直回転角のオーバーフロー保護と範囲制限
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

		if (!mbIsPhaseCameraActive)
		{
			// 左右のマウス移動量をカメラの水平旋回角（ヨー角）に蓄積反映
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
		// 正弦波(sinf)と時間経過によるフェードアウト倍率を乗算してカメラの揺らし量を求める
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

	// 演出の開始から3秒（180フレーム）が経過したら、自動的に標準カメラへと戻す
	if (phaseTimer > 180)
	{
		mbIsPhaseCameraActive = false;
		return;
	}
	
	mbIsPhaseCameraActive = true;

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
