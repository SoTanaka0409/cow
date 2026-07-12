#include "ResultScene.h"
#include "Master.h"
#include "GameConstants.h"
#include "SceneManager.h"

ResultScene::ResultScene()
{
	flag_ = true;
	count_ = 0;
	
	result_graph_handle_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kResult);
	rank_image_[0] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank1);
	rank_image_[1] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank2);
	rank_image_[2] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank3);
	ranking_title_image_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRankingTitle);
	your_score_text_img_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kScoreTitle);
	point_img_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kPoint);

		UIButton newGameBtn;
	newGameBtn.Initialize(SelectionManager::Title::title, Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kBtnStart), 920, 50, 0.0f);
	buttons_.push_back(newGameBtn);

		UIButton exitBtn;
	exitBtn.Initialize(SelectionManager::Title::titleOUT, Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kBtnExit), 920, 250, 1.5f);
	buttons_.push_back(exitBtn);

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmResult);
	Master::mpSoundManager->SetBGMVolume(120);
	Master::mpScore;
}

ResultScene::~ResultScene()
{
	Finalize(); // 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ貍上ｌ繧帝亟縺舌◆繧∵・遉ｺ逧・↓蜻ｼ縺ｳ蜃ｺ縺・
}

void ResultScene::Initialize()
{
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	Master::mpScore->LoadRanking();
}

void ResultScene::Draw()
{
	DrawExtendGraph(0, 0, 1600, 900, result_graph_handle_, FALSE);

	DrawRankingUI();

	// 繝励Ξ繧､繝､繝ｼ縺ｮ譛邨ら佐蠕励せ繧ｳ繧｢繧剃ｸｭ必要Κ縺ｫ謠冗判縺吶ｋ
	DrawExtendGraph(500, 300, 1100, 550, your_score_text_img_, TRUE);
	int score = Score::GetResultScore();
	int temp = score;
	int digitCount = 0;
	if (temp == 0) digitCount = 1;
	else {
		while (temp > 0) {
			temp /= 10;
			digitCount++;
		}
	}
	if (digitCount < 4) digitCount = 4;

	int startX = 850;
	if (digitCount > 3) {
		startX = 850 - (digitCount - 3) * 40;
	}

	Master::mpScore->DrawNumber(startX, 490, score, 1.0f, 4);
	
	int pointX = startX + digitCount * 80;
	DrawExtendGraph(pointX, 430, pointX + 200, 630, point_img_, TRUE);

	Scene::Draw();
	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
}

void ResultScene::Update()
{
	count_++;

	// 蠕・ｩ滓凾髢鍋ｵ碁℃蠕後∬・蜍慕噪縺ｫ繧ｿ繧､繝医Ν逕ｻ髱｢縺ｸ謌ｻ繧九ヵ繧ｧ繝ｼ繝峨ｒ髢句ｧ・
	if (count_ >= 200 && fade_state_ != kSceneFadeOut)
	{
		fade_state_ = kSceneFadeOut;
		next_scene_ = SceneManager::kSceneTitle;
	}

	Scene::Update();
}

// [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] 逕ｻ髱｢荳翫↓繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧呈緒逕ｻ
void ResultScene::DrawRankingUI()
{
	int baseX = 40;
	int baseY = 40;

	DrawExtendGraph(
		baseX - 50,
		baseY - 100,
		baseX + 420,
		baseY + 120,
		ranking_title_image_,
		TRUE
	);

	for (int i = 0; i < 3; i++)
	{
		const Score::RankData& data = Master::mpScore->GetRanking(i);
		int y = baseY + 60 + i * 80;

		DrawExtendGraph(
			baseX,
			y - 20,
			baseX + 120,
			y + 100,
			rank_image_[i],
			TRUE
		);

		float scale = 0.6f;
		int w = (int)(80 * scale);
		int h = (int)(80 * scale);
		int drawY = y - 10 + (80 - h) / 2;

		Master::mpScore->DrawNumber(
			baseX + 180,
			drawY,
			data.score_,
			scale,
			4
		);

		int temp = data.score_;
		int digitCount = 0;
		if (temp == 0) digitCount = 1;
		else {
			while (temp > 0) {
				temp /= 10;
				digitCount++;
			}
		}
		if (digitCount < 4) digitCount = 4;
		int pointX = baseX + 180 + digitCount * w;
		DrawExtendGraph(pointX, drawY, pointX + w, drawY + h, point_img_, TRUE);
	}
}

// [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] 逕ｻ蜒上い繧ｻ繝・ヨ蜑企勁縲。GM蛛懈ｭ｢
void ResultScene::Finalize()
{
	Master::mpSoundManager->StopBGM();
}
