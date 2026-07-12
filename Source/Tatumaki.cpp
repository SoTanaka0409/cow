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
	tatumaki_spawn_timer_ = 0;
	pos_ = pos;
	move_timer_ = 0;
	speed_ = 5.0f;
	velocity_ = VGet(0, 0, 0);

	effect_timer_ = 360;

	is_crisis_ = false;
	current_scale_ratio_ = 0.5f; 
	current_radius_ = 400.0f;
	capsule_collider_->radius_ = current_radius_;

	tatu_ = new EffekseerEffect("Resource/3D/EFK/Tatumaki2.efk", pos_, 200.0f);

	// 初期状態で発生直後にプレイヤーを即死させなぁE��め、スケールを絞って生�Eする
	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
}

Tatumaki::~Tatumaki()
{
	// メモリリーク回避のため、外部リソースであるEffekseerEffectを�E示皁E��解放する
	if (tatu_ != nullptr)
	{
		delete tatu_;
		tatu_ = nullptr;
	}
}

void Tatumaki::Update()
{
	// サイズ急変による視覚的違和感を防ぐため、目標サイズに向けてLerpで徐、E��補間する
	float targetScale = is_crisis_ ? 1.0f : 0.5f;
	float targetRadius = is_crisis_ ? 800.0f : 400.0f;
	
	current_scale_ratio_ += (targetScale - current_scale_ratio_) * 0.05f;
	current_radius_ += (targetRadius - current_radius_) * 0.05f;
	
	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
	capsule_collider_->radius_ = current_radius_;

	auto players = ServiceLocator::GetPlayers();
	Player3D* p = nullptr;
	float minDistSq = -1.0f;

	// マルチ�Eレイ環墁E��において、画面外�E遠ぁE�Eレイヤーを不�E然に追跡するのを防ぐため最短距離の対象を検索する
	for (auto player : players)
	{
		VECTOR diff = VSub(player->GetPosition(), pos_);
		diff.y = 0; // 高低差による追跡対象のブレを防ぐため、XZ平面のみで距離計算を行う
		float distSq = VSquareSize(diff);
		if (minDistSq < 0 || distSq < minDistSq)
		{
			minDistSq = distSq;
			p = player;
		}
	}

	// 追従対象が存在する場合�Eみ、�Eーミング処琁E��実行しプレイヤーに向かってベクトルを向ける
	if (p != nullptr)
	{
		VECTOR playerPos = p->GetPosition();
		VECTOR targetDir = VSub(playerPos, pos_);
		targetDir.y = 0; 

		// 完�Eに重なった際のゼロ除箁EVNorm)エラーを防ぐため�E閾値チェチE��
		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			// 旋回半征E��大きくして回避可能な遊�Eを持たせるため、加算する向き�Eベクトルを弱める
			float homingStrength = 0.001f;
			velocity_ = VAdd(velocity_, VScale(targetDir, homingStrength));
			velocity_ = VNorm(velocity_);
		}
	}

	pos_ = VAdd(pos_, VScale(velocity_, speed_));
	SetPosition(pos_);

	// カプセルコライダーを縦に長く取り、ジャンプ中のプレイヤーにも当たり判定を適用させめE
	capsule_collider_->position_ = VSub(position_, VGet(0, 2000, 0));
	capsule_collider_->position2_ = VAdd(position_, VGet(0, 2000, 0));

	// 竜巻がスチE�Eジ外へ消失し、ゲーム進行に支障をきたす�Eを防ぐため�E見えなぁE��E
	float limit = 5000.0f;
	if (pos_.x < -limit || pos_.x > limit) { velocity_.x *= -1; }
	if (pos_.z < -limit || pos_.z > limit) { velocity_.z *= -1; }

	effect_timer_--;
	if (effect_timer_ <= 0)
	{
		if (tatu_ != nullptr) 
		{ 
			tatu_->Play(); 
			
			// パフォーマンス最適化�Eため、�Eレイヤーから離れた位置でのSE再生を省略する
			if (p != nullptr)
			{
				VECTOR diff = VSub(p->GetPosition(), pos_);
				if (VSquareSize(diff) < 3000.0f * 3000.0f)
				{
					Master::mpSoundManager->PlaySE(SoundManager::kSeTatumaki);
				}
			}
		}
		effect_timer_ = 360;
	}

	// エフェクト�E座標をオブジェクトに追従させ、描画惁E��の同期を取めE
	if (tatu_ != nullptr)
	{
		tatu_->SetPosition(pos_);
		tatu_->Update();
	}
}

void Tatumaki::Draw()
{
}

void Tatumaki::OnEnter(Collider* collider, Collider* check)
{
	// 竜巻接触時、�Eレイヤーをランダムな位置へワープさせる�E��EナルチE��仕様！E
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dPlayer)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->parent_object_);

		float rangeLimit = 4000.0f;
		float warpX = (float)GetRand((int)rangeLimit * 2) - rangeLimit;
		float warpZ = (float)GetRand((int)rangeLimit * 2) - rangeLimit;

		Player->SetPosition(VGet(warpX, 2000.0f, warpZ));

		// プレイヤーへの視覚的フィードバチE��として画面を激しく揺らす
		Master::camera_->SetupShake(20.0f, 35.0f, 30.0f);
	}
}

void Tatumaki::OnTrigger(Collider* collider, Collider* check)
{
}

void Tatumaki::OnExit(Collider* collider, Collider* check)
{
}