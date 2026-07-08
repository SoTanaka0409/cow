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
	: Fadetimer(300.0f)
	, Fadeflag(true)
	, mCurrentPhase(GamePhase::Normal)
	, mnType(GameStepType::game_CowGet)
{
	mpGameTimer = nullptr;
	
	auto data = new GameStepData;
	data->type = GameStepType::game_CowGet;
	data->TrueFlag = true;
	mData.push_back(data);

	data = new GameStepData;
	data->type = GameStepType::game_final;
	data->TrueFlag = true;
	mData.push_back(data);

	m_PhaseTimer = GetNowCount();
	m_PhaseChangeCount = 0;
}

GameManager::~GameManager()
{
	for (auto data : mData)
	{
		delete data;
	}
	mData.clear();

	if (mpGameTimer != nullptr)
	{
		delete mpGameTimer;
		mpGameTimer = nullptr;
	}
}

/*
 * スコア登録を伴う終了ステップへの遷移を行う
 * [入力] type: 遷移先のステップID
 * [出力] なし
 * [副作用] 進行ステップ変更、フラグ更新、ネーム入力開始
 */
void GameManager::GameNextStep(GameStepType type)
{
	Player3D* player = ServiceLocator::GetPlayer();
	
	if (GameStepType::game_CowGet == type)
	{
		Fadeflag = true;
		mnType = type;
	}
	if (GameStepType::game_final == type)
	{
		Master::GameFinishFlag = true;
		if (player != nullptr)
		{
			Master::mpScore->AddScore(player->mpScore->GetScore());
			Master::mpScore->SetResultScore(player->mpScore->GetScore());
			player->mpScore->AddRanking();

			// 名前入力はスキップして自動セーブする仕様のため
			player->mpScore->Save();
			player->mpScore->SaveRanking();
		}

		// リザルト画面へのフェードアウトを開始する
		if (auto scene = Master::mpSceneManager->GetCurrentScene())
		{
			scene->mFadeState = Scene::SceneFade_Out;
			scene->mNextScene = SceneManager::SCENE_RESULT;
		}

		mnType = type;
	}
}

/*
 * フェードイン等、ゲーム進行に必要な演出描画を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 画面全域へDrawBoxによる黒塗り描画
 */
void GameManager::Draw()
{
	if (Fadeflag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(Fadetimer));
		// ウィンドウ全画面をカバーするため Utility 定数の解像度を使用する
		DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		Fadetimer -= 2.0f;

		if (Fadetimer <= 0.0f)
		{
			Fadetimer = 0.0f;
			Fadeflag = false;
		}
	}
}

void GameManager::Update()
{
	Player3D* player = ServiceLocator::GetPlayer();
	
	if (GameStepType::game_final == mnType)
	{
	}
	
	// 制限時間タイマー管理およびランダムフェーズ切り替え処理
	if (GameStepType::game_CowGet == mnType)
	{
		if (!mpGameTimer)
		{
			mpGameTimer = new GameTimer(VGet(0, 0, 0), 60, GameTimer::Tag_Game);
		}

		if (mpGameTimer)
		{
			if (mpGameTimer->OutTimerFlag())
			{
				mpGameTimer->SetOutTimerFlag(false);
				GameNextStep(GameManager::game_final);
			}
			else
			{
				mpGameTimer->Update();
			}
		}

		int Timer = GetNowCount();

		if (Timer - m_PhaseTimer >= 1000)
		{
			m_PhaseTimer = Timer;
			m_PhaseChangeCount++;
		}
		
		// 30秒ごとにゲームのフェーズ（演出）をランダムに変更する制約
		if (m_PhaseChangeCount >= 30)
		{
			m_PhaseChangeCount = 0;
			int m_Num = rand() % 2 + 1;
			
			if (m_Num == 1)
			{
				mCurrentPhase = GamePhase::TornadoCrisis;
			}
			else if (m_Num == 2)
			{
				mCurrentPhase = GamePhase::MassSpawn;
			}
			else
			{
				mCurrentPhase = GamePhase::Normal;
			}
		}
	}
}
