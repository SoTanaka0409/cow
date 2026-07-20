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
#include <algorithm>
#include <vector>

// 入力：竜巻の初期発生座標
// 副作用：EffekseerEffect インスタンスの動的メモリ確保
Tornado::Tornado(VECTOR pos)
	: Object3D(pos)
{
	tatumaki_spawn_timer_ = 0;
	move_timer_ = 0;
	speed_ = 5.0f;
	velocity_ = VGet(1.0f, 0.0f, 0.0f);

	effect_timer_ = 360;

	is_crisis_ = false;
	current_scale_ratio_ = 0.5f;
	current_radius_ = 400.0f;
	capsule_collider_->radius_ = current_radius_;

	tatu_ = new EffekseerEffect("Resource/3D/エフェクト/竜巻.efk", position_, 200.0f);
	tatu_->Play();

	// 序盤の難易度を抑え、プレイヤーに回避の余裕を与えるため初期サイズを小さく設定
	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
}

// 副作用：動的確保したエフェクトインスタンスのメモリ解放
Tornado::~Tornado()
{
	// メモリリークを防止するため、外からインジェクトされず自前で new 項目を解放
	if (tatu_ != nullptr)
	{
		delete tatu_;
		tatu_ = nullptr;
	}
}

// 副作用：サイズ補間計算、追尾対象の選定、座標および移動ベクトルの更新、SEの再生、ノックバック物理シミュレーション
void Tornado::Update()
{
	UpdateScaleAndRadius();
	UpdateHomingPlayer();
	UpdateWallBounce();
	UpdateEffectAndSound();
	UpdateKnockback();
}

void Tornado::UpdateScaleAndRadius()
{
	// サイズが急激に変化する視覚的違和感を防ぐため、目標値に向けて毎フレーム Lerp（線形補間）する
	float targetScale = is_crisis_ ? 1.0f : 0.5f;
	float targetRadius = is_crisis_ ? 800.0f : 400.0f;

	current_scale_ratio_ += (targetScale - current_scale_ratio_) * 0.05f;
	current_radius_ += (targetRadius - current_radius_) * 0.05f;

	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
	capsule_collider_->radius_ = current_radius_;
}

void Tornado::UpdateHomingPlayer()
{
	auto players = ServiceLocator::GetPlayers();
	Player3D* p = nullptr;
	float minDistSq = -1.0f;

	// マルチプレイ時の挙動として、最も近くにいる（危険度の高い）プレイヤーを追尾対象として選出する
	for (auto player : players)
	{
		VECTOR diff = VSub(player->GetPosition(), position_);
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
		VECTOR targetDir = VSub(playerPos, position_);
		targetDir.y = 0;

		// ゼロベクトルの時に VNorm を呼ぶと計算結果が不定（NaN）になるバグを回避するための安全弁
		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			float homingStrength = 0.001f;
			if (VSquareSize(velocity_) < 0.001f) {
				velocity_ = targetDir;
			} else {
				velocity_ = VAdd(velocity_, VScale(targetDir, homingStrength));
				velocity_ = VNorm(velocity_);
			}
		}
	}

	position_ = VAdd(position_, VScale(velocity_, speed_));

	// プレイヤーがジャンプ等で垂直に逃げても捕捉できるよう、上空 2000px まで判定を伸ばす
	capsule_collider_->position_ = VSub(position_, VGet(0, 2000, 0));
	capsule_collider_->position2_ = VAdd(position_, VGet(0, 2000, 0));
}

void Tornado::UpdateWallBounce()
{
	// 竜巻がステージ外へ消失してゲーム進行不可になるのを防ぐため、5000 の壁で跳ね返らせる
	float limit = 5000.0f;
	if (position_.x < -limit || position_.x > limit) { velocity_.x *= -1; }
	if (position_.z < -limit || position_.z > limit) { velocity_.z *= -1; }
}

void Tornado::UpdateEffectAndSound()
{
	effect_timer_--;
	if (effect_timer_ <= 0)
	{
		if (tatu_ != nullptr)
		{
			tatu_->Play();

			// パフォーマンスと聴覚的乱雑さを抑えるため、近くのプレイヤーにのみSEを鳴らす
			auto players = ServiceLocator::GetPlayers();
			Player3D* p = nullptr;
			float minDistSq = -1.0f;
			for (auto player : players)
			{
				VECTOR diff = VSub(player->GetPosition(), position_);
				diff.y = 0;
				float distSq = VSquareSize(diff);
				if (minDistSq < 0 || distSq < minDistSq)
				{
					minDistSq = distSq;
					p = player;
				}
			}

			if (p != nullptr)
			{
				VECTOR diff = VSub(p->GetPosition(), position_);
				if (VSquareSize(diff) < 3000.0f * 3000.0f)
				{
					Master::sound_manager_->PlaySE(SoundManager::kSeTornado);
				}
			}
		}
		effect_timer_ = 360;
	}

	// エフェクトの移動漏れによる、見た目と当たり判定の位置ズレ（同期ズレ）を防止する
	if (tatu_ != nullptr)
	{
		tatu_->SetPosition(position_);
		tatu_->Update();
	}
}

void Tornado::UpdateKnockback()
{
	// 竜巻接触により吹き飛ばされたプレイヤーの減衰（フリクション）およびコリジョン判定処理
	knockbacks_.erase(
		std::remove_if(knockbacks_.begin(), knockbacks_.end(), [](auto& knockback) {
			Player3D* p_knock = knockback.player;
			VECTOR& vel = knockback.velocity;

			VECTOR oldPos = p_knock->GetPosition();
			VECTOR newPos = VAdd(oldPos, vel);

			// バグ回避：ノックバックの勢いでプレイヤーがステージの壁を貫通し、異次元へ落下するのを防ぐ
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
						// 壁衝突時は即座にその位置で慣性エネルギーを失わせ、めり込みを防止する
						newPos = oldPos;
						vel = VGet(0.0f, 0.0f, 0.0f);
						break;
					}
				}
			}

			p_knock->SetPosition(newPos);

			// 物理演算：空気抵抗および地面との摩擦をシミュレートし、ノックバック速度を毎フレーム 10% 減衰させる
			vel = VScale(vel, 0.9f);

			// 移動速度が一定以下になり、ほぼ静止したとみなせる場合は物理演算リストから除外する
			return VSize(vel) < 0.5f;
		}),
		knockbacks_.end()
	);
}

void Tornado::Draw()
{
}

// 入力：collider=自身のカプセル衝突判定, check=相手の衝突判定
// 副作用：接触したプレイヤーの速度ベクトル加算、カメラシェイク演出の発動
void Tornado::OnEnter(Collider* collider, Collider* check)
{
	// テレポートの代わりに、竜巻の中心から外周方向へ力強くプレイヤーを吹き飛ばす物理挙動を処理
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dPlayer)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->parent_object_);

		VECTOR diff = VSub(Player->GetPosition(), position_);
		diff.y = 0.0f;

		// バグ回避：竜巻の完全な中心座標と重なった瞬間の、ゼロベクトル割り算（ゼロ除算）によるNaNバグを防止する
		if (VSize(diff) < 0.1f) {
			diff = VGet(1.0f, 0.0f, 0.0f);
		}

		// 吹き飛ばしの初速ベクトル（水平方向に 300.0f の強度で弾き飛ばす）
		VECTOR knockbackVelocity = VScale(VNorm(diff), 300.0f);

		// 1フレーム中に複数回当たり判定が重複して発生し、過剰な多段ノックバックになるのを防ぐ
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
