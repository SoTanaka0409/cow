#include "ServiceLocator.h"
#include "Tornado.h"
#include "Player3D.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Wall.h"
#include <vector>

// 入力：竜巻の初期発生座標
// 副作用：EffekseerEffect インスタンスの動的メモリ確保
Tornado::Tornado(VECTOR pos)
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

	tatu_ = new EffekseerEffect("Resource/3D/EFK/Tornado2.efk", pos_, 200.0f);

	// 序盤の難易度を抑え、プレイヤーに回避の余裕を与えるため初期サイズを小さく設定
	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
}

// 副作用：動的確保したエフェクトインスタンスのメモリ解放
Tornado::~Tornado()
{
	// メモリリークを防止するため、外からインジェクトされず自前で new したエフェクトを解放
	if (tatu_ != nullptr)
	{
		delete tatu_;
		tatu_ = nullptr;
	}
}

// 副作用：サイズ補間計算、追尾対象の選定、座標および移動ベクトルの更新、SEの再生
void Tornado::Update()
{
	// サイズが急激に変化する視覚的違和感を防ぐため、目標値に向けて毎フレーム Lerp（線形補間）する
	float targetScale = is_crisis_ ? 1.0f : 0.5f;
	float targetRadius = is_crisis_ ? 800.0f : 400.0f;

	current_scale_ratio_ += (targetScale - current_scale_ratio_) * 0.05f;
	current_radius_ += (targetRadius - current_radius_) * 0.05f;

	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
	capsule_collider_->radius_ = current_radius_;

	auto players = ServiceLocator::GetPlayers();
	Player3D* p = nullptr;
	float minDistSq = -1.0f;

	// マルチプレイ時の挙動として、最も近くにいて脅威度の高いプレイヤーを追尾対象として選出する
	for (auto player : players)
	{
		VECTOR diff = VSub(player->GetPosition(), pos_);
		diff.y = 0; // 竜巻は地上を移動するため、高度の差による距離の計算ズレを排除する
		float distSq = VSquareSize(diff);
		if (minDistSq < 0 || distSq < minDistSq)
		{
			minDistSq = distSq;
			p = player;
		}
	}

	// 旋回性能が高すぎるとプレイヤーが逃げ切れないため、微小な力（0.001）で緩やかにホーミングさせる
	if (p != nullptr)
	{
		VECTOR playerPos = p->GetPosition();
		VECTOR targetDir = VSub(playerPos, pos_);
		targetDir.y = 0;

		// ゼロベクトルの時に VNorm を呼ぶと計算結果が不定（NaN）になるバグを回避するための安全弁
		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			float homingStrength = 0.001f;
			velocity_ = VAdd(velocity_, VScale(targetDir, homingStrength));
			velocity_ = VNorm(velocity_);
		}
	}

	pos_ = VAdd(pos_, VScale(velocity_, speed_));
	SetPosition(pos_);

	// プレイヤーがジャンプ等で垂直に逃げても捕捉できるよう、上空 2000px まで判定を伸ばす
	capsule_collider_->position_ = VSub(position_, VGet(0, 2000, 0));
	capsule_collider_->position2_ = VAdd(position_, VGet(0, 2000, 0));

	// 竜巻がステージ外へ消失してゲーム進行不能になるのを防ぐため、5000 の境界で跳ね返らせる
	float limit = 5000.0f;
	if (pos_.x < -limit || pos_.x > limit) { velocity_.x *= -1; }
	if (pos_.z < -limit || pos_.z > limit) { velocity_.z *= -1; }

	effect_timer_--;
	if (effect_timer_ <= 0)
	{
		if (tatu_ != nullptr)
		{
			tatu_->Play();

			// パフォーマンスと聴覚的乱雑さを抑えるため、近くのプレイヤーにのみSEを鳴らす
			if (p != nullptr)
			{
				VECTOR diff = VSub(p->GetPosition(), pos_);
				if (VSquareSize(diff) < 3000.0f * 3000.0f)
				{
					Master::mpSoundManager->PlaySE(SoundManager::kSeTornado);
				}
			}
		}
		effect_timer_ = 360;
	}

	// エフェクトの移動漏れによる、見た目と当たり判定の位置ズレ（同期ズレ）を防止する
	if (tatu_ != nullptr)
	{
		tatu_->SetPosition(pos_);
		tatu_->Update();
	}

	// ノックバック中のプレイヤーの処理
	for (auto it = knockbacks_.begin(); it != knockbacks_.end(); ) {
		Player3D* p_knock = it->player;
		VECTOR& vel = it->velocity;

		VECTOR oldPos = p_knock->GetPosition();
		VECTOR newPos = VAdd(oldPos, vel);

		// 壁との衝突判定
		bool hitwall = false;
		const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dWall);
		for (auto& w : walls) {
			Wall* wall = dynamic_cast<Wall*>(w);
			if (wall != nullptr) {
				std::vector<VERTEX3D> vertex = wall->GetVertex();
				if (HitCheck_Capsule_Triangle(
					newPos, VAdd(newPos, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
					newPos, VAdd(newPos, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
					vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos))
				{
					hitwall = true;
					// 壁に当たった場合はその場で停止させる
					newPos = oldPos;
					vel = VGet(0.0f, 0.0f, 0.0f);
					break;
				}
			}
		}

		p_knock->SetPosition(newPos);

		// 速度の減衰（摩擦）
		vel = VScale(vel, 0.9f);

		// 速度が十分に小さくなったらリストから除外
		if (VSize(vel) < 0.5f) {
			it = knockbacks_.erase(it);
		} else {
			++it;
		}
	}
}

void Tornado::Draw()
{
}

// 入力：collider=自身の衝突判定, check=相手の衝突判定
// 副作用：接触したプレイヤーの座標変更、カメラシェイクの起動
void Tornado::OnEnter(Collider* collider, Collider* check)
{
	// 竜巻に巻き込まれたプレイヤーを上空へ吹き飛ばしつつ、ランダムな位置へテレポートさせる仕様
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dPlayer)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->parent_object_);

		// 台風からプレイヤーへ向かうベクトルを計算（Y軸の高さは無視）
		VECTOR diff = VSub(Player->GetPosition(), pos_);
		diff.y = 0.0f;
		
		// 完全に重なっている場合は適当な方向に飛ばす
		if (VSize(diff) < 0.1f) {
			diff = VGet(1.0f, 0.0f, 0.0f);
		}
		
		// 吹き飛ばす初速（30.0f）
		VECTOR knockbackVelocity = VScale(VNorm(diff), 30.0f);
		
		// 既にリストにあるかチェックして更新、なければ追加
		bool found = false;
		for (auto& kb : knockbacks_) {
			if (kb.player == Player) {
				kb.velocity = knockbackVelocity;
				found = true;
				break;
			}
		}
		if (!found) {
			knockbacks_.push_back({ Player, knockbackVelocity });
		}

		// 竜巻に直撃した衝撃を視覚的にフィードバックし、危機感を演出する
		Master::camera_->SetupShake(20.0f, 35.0f, 30.0f);
	}
}

void Tornado::OnTrigger(Collider* collider, Collider* check)
{
}

void Tornado::OnExit(Collider* collider, Collider* check)
{
}