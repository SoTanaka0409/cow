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
	, currentPhase(GamePhase::Normal)
	, type(GameStepType::game_CowGet)
{
	gameTimer = nullptr;
	
	auto data = new GameStepData;
	data->type = GameStepType::game_CowGet;
	data->trueFlag = true;
	gameStepDataList.push_back(data);

	data = new GameStepData;
	data->type = GameStepType::game_final;
	data->trueFlag = true;
	gameStepDataList.push_back(data);

	phaseTimer = GetNowCount();
	phaseChangeCount = 0;
}

GameManager::~GameManager()
{
	for (auto data : gameStepDataList)
	{
		delete data;
	}
	gameStepDataList.clear();

	if (gameTimer != nullptr)
	{
		delete gameTimer;
		gameTimer = nullptr;
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
		type = type;
	}
	if (GameStepType::game_final == type)
	{
		Master::gameFinishFlag = true;
		if (player != nullptr)
		{
			Master::score->AddScore(player->score->GetScore());
			Master::score->SetResultScore(player->score->GetScore());
			player->score->AddRanking();

			// 名前入力はスキップして自動セーブする仕様のため
			player->score->Save();
			player->score->SaveRanking();
		}

		// リザルト画面へのフェードアウトを開始する
		Master::sceneManager->GetCurrentScene()->fadeState = Scene::SceneFade_Out;
		Master::sceneManager->GetCurrentScene()->nextScene = SceneManager::SCENE_RESULT;

		type = type;
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
	
	if (GameStepType::game_final == type)
	{
	}
	
	// 制限時間タイマー管理およびランダムフェーズ切り替え処理
	if (GameStepType::game_CowGet == type)
	{
		if (!gameTimer)
		{
			gameTimer = new GameTimer(VGet(0, 0, 0), 60, GameTimer::Tag_Game);
		}

		if (gameTimer)
		{
			if (gameTimer->OutTimerFlag())
			{
				gameTimer->SetOutTimerFlag(false);
				GameNextStep(GameManager::game_final);
			}
			else
			{
				gameTimer->Update();
			}
		}

		int Timer = GetNowCount();

		if (Timer - phaseTimer >= 1000)
		{
			phaseTimer = Timer;
			phaseChangeCount++;
		}
		
		// 30秒ごとにゲームのフェーズ（演出）をランダムに変更する制約
		if (phaseChangeCount >= 30)
		{
			phaseChangeCount = 0;
			int num = rand() % 2 + 1;
			
			if (num == 1)
			{
				currentPhase = GamePhase::TornadoCrisis;
			}
			else if (num == 2)
			{
				currentPhase = GamePhase::MassSpawn;
			}
			else
			{
				currentPhase = GamePhase::Normal;
			}
		}
	}
}
