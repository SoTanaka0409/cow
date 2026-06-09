#include "Tatumaki.h"
#include "Player3D.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

Tatumaki::Tatumaki(VECTOR pos)
	: Object3D(pos)
{
	mTatumakiSpawnTimer = 0;
	mPos = pos;
	mMoveTimer = 0;
	mSpeed = 5.0f;
	mVelocity = VGet(0, 0, 0);

	mEffectTimer = 360;

	// 初期サイズを通常時の0.5倍スケールに設定し、進行に伴って巨大化できるようにする
	mIsCrisis = false;
	mCurrentScaleRatio = 0.5f; 
	mCurrentRadius = 400.0f;
	mpCapsuleCollider->mfRadius = mCurrentRadius;

	Tatu = new EffekseerEffect("Resource/3D/EFK/Tatumaki2.efk", mPos, 200.0f);

	// エフェクトの縦横比を調整し、竜巻としての立体感（縦長）を高める
	Tatu->SetScale(VGet(1.0f * mCurrentScaleRatio, 1.4f * mCurrentScaleRatio, 1.0f * mCurrentScaleRatio));
}

Tatumaki::~Tatumaki()
{
	// 動的にロードされたEffekseerエフェクト資源を破棄してメモリリークを防ぐ
	if (Tatu != nullptr)
	{
		delete Tatu;
		Tatu = nullptr;
	}
}

void Tatumaki::Update()
{
	// クライシス（後半戦）状態と通常状態のサイズを補完関数(Lerp)で滑らかに遷移させる
	float targetScale = mIsCrisis ? 1.0f : 0.5f;
	float targetRadius = mIsCrisis ? 800.0f : 400.0f;
	
	mCurrentScaleRatio += (targetScale - mCurrentScaleRatio) * 0.05f;
	mCurrentRadius += (targetRadius - mCurrentRadius) * 0.05f;
	
	Tatu->SetScale(VGet(1.0f * mCurrentScaleRatio, 1.4f * mCurrentScaleRatio, 1.0f * mCurrentScaleRatio));
	mpCapsuleCollider->mfRadius = mCurrentRadius;

	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);

	if (p != nullptr)
	{
		// プレイヤーの現在位置へ少しずつ向きを補正して、緩やかなホーミング挙動を実現する
		VECTOR playerPos = p->GetPosition();
		VECTOR targetDir = VSub(playerPos, mPos);

		targetDir.y = 0; // 高度方向の追従は不要なため無視する

		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			// 急激な旋回を防ぎ、逃げ回る余地を残すため、非常に小さい係数で進行方向を混ぜ合わせる
			float homingStrength = 0.001f;
			mVelocity = VAdd(mVelocity, VScale(targetDir, homingStrength));
			mVelocity = VNorm(mVelocity);
		}
	}

	mPos = VAdd(mPos, VScale(mVelocity, mSpeed));
	SetPosition(mPos);

	mpCapsuleCollider->mvPosition = VSub(mvPosition, VGet(0, 2000, 0));
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0, 2000, 0));

	// ステージ外（見えない境界）に逃げてしまわないよう、境界接触時に進行方向を反転させる
	float limit = 5000.0f;
	if (mPos.x < -limit || mPos.x > limit) { mVelocity.x *= -1; }
	if (mPos.z < -limit || mPos.z > limit) { mVelocity.z *= -1; }

	mEffectTimer--;
	if (mEffectTimer <= 0)
	{
		if (Tatu != nullptr) 
		{ 
			Tatu->Play(); 
			
			// プレイヤーが近くにいる場合のみ環境音を再生し、臨場感を出しつつ不要なSE混雑を防ぐ
			if (p != nullptr)
			{
				VECTOR diff = VSub(p->GetPosition(), mPos);
				if (VSquareSize(diff) < 3000.0f * 3000.0f)
				{
					Master::mpSoundManager->PlaySE(SoundManager::SE_TATUMAKI);
				}
			}
		}
		mEffectTimer = 360;
	}

	if (Tatu != nullptr)
	{
		Tatu->SetPosition(mPos);
		Tatu->Update();
	}
}

void Tatumaki::Draw()
{
}

void Tatumaki::OnEnter(Collider* collider, Collider* check)
{
	// プレイヤーが竜巻の当たり判定に接触した場合、ステージ上のランダムな安全圏にワープさせる
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_player)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->mpParentObject);

		float rangeLimit = 4000.0f;
		float warpX = (float)GetRand((int)rangeLimit * 2) - rangeLimit;
		float warpZ = (float)GetRand((int)rangeLimit * 2) - rangeLimit;

		Player->SetPosition(VGet(warpX, 2000.0f, warpZ));

		// 竜巻に巻き込まれた衝撃を表現するためにカメラを揺らす
		Master::mpCamera->SetupShake(20.0f, 35.0f, 30.0f);
	}
}

void Tatumaki::OnTrigger(Collider* collider, Collider* check)
{
}

void Tatumaki::OnExit(Collider* collider, Collider* check)
{
}
