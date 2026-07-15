#include "ServiceLocator.h"
#include"GameManager.h"
#include"Master.h"
#include"SceneManager.h"
#include"Player3D.h"
#include"ObjectManager.h"
#include"InputManager.h"
#include"GameTimer.h"
#include"Utility.h"
GameManager::GameManager()
	: fade_timer_(300.0f)
	, fade_flag_(true)
	, current_phase_(GamePhase::kNormal)
	, type_(GameStepType::kCowGet)
{
	game_timer_ = nullptr;
	
	auto data = new GameStepData;
	data->type = GameStepType::kCowGet;
	data->TrueFlag = true;
	data_.push_back(data);
	data = new GameStepData;
	data->type = GameStepType::kFinal;
	data->TrueFlag = true;
	data_.push_back(data);
	phase_timer_ = GetNowCount();
	phase_change_count_ = 0;
}
GameManager::~GameManager()
{
	for (auto data : data_)
	{
		delete data;
	}
	data_.clear();
	if (game_timer_ != nullptr)
	{
		delete game_timer_;
		game_timer_ = nullptr;
	}
}
/*
 * 指定した進行ステップへの移行処理を統一するため
 * [入力] type: 遷移先のステップタイプ
 * [出力] なし
 * [副作用] 進行ステップ変更、リザルト画面への遷移フラグ設定など
 */
void GameManager::GameNextStep(GameStepType type)
{
	Player3D* player = ServiceLocator::GetPlayer();
	
	if (GameStepType::kCowGet == type)
	{
		fade_flag_ = true;
		type_ = type;
	}
	if (GameStepType::kFinal == type)
	{
		Master::GameFinishFlag = true;
		if (player != nullptr)
		{
			Master::mpScore->AddScore(player->mpScore->GetScore());
			Master::mpScore->SetResultScore(player->mpScore->GetScore());
			player->mpScore->AddRanking();
			// 不測の事態に備え、リザルト移行前に進行状況を保存しておくため
			player->mpScore->Save();
			player->mpScore->SaveRanking();
		}
		// ゲーム終了に伴い、次シーンへの移行を促すため
		if (auto scene = Master::mpSceneManager->GetCurrentScene())
		{
			scene->fade_state_ = Scene::kSceneFadeOut;
			scene->next_scene_ = SceneManager::kSceneResult;
		}
		type_ = type;
	}
}
/*
 * ゲーム開始時の暗転演出を描画するため
 * [入力] なし
 * [出力] なし
 * [副作用] 画面全域に黒い矩形を描画
 */
void GameManager::Draw()
{
	if (fade_flag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fade_timer_));
		DrawBox(0, 0, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		fade_timer_ -= 2.0f;
		if (fade_timer_ <= 0.0f)
		{
			fade_timer_ = 0.0f;
			fade_flag_ = false;
		}
	}
}
/*
 * 制限時間やフェーズ遷移など時間経過に伴う状態更新を行うため
 * [入力] なし
 * [出力] なし
 * [副作用] タイマーの更新、ランダムなフェーズ切り替え
 */
void GameManager::Update()
{
	Player3D* player = ServiceLocator::GetPlayer();
	
	if (GameStepType::kFinal == type_)
	{
	}
	
	if (GameStepType::kCowGet == type_)
	{
		UpdateGameTimerAndPhase();
	}
}

void GameManager::UpdateGameTimerAndPhase()
{
	if (!game_timer_)
	{
		game_timer_ = new GameTimer(VGet(0, 0, 0), 60, GameTimer::Tag_Game);
	}
	if (game_timer_)
	{
		if (game_timer_->OutTimerFlag())
		{
			game_timer_->SetOutTimerFlag(false);
			GameNextStep(GameManager::kFinal);
		}
		else
		{
			game_timer_->Update();
		}
	}
	int Timer = GetNowCount();
	if (Timer - phase_timer_ >= 1000)
	{
		phase_timer_ = Timer;
		phase_change_count_++;
	}
	
	// プレイヤーに変化を楽しんでもらうため
	if (phase_change_count_ >= 30)
	{
		phase_change_count_ = 0;
		int m_Num = rand() % 2 + 1;
		
		if (m_Num == 1)
		{
			current_phase_ = GamePhase::kTornadoCrisis;
		}
		else if (m_Num == 2)
		{
			current_phase_ = GamePhase::kMassSpawn;
		}
		else
		{
			current_phase_ = GamePhase::kNormal;
		}
	}
}