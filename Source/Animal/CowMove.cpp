#include "CowMove.h"
#include "GameConstants.h"
#include "Master.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Wall.h"
#include "stage.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include <cmath>
#include "Player3D.h"
#include "Bait.h"
#include "TutorialScene.h"
#include "ServiceLocator.h"

namespace {
	int s_mnTagCountCow = 0;
	CowMove::TagCow s_tag1Cow = CowMove::kNone;
	CowMove::TagCow s_tag2Cow = CowMove::kNone;
	CowMove::TagCow s_tag3Cow = CowMove::kNone;
}

/// @brief 捕獲対象（牛）の基礎ステータス（スコア・XP・吸い込み上限高度）をゲームモードに応じて初期化する。
/// @details 固有パラメータ設定および吸い込み時エフェクトの動的生成を行う。
CowMove::CowMove(const std::string& filename, VECTOR init_pos)
	: CharacterMove(filename, init_pos)
{
	death_timer_ = GameConstants::kCowDefault.death_time_height;
	score_ = GameConstants::kCowDefault.score;
	xp_ = GameConstants::kCowDefault.xp;
	bait_flag_ = false;

	is_visible_ = true;
	effect_timer_ = 0;
	SetTag(Object3D::kTag3dCow);

	if (Master::scene_manager_->GetSceneType() == SceneManager::kSceneTutorial)
	{
		death_timer_ = GameConstants::kCowTutorial.death_time_height;
	}

	cow_vm_ = new EffekseerEffect("Resource/3D/Effect/CowVacuum.efk", position_, 50.0f);
}

CowMove::~CowMove()
{
}

/// @brief オブジェクトプーリングによる再利用時、前回の死亡フラグや吸い込みエフェクトの残価による誤動作を防止する。
/// @details 内部フラグ・エフェクト・コライダー座標の初期化を行う。
void CowMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	mDeleteFlag = false;
	effect_timer_ = 0;
	if (cow_vm_ != nullptr)
	{
		cow_vm_->SetPosition(pos);
		cow_vm_->Stop();
	}
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
	}
}

/// @brief キャラクターの移動・物理演算に加え、非表示状態（吸い込み完了直前）のエフェクト演出を進行させる。
/// @details 基底クラスの更新処理、および吸い込み用エフェクトのフレーム更新を行う。
void CowMove::Update()
{
	CharacterMove::Update();

	if (cow_vm_ != nullptr && is_visible_ == false)
	{
		cow_vm_->Update();
	}
}

/// @brief 生存時は3Dモデルを描画し、吸い込み消滅演出中はモデルを隠して吸い込みエフェクトのみをレンダリングする。
/// @details 可視性フラグに応じたモデル描画および演出エフェクトの描画を行う。
void CowMove::Draw()
{
	if (is_visible_)
	{
		CharacterMove::Draw();
	}
	if (cow_vm_ != nullptr && is_visible_ == false)
	{
		cow_vm_->Draw();
	}
}

/// @brief 牛の身体サイズ・高さに合わせ、判定漏れを防ぐカプセルコライダーの上下空間座標を追従更新する。
/// @details 判定用カプセルコライダーの基準座標・到達点座標・半径を同期する。
void CowMove::ColliderMove()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		capsule_collider_->radius_ = collider_radius_;
	}
}

/// @brief 他の牛との重なり回避、餌への誘導、および通常の徘徊AIを優先度順に合成して位置を決定する。
/// @details 回避・追従・徘徊・壁判定ロジックのディスパッチとモデル座標の同期を行う。
void CowMove::MoveCharacter()
{
	AvoidOtherCows();

	if (SeekBait())
	{
		return;
	}

	CharacterMove::UpdateWanderAI();
	CharacterMove::CheckWallCollision();

	model_->SetPosition(position_);
}

/// @brief 多数の牛が一箇所に重なった際の描画チラつき（Zファイティング）と不自然な密集を押し出し演算で防止する。
/// @details 牛同士の距離計算を行い、近接時に互いを離す反発ベクトルを加算・適用する。
void CowMove::AvoidOtherCows()
{
	const auto& cows = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dCow);
	for (auto& obj : cows)
	{
		CowMove* otherCow = dynamic_cast<CowMove*>(obj);
		if (otherCow != nullptr && otherCow != this)
		{
			VECTOR otherPos = otherCow->GetPosition();
			VECTOR diff = VSub(position_, otherPos);
			diff.y = 0.0f;

			float distSq = VSquareSize(diff);
			float minDist = 50.0f;

			if (distSq < minDist * minDist)
			{
				VECTOR dir_ = diff;
				if (distSq < 0.001f)
				{
					dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
					if (VSquareSize(dir_) < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
				}
				dir_ = VNorm(dir_);
				position_ = VAdd(position_, VScale(dir_, 15.0f * Master::GetDeltaTimeScaler()));
			}
		}
	}
	model_->SetPosition(position_);
}

