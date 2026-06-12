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
	
	// 各進行ステップデータの定義と登録
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
 * @brief 次のステップ（終了画面等）へ状態遷移させ、スコアランキング登録を行う
 * [入力] type: 遷移先のステップID
 * [出力] なし
 * [副作用] mnTypeの更新、Master::GameFinishFlagの有効化、スコアの追加・ネーム入力の開始
 */
void GameManager::GameNextStep(GameStepType type)
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	
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
			// プレイ中スコアをマスターに登録し、ランキングへ反映
			Master::mpScore->AddScore(player->mpScore->GetScore());
			Master::mpScore->SetResultScore(player->mpScore->GetScore());
			player->mpScore->AddRanking();

			// 名前入力はスキップして自動セーブ
			player->mpScore->Save();
			player->mpScore->SaveRanking();
		}

		// リザルト画面へのフェードアウトを開始
		Master::mpSceneManager->GetCurrentScene()->mFadeState = Scene::SceneFade_Out;
		Master::mpSceneManager->GetCurrentScene()->mNextScene = SceneManager::SCENE_RESULT;

		mnType = type;
	}
}

/*
 * @brief 開始時のフェードイン暗転矩形を画面に描画する
 * [入力] なし
 * [出力] なし
 * [副作用] 画面全域へDrawBoxによる黒塗り描画
 */
void GameManager::Draw()
{
	if (Fadeflag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(Fadetimer));
		// ウィンドウ全画面を綺麗にカバーするように Utility 定数の解像度を使用
		DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		Fadetimer -= 2.0f; // 徐々に明るくする

		if (Fadetimer <= 0.0f)
		{
			Fadetimer = 0.0f;
			Fadeflag = false;
		}
	}
}

void GameManager::Update()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	// タイムアップ後のリザルト遷移待ち処理
	if (GameStepType::game_final == mnType)
	{
		// GameNextStepでフェードアウト設定済みなため、ここでは何もしない
	}
	
	// ゲーム本編進行中：制限時間タイマー管理およびランダムフェーズ切り替え処理
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

		// 1000ms（1秒）ごとにカウンターを1進める
		if (Timer - m_PhaseTimer >= 1000)
		{
			m_PhaseTimer = Timer;
			m_PhaseChangeCount++;
		}
		
		// 30秒ごとにゲームのフェーズ（演出）をランダムに変更する
		if (m_PhaseChangeCount >= 30)
		{
			m_PhaseChangeCount = 0;
			int m_Num = rand() % 2 + 1; // 1〜2の乱数を取得
			
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
