#include"ResultScene.h"
#include"Master.h"
#include"SceneManager.h"

ResultScene::ResultScene()
{
	mFlag = true;
	mnCount = 0;
	
	ResultGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/繝ｪ繧ｶ繝ｫ繝・png");
	rankImage[0] = Master::mpResourceManager->LoadGraphics("Resource/2D/1菴・png");
	rankImage[1] = Master::mpResourceManager->LoadGraphics("Resource/2D/2菴・png");
	rankImage[2] = Master::mpResourceManager->LoadGraphics("Resource/2D/3菴・png");
	rankingTitleImage = Master::mpResourceManager->LoadGraphics("Resource/2D/繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ.png");
	yourScoreTextImg = Master::mpResourceManager->LoadGraphics("Resource/2D/繧ｹ繧ｳ繧｢.png");
	pointImg = Master::mpResourceManager->LoadGraphics("Resource/2D/轤ｹ.png");

	ResultButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::title;
	newGameBtn.graphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/繧ｹ繧ｿ繝ｼ繝医・繧ｿ繝ｳ1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	mButtons.push_back(newGameBtn);

	ResultButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/縺励ｅ縺・ｊ繧・≧縺ｼ縺溘ｓ (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graphHandle, &exitBtn.w, &exitBtn.h);
	exitBtn.isHover = false;
	mButtons.push_back(exitBtn);

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
	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	Master::mpScore->LoadRanking();
}

void ResultScene::Draw()
{
	DrawExtendGraph(0, 0, 1600, 900, ResultGraphHandle, FALSE);

	DrawRankingUI();

	// 繝励Ξ繧､繝､繝ｼ縺ｮ譛邨ら佐蠕励せ繧ｳ繧｢繧剃ｸｭ蠢・Κ縺ｫ謠冗判縺吶ｋ
	DrawExtendGraph(500, 300, 1100, 550, yourScoreTextImg, TRUE);
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
	DrawExtendGraph(pointX, 430, pointX + 200, 630, pointImg, TRUE);

	Scene::Draw();
	if (mFadeState != SceneFade_None) {
		Scene::Fade(mFadeState);
	}
}

void ResultScene::Update()
{
	mnCount++;

	// 蠕・ｩ滓凾髢鍋ｵ碁℃蠕後∬・蜍慕噪縺ｫ繧ｿ繧､繝医Ν逕ｻ髱｢縺ｸ謌ｻ繧九ヵ繧ｧ繝ｼ繝峨ｒ髢句ｧ・
	if (mnCount >= 200 && mFadeState != SceneFade_Out)
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_TITLE;
	}

	if (mFadeState == SceneFade_Out)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)mNextScene);
		}
		return;
	}

	Scene::Update();
}

// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 逕ｻ髱｢荳翫↓繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧呈緒逕ｻ
void ResultScene::DrawRankingUI()
{
	int baseX = 40;
	int baseY = 40;

	DrawExtendGraph(
		baseX - 50,
		baseY - 100,
		baseX + 420,
		baseY + 120,
		rankingTitleImage,
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
			rankImage[i],
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
		DrawExtendGraph(pointX, drawY, pointX + w, drawY + h, pointImg, TRUE);
	}
}

// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 逕ｻ蜒上い繧ｻ繝・ヨ蜑企勁縲。GM蛛懈ｭ｢
void ResultScene::Finalize()
{
	Master::mpSoundManager->StopBGM();
}