/// @brief 設置された餌の匂い（範囲判定）を検知した際、通常の徘徊を中断して最寄りの餌へ直線移動させる。
/// @details チュートリアル中やUFO吸い込み中のスタックを回避しつつ座標を更新する。
bool CowMove::SeekBait()
{
	if (Master::scene_manager_->GetSceneType() == SceneManager::kSceneTutorial) return false;
	if (ai_state_ == kStateVacuum) return false;

	old_position_ = position_;

	if (bait_flag_)
	{
		const auto& b = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dBait);
		if (!b.empty())
		{
			Bait* bait = dynamic_cast<Bait*>(b.at(0));
			if (bait)
			{
				VECTOR diff = VSub(bait->GetPosition(), position_);
				float distSq = VSquareSize(diff);

				if (distSq > 100.0f)
				{
					position_ = VAdd(position_, VScale(VNorm(diff), 25.0f * Master::GetDeltaTimeScaler()));
				}
				else
				{
					position_ = bait->GetPosition();
				}
			}
		}
		model_->SetPosition(position_);
		return true;
	}
	return false;
}

/// @brief ランダムな徘徊行動の目的地選定および移動ベクトルの算出を行う。
/// @details 基底クラスの徘徊AIロジックを実行する。
void CowMove::UpdateWanderAI()
{
	CharacterMove::UpdateWanderAI();
}

/// @brief ステージ外への脱出を防ぐため、境界壁との接触・めり込み防止処理を行う。
/// @details 基底クラスの壁衝突判定処理を実行する。
void CowMove::CheckWallCollision()
{
	CharacterMove::CheckWallCollision();
}

/// @brief 移動方向ベクトルに合わせてモデルのY軸回転角度をイージング補間する。
/// @details 基底クラスの回転補間処理を実行する。
void CowMove::RotationCharacter()
{
	CharacterMove::RotationCharacter();
}

/// @brief 物理演算・移動結果に伴うモデルの回転行列を再計算して反映する。
/// @details 基底クラスのモデル回転適用処理を実行する。
void CowMove::CharacterRotate()
{
	CharacterMove::CharacterRotate();
}

/// @brief アニメーション追加インターフェース（個別アニメーション追加が必要な場合にオーバーライド）。
/// @details 処理なし（基底クラスの仕様に準拠）。
void CowMove::AddAnimation(AnimationState state, const std::string& filename)
{
}

/// @brief 餌の感知範囲への侵入や、他の牛との物理的な重なり検知時の初期衝突解決を行う。
/// @details 餌フラグの有効化、または牛同士の急激な離脱ベクトル加算を行う。
void CowMove::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			bait_flag_ = true;
		}

		if (check->parent_object_->GetTag() == kTag3dCow)
		{
			VECTOR otherPos = check->parent_object_->GetPosition();
			VECTOR dir_ = VSub(position_, otherPos);
			dir_.y = 0.0f;

			float lengthSq = VSquareSize(dir_);
			if (lengthSq < 0.001f)
			{
				dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				lengthSq = VSquareSize(dir_);
				if (lengthSq < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
			}

			dir_ = VNorm(dir_);
			position_ = VAdd(position_, VScale(dir_, 3.0f * Master::GetDeltaTimeScaler()));
		}
	}
}

/// @brief 牛同士がめり込み続けている間、反発力を継続的に加算して物理スタックを解除する。
/// @details 重なり解消のための押し出し移動ベクトルを適用する。
void CowMove::OnTrigger(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dCow)
		{
			VECTOR otherPos = check->parent_object_->GetPosition();
			VECTOR dir_ = VSub(position_, otherPos);
			dir_.y = 0.0f;

			float lengthSq = VSquareSize(dir_);
			if (lengthSq < 0.001f)
			{
				dir_ = VGet((float)(GetRand(100) - 50), 0.0f, (float)(GetRand(100) - 50));
				lengthSq = VSquareSize(dir_);
				if (lengthSq < 0.001f) dir_ = VGet(1.0f, 0.0f, 0.0f);
			}

			dir_ = VNorm(dir_);
			position_ = VAdd(position_, VScale(dir_, 3.0f * Master::GetDeltaTimeScaler()));
		}
	}
}

