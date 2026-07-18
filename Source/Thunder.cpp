#include "ServiceLocator.h"
#include "Thunder.h"
#include <cmath>
#include "CapsuleCollider.h"
#include "dxlib.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

// 入力：落雷の初期中心座標
// 副作用：EffekseerEffect インスタンスの動的メモリ確保
Thunder::Thunder(VECTOR pos)
	: Object3D(pos)
{
	has_stunned_ = false;
	pos_ = pos;
	// プレイヤーが予兆を見てから回避行動をとれるよう、落下前に1秒間の猶予を設ける
	warning_timer_ = 60;
	strike_timer_ = 20;
	interval_timer_ = 180;

	state_ = kIdle;
	active_ = true;

	capsule_collider_->radius_ = 230;

	thunder_ = new EffekseerEffect("Resource/3D/EFK/thud.efk", pos_, 200.0f);
	thunder_->SetScale(VGet(1.0f, 1.0f, 1.0f));

	warning_ = new EffekseerEffect("Resource/3D/EFK/warning2.efk", pos_, 40.0f);
	stun_ = new EffekseerEffect("Resource/3D/EFK/stun.efk", pos_, 20.0f);
}

// 副作用：動的確保したエフェクトインスタンスのメモリ解放
Thunder::~Thunder()
{
	// 各エフェクトはマネージャー登録せず個別に生存管理しているため、手動で解放する
	if (thunder_ != nullptr)
	{
		delete thunder_;
		thunder_ = nullptr;
	}

	if (warning_ != nullptr)
	{
		delete warning_;
		warning_ = nullptr;
	}

	if (stun_ != nullptr)
	{
		delete stun_;
		stun_ = nullptr;
	}
}

// 副作用：各種タイマーの更新、エフェクトの再生、SEの再生
void Thunder::Update()
{
	UpdateEffects();
	if (!active_) return;
	UpdateCollider();
	UpdateState();
	UpdateStunEffect();
}

void Thunder::UpdateEffects()
{
	if (thunder_ != nullptr)
	{
		thunder_->SetPosition(pos_);
		thunder_->Update();
	}

	if (warning_ != nullptr)
	{
		warning_->SetPosition(pos_);
		warning_->Update();
	}

	if (stun_ != nullptr)
	{
		stun_->Update();
	}
}

void Thunder::UpdateCollider()
{
	// 上空から地面を貫く形にカプセル形状を上空へ伸ばす
	capsule_collider_->position_ = VSub(position_, VGet(0, 2000, 0));
	capsule_collider_->position2_ = VAdd(position_, VGet(0, 2000, 0));
}

void Thunder::UpdateState()
{
	switch (state_)
	{
	case kIdle:
		interval_timer_--;
		if (interval_timer_ <= 0)
		{
			// ステージの中心（0,0）から3000の範囲内にランダムで位置決めする
			float range = 3000.0f;
			pos_.x = (float)(GetRand((int)range * 2) - (int)range);
			pos_.z = (float)(GetRand((int)range * 2) - (int)range);
			pos_.y = 0.0f;

			position_ = pos_;
			warning_timer_ = 60;
			state_ = kWarning;

			if (warning_ != nullptr)
			{
				warning_->Play();
			}
		}
		break;

	case kWarning:
		warning_timer_--;
		if (warning_timer_ <= 0)
		{
			state_ = kStrike;
			strike_timer_ = 30;
			has_stunned_ = false;

			if (thunder_ != nullptr)
			{
				thunder_->Play();

				// 距離が遠すぎる音を鳴らさないよう、プレイヤーが近い場合のみ鳴らす
				auto players = ServiceLocator::GetPlayers();
				bool playSound = false;
				for (auto p : players)
				{
					VECTOR diff = VSub(p->GetPosition(), pos_);
					if (VSquareSize(diff) < 3000.0f * 3000.0f)
					{
						playSound = true;
						break;
					}
				}
				if (playSound)
				{
					Master::sound_manager_->PlaySE(SoundManager::kSeKaminari);
				}
			}
		}
		break;

	case kStrike:
		strike_timer_--;
		if (strike_timer_ <= 0)
		{
			interval_timer_= 120;
			state_ = kIdle;
		}
		break;
	}
}

void Thunder::UpdateStunEffect()
{
	if (stun_effect_timer_ > 0)
	{
		stun_effect_timer_--;

		// スタンエフェクトの単発再生を避け、毎秒(30フレーム)でループ再生
		if (stun_effect_timer_ > 0 && stun_effect_timer_ % 30 == 0)
		{
			if (stun_ != nullptr)
			{
				stun_->Play();
			}
		}
	}
}

void Thunder::Draw()
{
}

// 出力：オブジェクトの稼働状態（生存フラグ）
bool Thunder::IsActive() const
{
	return active_;
}

// 入力：playerPos=判定対象の座標, range=判定半径
// 出力：衝突しているかどうかの真偽値
bool Thunder::CheckHit(VECTOR playerPos, float range)
{
	if (state_ != kStrike) return false;

	VECTOR diff = VSub(playerPos, pos_);
	float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

	return distance < range;
}

// 入力：collider=自身の衝突判定, check=相手の衝突判定
// 副作用：プレイヤーのスタン状態遷移、カメラシェイクの起動
void Thunder::OnEnter(Collider* collider, Collider* check)
{
	if (state_ != kStrike) return;
	// 1回の落雷フレーム中に多段ヒットしてスタン期間が意図せず延長されるのを防ぐ
	if (has_stunned_) return;

	if (check->parent_object_->GetTag() == kTag3dPlayer)
	{
		Player3D* player = dynamic_cast<Player3D*>(check->parent_object_);
		if (player != nullptr)
		{
			has_stunned_ = true;
			stun_effect_timer_ = 120;

			VECTOR playerPos = player->GetPosition();
			stun_->SetPosition(playerPos);
			stun_->Play();

			player->ApplyStun(120);

			// 落雷直撃の衝撃を視覚的に強調し、危機的な被弾状況をプレイヤーに伝える
			Master::camera_->SetupShake(30.0f, 45.0f, 40.0f);
		}
	}
}

void Thunder::OnTrigger(Collider* collider, Collider* check)
{
}

void Thunder::OnExit(Collider* collider, Collider* check)
{
}
