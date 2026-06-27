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
	tatumakiSpawnTimer = 0;
	pos = pos;
	moveTimer = 0;
	speed = 5.0f;
	velocity = VGet(0, 0, 0);

	effectTimer = 360;

	isCrisis = false;
	currentScaleRatio = 0.5f; 
	currentRadius = 400.0f;
	capsuleCollider->radius = currentRadius;

	Tatu = new EffekseerEffect("Resource/3D/EFK/Tatumaki2.efk", pos, 200.0f);

	// 初期状態で発生直後にプレイヤーを即死させないため、スケールを絞って生成する
	Tatu->SetScale(VGet(1.0f * currentScaleRatio, 1.4f * currentScaleRatio, 1.0f * currentScaleRatio));
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
	float targetScale = isCrisis ? 1.0f : 0.5f;
	float targetRadius = isCrisis ? 800.0f : 400.0f;
	
	currentScaleRatio += (targetScale - currentScaleRatio) * 0.05f;
	currentRadius += (targetRadius - currentRadius) * 0.05f;
	
	Tatu->SetScale(VGet(1.0f * currentScaleRatio, 1.4f * currentScaleRatio, 1.0f * currentScaleRatio));
	capsuleCollider->radius = currentRadius;

	auto players = ServiceLocator::GetPlayers();
	Player3D* p = nullptr;
	float minDistSq = -1.0f;

	// マルチプレイ環境下において、画面外の遠いプレイヤーを不自然に追跡するのを防ぐため最短距離の対象を検索する
	for (auto player : players)
	{
		VECTOR diff = VSub(player->GetPosition(), pos);
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
		VECTOR targetDir = VSub(playerPos, pos);
		targetDir.y = 0; 

		// 完全に重なった際のゼロ除算(VNorm)エラーを防ぐための閾値チェック
		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			// 旋回半径を大きくして回避可能な遊びを持たせるため、加算する向きのベクトルを弱める
			float homingStrength = 0.001f;
			velocity = VAdd(velocity, VScale(targetDir, homingStrength));
			velocity = VNorm(velocity);
		}
	}

	pos = VAdd(pos, VScale(velocity, speed * Master::GetDeltaTimeScaler()));
	SetPosition(pos);

	// カプセルコライダーを縦に長く取り、ジャンプ中のプレイヤーにも当たり判定を適用させる
	capsuleCollider->mvPosition = VSub(mvPosition, VGet(0, 2000, 0));
	capsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0, 2000, 0));

	// 竜巻がステージ外へ消失し、ゲーム進行に支障をきたすのを防ぐための見えない壁
	float limit = 5000.0f;
	if (pos.x < -limit || pos.x > limit) { velocity.x *= -1; }
	if (pos.z < -limit || pos.z > limit) { velocity.z *= -1; }

	effectTimer--;
	if (effectTimer <= 0)
	{
		if (Tatu != nullptr) 
		{ 
			Tatu->Play(); 
			
			// パフォーマンス最適化のため、プレイヤーから離れた位置でのSE再生を省略する
			if (p != nullptr)
			{
				VECTOR diff = VSub(p->GetPosition(), pos);
				if (VSquareSize(diff) < 3000.0f * 3000.0f)
				{
					Master::soundManager->PlaySE(SoundManager::SE_TATUMAKI);
				}
			}
		}
		effectTimer = 360;
	}

	// エフェクトの座標をオブジェクトに追従させ、描画情報の同期を取る
	if (Tatu != nullptr)
	{
		Tatu->SetPosition(pos);
		Tatu->Update();
	}
}

void Tatumaki::Draw()
{
}

void Tatumaki::OnEnter(Collider* collider, Collider* check)
{
	// 竜巻接触時、プレイヤーをランダムな位置へワープさせる（ペナルティ仕様）
	if (collider == capsuleCollider && check->parentObject->GetTag() == Tag3D_player)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->parentObject);

		float rangeLimit = 4000.0f;
		float warpX = (float)GetRand((int)rangeLimit * 2) - rangeLimit;
		float warpZ = (float)GetRand((int)rangeLimit * 2) - rangeLimit;

		Player->SetPosition(VGet(warpX, 2000.0f, warpZ));

		// プレイヤーへの視覚的フィードバックとして画面を激しく揺らす
		Master::camera->SetupShake(20.0f, 35.0f, 30.0f);
	}
}

void Tatumaki::OnTrigger(Collider* collider, Collider* check)
{
}

void Tatumaki::OnExit(Collider* collider, Collider* check)
{
}
