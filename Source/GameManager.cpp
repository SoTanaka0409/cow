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
 * 繧�E�繧�E�繧�E�逋ｻ骭�E�繧剁E���E�縺・�E�めE��・せ繝�Eャ繝励∈縺�E�驕ｷ遘ｻ繧定｡後≧
 * [蜈･蜉嫁Etype: 驕ｷ遘ｻ蜈医・繧�E�繝�Eャ繝悠D
 * [蜁E��蜉嫁E縺�E�縺・
 * [蜑ｯ菴懁E��] 騾�E�陦後せ繝�Eャ繝怜､画峩縲√ヵ繝ｩ繧�E�譖ｴ譁E��縲√ロ繝ｼ繝蜈･蜉幁E��蟋・
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

			// 蜷榊��蜈･蜉帙�E繧�E�繧�E�繝�E・縺励※閾�E�蜍輔そ繝ｼ繝悶☁E��倶�E�墓ｧ倥・縺溘ａE
			player->mpScore->Save();
			player->mpScore->SaveRanking();
		}

		// 繝ｪ繧�E�繝ｫ繝育判髱�E�縺�E�縺�E�繝輔ぉ繝ｼ繝峨ぁE���E�繝医�E�髢句�E�九�E繧・
		if (auto scene = Master::mpSceneManager->GetCurrentScene())
		{
			scene->fade_state_ = Scene::kSceneFadeOut;
			scene->next_scene_ = SceneManager::kSceneResult;
		}

		type_ = type;
	}
}

/*
 * 繝輔ぉ繝ｼ繝峨ぁE��ｳ遲峨√ご繝ｼ繝騾�E�陦後�E蠢・�E�√�E貍泌�E謠冗判繧定｡後≧
 * [蜈･蜉嫁E縺�E�縺・
 * [蜁E��蜉嫁E縺�E�縺・
 * [蜑ｯ菴懁E��] 逕ｻ髱�E�蜈ｨ蝓溘�EDrawBox縺�E�繧医�E�鮟貞｡励�E�謠冗判
 */
void GameManager::Draw()
{
	if (fade_flag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fade_timer_));
		// 繧�E�繧�E�繝ｳ繝峨え蜈�E�逕ｻ髱�E�繧偵き繝�E・縺吶�E�縺溘ａEUtility 螳壽焚縺�E�隗｣蜒丞ｺ�E�繧剁E���E�逕ｨ縺吶�E�E
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

void GameManager::Update()
{
	Player3D* player = ServiceLocator::GetPlayer();
	
	if (GameStepType::kFinal == type_)
	{
	}
	
	// 蛻�E�髯先�E髢薙ち繧�E�繝槭・邂｡送E�E♀繧医・繝ｩ繝ｳ繝繝繝輔ぉ繝ｼ繧�E�蛻・�E�譖ｿ縺亥・送E�E
	if (GameStepType::kCowGet == type_)
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
		
		// 30遘�E�E�E���E�縺�E�繧�E�繝ｼ繝縺�E�繝輔ぉ繝ｼ繧�E�・域ｼ泌�E・峨�E�繝ｩ繝ｳ繝繝縺�E�螟画峩縺吶�E�蛻�E�邏�E
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
}