/// @brief 餌の感知範囲から完全に外れた際、餌追従フラグを降ろして通常の徘徊へ戻す。
/// @details 餌ターゲットフラグ（bait_flag_）を無効化する。
void CowMove::OnExit(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_ != nullptr)
	{
		if (check->parent_object_->GetTag() == kTag3dBait)
		{
			bait_flag_ = false;
		}
	}
}

/// @brief UFOの光線による吸引上昇アニメーション、フィーバー時の追従、および規定高度到達時のキャプチャ完了処理。
/// @details 上昇運動・UFO直下補正・高度判定に基づく消滅およびDie()関数の呼び出しを行う。
void CowMove::CharacterDied()
{
	if (ai_state_ == kStateVacuum)
	{
		Player3D* player = target_player_;

		CharacterRotate();
		if (player != nullptr)
		{
			position_.y += player->Status(Player3D::kStatusAttackS) * Master::GetDeltaTimeScaler();

			if (Master::FeverFlag)
			{
				float followSpeed = 0.15f;
				VECTOR playerPos = player->GetPosition();
				position_.x += (playerPos.x - position_.x) * followSpeed;
				position_.z += (playerPos.z - position_.z) * followSpeed;
			}
		}
		model_->SetPosition(position_);

		if (cow_vm_ != nullptr)
		{
			if (position_.y > death_timer_)
			{
				if (effect_timer_ <= 0 && is_visible_ == true)
				{
					effect_timer_ = 60;
					is_visible_ = false;
					capsule_collider_->SetDeleteFlag(true);
				}

				if (!is_visible_)
				{
					effect_timer_--;
				}

				if (effect_timer_ <= 0 && !is_visible_)
				{
					Die(kDeathVacuum);
				}
			}
		}
		else
		{
			if (position_.y > death_timer_)
			{
				Die(kDeathVacuum);
			}
		}
	}
}

/// @brief 餌の捕食アクション等により即座に消滅・回収された際の死亡処理をトリガーする。
/// @details モデル消滅フラグの設定、餌死亡理由でのDie()呼び出し、破棄フラグ付与を行う。
void CowMove::KilledByBait()
{
	is_visible_ = false;
	Die(kDeathBait);
	mDeleteFlag = true;
}

/// @brief 牛のキャプチャ完了時、プレイヤーへスコア・XP・コンボ数を付与し、SEおよび爆発エフェクトを再生する。
/// @details プレイヤーリソース（Score/XP/Combo）の加算と削除予約フラグの設定を行う。
void CowMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	Player3D* player = target_player_;

	switch (reason)
	{
	case kDeathVacuum:
		Master::sound_manager_->PlaySE(SoundManager::kSeCow);
		Master::effect_manager_->PlayCowDeathEffect(position_);
		if (player != nullptr)
		{
			player->level_manager_->AddXp(xp_);
			player->combo_->AddHit();
			player->score_manager_->AddScore(static_cast<int>(score_ * player->combo_->GetMultiplier()));

			if (tag_cow_ == CowMove::TagCow::kCowT)
			{
				Master::tutorial_count_++;
			}

			s_mnTagCountCow++;
			if (s_mnTagCountCow == 1)
			{
				s_tag1Cow = tag_cow_;
			}
			else if (s_mnTagCountCow == 2 && s_tag1Cow == tag_cow_)
			{
				s_tag2Cow = tag_cow_;
			}
			else if (s_mnTagCountCow == 3 && s_tag2Cow == tag_cow_)
			{
				s_tag3Cow = tag_cow_;
				if (s_tag3Cow == CowMove::kCow1) player->score_manager_->AddScore(300);
				if (s_tag2Cow == CowMove::kCow2) player->score_manager_->AddScore(600);
			}
			else
			{
				s_mnTagCountCow = 0;
				s_tag1Cow = CowMove::kNone;
				s_tag2Cow = CowMove::kNone;
				s_tag3Cow = CowMove::kNone;
			}
		}
		mDeleteFlag = true;
		break;

	case kDeathBait:
		Master::sound_manager_->PlaySE(SoundManager::kSeCow);
		Master::effect_manager_->PlayCowDeathEffect(position_);
		if (player != nullptr)
		{
			player->level_manager_->AddXp(xp_);
			player->combo_->AddHit();
			player->score_manager_->AddScore(static_cast<int>(score_ * player->combo_->GetMultiplier()));
		}
		mDeleteFlag = true;
		break;

	case kDeathLimit:
		mDeleteFlag = true;
		break;
	}
}