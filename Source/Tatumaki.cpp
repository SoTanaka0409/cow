#include "ServiceLocator.h"
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

	mIsCrisis = false;
	mCurrentScaleRatio = 0.5f; 
	mCurrentRadius = 400.0f;
	mpCapsuleCollider->mfRadius = mCurrentRadius;

	Tatu = new EffekseerEffect("Resource/3D/EFK/Tatumaki2.efk", mPos, 200.0f);

	// 初期状態で発生直後にプレイヤーを即死させないため、スケールを絞って生成する
	Tatu->SetScale(VGet(1.0f * mCurrentScaleRatio, 1.4f * mCurrentScaleRatio, 1.0f * mCurrentScaleRatio));
}

Tatumaki::~Tatumaki()
{
	// メモリリーク回避のため、外部リソースであるEffekseerEffectを明示的に解放する
	if (Tatu != nullptr)
	{
		delete Tatu;
		Tatu = nullptr;
	}
}

void Tatumaki::Update()
{
	// サイズ急変による視覚的違和感を防ぐため、目標サイズに向けてLerpで徐々に補間する
	float targetScale = mIsCrisis ? 1.0f : 0.5f;
	float targetRadius = mIsCrisis ? 800.0f : 400.0f;
	
	mCurrentScaleRatio += (targetScale - mCurrentScaleRatio) * 0.05f;
	mCurrentRadius += (targetRadius - mCurrentRadius) * 0.05f;
	
	Tatu->SetScale(VGet(1.0f * mCurrentScaleRatio, 1.4f * mCurrentScaleRatio, 1.0f * mCurrentScaleRatio));
	mpCapsuleCollider->mfRadius = mCurrentRadius;

	auto players = ServiceLocator::GetPlayers();
	Player3D* p = nullptr;
	float minDistSq = -1.0f;

	// マルチプレイ環境下において、画面外の遠いプレイヤーを不自然に追跡するのを防ぐため最短距離の対象を検索する
	for (auto player : players)
	{
		VECTOR diff = VSub(player->GetPosition(), mPos);
		diff.y = 0; // 高低差による追跡対象のブレを防ぐため、XZ平面のみで距離計算を行う
		float distSq = VSquareSize(diff);
		if (minDistSq < 0 || distSq < minDistSq)
		{
			minDistSq = distSq;
			p = player;
		}
	}

	// 追従対象が存在する場合のみ、ホーミング処理を実行しプレイヤーに向かってベクトルを向ける
	if (p != nullptr)
	{
		VECTOR playerPos = p->GetPosition();
		VECTOR targetDir = VSub(playerPos, mPos);
		targetDir.y = 0; 

		// 完全に重なった際のゼロ除算(VNorm)エラーを防ぐための閾値チェック
		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			// 旋回半径を大きくして回避可能な遊びを持たせるため、加算する向きのベクトルを弱める
			float homingStrength = 0.001f;
			mVelocity = VAdd(mVelocity, VScale(targetDir, homingStrength));
			mVelocity = VNorm(mVelocity);
		}
	}

	mPos = VAdd(mPos, VScale(mVelocity, mSpeed));
	SetPosition(mPos);

	// カプセルコライダーを縦に長く取り、ジャンプ中のプレイヤーにも当たり判定を適用させる
	mpCapsuleCollider->mvPosition = VSub(mvPosition, VGet(0, 2000, 0));
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0, 2000, 0));

	// 竜巻がステージ外へ消失し、ゲーム進行に支障をきたすのを防ぐための見えない壁
	float limit = 5000.0f;
	if (mPos.x < -limit || mPos.x > limit) { mVelocity.x *= -1; }
	if (mPos.z < -limit || mPos.z > limit) { mVelocity.z *= -1; }

	mEffectTimer--;
	if (mEffectTimer <= 0)
	{
		if (Tatu != nullptr) 
		{ 
			Tatu->Play(); 
			
			// パフォーマンス最適化のため、プレイヤーから離れた位置でのSE再生を省略する
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

	// エフェクトの座標をオブジェクトに追従させ、描画情報の同期を取る
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
	// 竜巻接触時、プレイヤーをランダムな位置へワープさせる（ペナルティ仕様）
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_player)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->mpParentObject);

		float rangeLimit = 4000.0f;
		float warpX = (float)GetRand((int)rangeLimit * 2) - rangeLimit;
		float warpZ = (float)GetRand((int)rangeLimit * 2) - rangeLimit;

		Player->SetPosition(VGet(warpX, 2000.0f, warpZ));

		// プレイヤーへの視覚的フィードバックとして画面を激しく揺らす
		Master::mpCamera->SetupShake(20.0f, 35.0f, 30.0f);
	}
}

void Tatumaki::OnTrigger(Collider* collider, Collider* check)
{
}

void Tatumaki::OnExit(Collider* collider, Collider* check)
{
}
