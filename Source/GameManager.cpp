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
	game_timer_ = nullptr;
	
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

	if (game_timer_ != nullptr)
	{
		delete game_timer_;
		game_timer_ = nullptr;
	}
}

/*
 * 郢ｧ・ｹ郢ｧ・ｳ郢ｧ・｢騾具ｽｻ鬪ｭ・ｲ郢ｧ蜑・ｽｼ・ｴ邵ｺ繝ｻ・ｵ繧・ｽｺ繝ｻ縺帷ｹ昴・繝｣郢晏干竏育ｸｺ・ｮ鬩包ｽｷ驕假ｽｻ郢ｧ螳夲ｽ｡蠕娯鴬
 * [陷茨ｽ･陷牙ｫ・type: 鬩包ｽｷ驕假ｽｻ陷亥現繝ｻ郢ｧ・ｹ郢昴・繝｣郢晄あD
 * [陷・ｽｺ陷牙ｫ・邵ｺ・ｪ邵ｺ繝ｻ
 * [陷托ｽｯ闖ｴ諛・舞] 鬨ｾ・ｲ髯ｦ蠕後○郢昴・繝｣郢晄懶ｽ､逕ｻ蟲ｩ邵ｲ竏壹Ψ郢晢ｽｩ郢ｧ・ｰ隴厄ｽｴ隴・ｽｰ邵ｲ竏壹Ο郢晢ｽｼ郢晢｣ｰ陷茨ｽ･陷牙ｹ・ｹ戊沂繝ｻ
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

			// 陷ｷ讎顔√陷茨ｽ･陷牙ｸ吶・郢ｧ・ｹ郢ｧ・ｭ郢昴・繝ｻ邵ｺ蜉ｱ窶ｻ髢ｾ・ｪ陷崎ｼ斐◎郢晢ｽｼ郢晄じ笘・ｹｧ蛟ｶ・ｻ蠅難ｽｧ蛟･繝ｻ邵ｺ貅假ｽ・
			player->mpScore->Save();
			player->mpScore->SaveRanking();
		}

		// 郢晢ｽｪ郢ｧ・ｶ郢晢ｽｫ郢晁ご蛻､鬮ｱ・｢邵ｺ・ｸ邵ｺ・ｮ郢晁ｼ斐♂郢晢ｽｼ郢晏ｳｨ縺・ｹｧ・ｦ郢晏現・帝ｫ｢蜿･・ｧ荵昶・郢ｧ繝ｻ
		if (auto scene = Master::mpSceneManager->GetCurrentScene())
		{
			scene->mFadeState = Scene::SceneFade_Out;
			scene->mNextScene = SceneManager::SCENE_RESULT;
		}

		mnType = type;
	}
}

/*
 * 郢晁ｼ斐♂郢晢ｽｼ郢晏ｳｨ縺・ｹ晢ｽｳ驕ｲ蟲ｨﾂ竏壹＃郢晢ｽｼ郢晢｣ｰ鬨ｾ・ｲ髯ｦ蠕娯・陟｢繝ｻ・ｦ竏壺・雋肴ｳ後・隰蜀怜愛郢ｧ螳夲ｽ｡蠕娯鴬
 * [陷茨ｽ･陷牙ｫ・邵ｺ・ｪ邵ｺ繝ｻ
 * [陷・ｽｺ陷牙ｫ・邵ｺ・ｪ邵ｺ繝ｻ
 * [陷托ｽｯ闖ｴ諛・舞] 騾包ｽｻ鬮ｱ・｢陷茨ｽｨ陜捺ｺ倪・DrawBox邵ｺ・ｫ郢ｧ蛹ｻ・矩ｮ溯ｲ橸ｽ｡蜉ｱ・願ｬ蜀怜愛
 */
void GameManager::Draw()
{
	if (Fadeflag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(Fadetimer));
		// 郢ｧ・ｦ郢ｧ・｣郢晢ｽｳ郢晏ｳｨ縺郁怦・ｨ騾包ｽｻ鬮ｱ・｢郢ｧ蛛ｵ縺咲ｹ晁・繝ｻ邵ｺ蜷ｶ・狗ｸｺ貅假ｽ・Utility 陞ｳ螢ｽ辟夂ｸｺ・ｮ髫暦ｽ｣陷剃ｸ橸ｽｺ・ｦ郢ｧ蜑・ｽｽ・ｿ騾包ｽｨ邵ｺ蜷ｶ・・
		DrawBox(0, 0, Utility::kScreenWidth, Utility::kScreenHeight, GetColor(0, 0, 0), TRUE);
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
	
	// 陋ｻ・ｶ鬮ｯ蜈亥・鬮｢阮吶■郢ｧ・､郢晄ｧｭ繝ｻ驍ゑｽ｡騾・・笙郢ｧ蛹ｻ繝ｻ郢晢ｽｩ郢晢ｽｳ郢敖郢晢｣ｰ郢晁ｼ斐♂郢晢ｽｼ郢ｧ・ｺ陋ｻ繝ｻ・願ｭ厄ｽｿ邵ｺ莠･繝ｻ騾・・
	if (GameStepType::game_CowGet == mnType)
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
				GameNextStep(GameManager::game_final);
			}
			else
			{
				game_timer_->Update();
			}
		}

		int Timer = GetNowCount();

		if (Timer - m_PhaseTimer >= 1000)
		{
			m_PhaseTimer = Timer;
			m_PhaseChangeCount++;
		}
		
		// 30驕伜・・・ｸｺ・ｨ邵ｺ・ｫ郢ｧ・ｲ郢晢ｽｼ郢晢｣ｰ邵ｺ・ｮ郢晁ｼ斐♂郢晢ｽｼ郢ｧ・ｺ繝ｻ蝓滂ｽｼ豕後・繝ｻ蟲ｨ・堤ｹ晢ｽｩ郢晢ｽｳ郢敖郢晢｣ｰ邵ｺ・ｫ陞溽判蟲ｩ邵ｺ蜷ｶ・玖崕・ｶ驍上・
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