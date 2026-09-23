#include "ServiceLocator.h"
#include "Fever.h"
#include "GoldCow.h"
#include "CowManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "Cow.h"
#include "Utility.h"
#include "GameConstants.h"

/// @brief 未定義状態によるバグを防ぐため、メンバ変数を初期化する
/// @details 各種メンバ変数が初期値に設定される
Fever::Fever()
	: drop_time_(0)
	, drop_count_(0)
	, fever_player_(nullptr)
{
	gauge_ = 0;
	timer_ = 0;
	is_fever_ = false;
}

/// @brief インスタンス破棄時のリソース漏れを防ぐため
Fever::~Fever()
{
}

/// @brief 今後の拡張でフィーバーゲージを任意増加させるために用意
/// @param value 増加量
void Fever::AddGauge(int value)
{
}

/// @brief スコア獲得のチャンスを作るため、プレイヤーを強化しフィーバーを開始する
/// @param player 対象のプレイヤー
/// @details プレイヤーの攻撃力が倍増し、フィーバー関連の変数が初期化される
void Fever::StartFever(Player3D* player)
{
	if (player == nullptr) return;
	fever_player_ = player;
	player_status_ = player->GetStatusAttack();
	player->SetStatusAttack(player_status_ * 2.0f);
	is_fever_ = true;
	timer_ = 600;
	drop_count_ = 0;
	drop_time_ = 60;
	Master::FeverFlag = true;
}

/// @brief フィーバー終了後にゲーム進行が停滞しないよう、元の状態に戻しつつ獲物を補充する
/// @details プレイヤーの攻撃力が元に戻り、ステージに初期配置の動物が生成される
void Fever::EndFever()
{
	if (fever_player_ != nullptr)
	{
		fever_player_->SetStatusAttack(player_status_);
		fever_player_ = nullptr;
	}
	is_fever_ = false;

	VECTOR spawnPos = VGet(0.0f, 0.0f, 0.0f);
	float scatterArea = Utility::StageSize.x;
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2, false, 500.0f);
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCow1, 20, false, scatterArea);
	ServiceLocator::GetAnimalManager()->SpawnAnimal(GameConstants::kAnimalSheep.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5, scatterArea);
	Master::FeverFlag = false;
}

/// @brief フィーバー状態の進行と、ボーナス要素の定期生成を行うため
/// @details 画面にエフェクトが描画され、タイマーが減少し、金の牛が生成される
void Fever::Update()
{
	if (!is_fever_) return;

	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);

	DrawBox(0, 0, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(255, 200, 50), FALSE);
	DrawBox(1, 1, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(255, 220, 100), FALSE);
	DrawBox(2, 2, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(255, 255, 180), FALSE);

	for (int i = 0; i < 30; i++)
	{
		DrawBox(
			i,
			i,
			Utility::kScreenWidth - i,
			Utility::kScreenHeight - i,
			GetColor(255, 200, 50),
			FALSE
		);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	drop_count_++;
	timer_--;

	if (drop_count_ > drop_time_)
	{
		drop_count_ = 0;
		VECTOR spawnPos = VGet(0.0f, 0.0f, 0.0f);
		ServiceLocator::GetCowManager()->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2, true, 500.0f);
	}

	if (timer_ <= 0)
	{
		EndFever();
	}
}

/// @brief 外部からフィーバー中かどうか判定し、専用の処理を分岐させるため
/// @return フィーバー中ならtrue
bool Fever::IsFever() const {
	return is_fever_;
}
