#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	mCowVoiceTimer = 180; // 蛻晏屓蜀咲函縺ｾ縺ｧ縺ｮ蠕・ｩ溘ヵ繝ｬ繝ｼ繝險ｭ螳・
	
	mnTitleGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/タイトルシーン.png");
	mnUfoGraphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/牛とUFO.png");
	GetGraphSize(mnUfoGraphHandle, &mUfoW, &mUfoH);

	mUfoX = 540;
	mUfoY = 30;

	mbIsDraggingUfo = false;
	mOffsetX = 0;
	mOffsetY = 0;

	mUfoVX = 2.0f;
	mUfoVY = 1.5f;

	mUfoAngle = 0.0f;
	mIsAutoPatrol = false;
	mAutoPatrolTimer = 0;

	TitleButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::NewGame;
	newGameBtn.graphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	mButtons.push_back(newGameBtn);

	TitleButton tutorialBtn;
	tutorialBtn.type = SelectionManager::Title::Tutorial;
	tutorialBtn.graphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/チュートリアルぼたん.png");
	tutorialBtn.x = 900;
	tutorialBtn.y = 250;
	GetGraphSize(tutorialBtn.graphHandle, &tutorialBtn.w, &tutorialBtn.h);
	tutorialBtn.isHover = false;
	mButtons.push_back(tutorialBtn);

	TitleButton opeBtn;
	opeBtn.type = SelectionManager::Title::OperationProcedures;
	opeBtn.graphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/せっていぼたん1.png");
	opeBtn.x = 915;
	opeBtn.y = 450;
	GetGraphSize(opeBtn.graphHandle, &opeBtn.w, &opeBtn.h);
	opeBtn.isHover = false;
	mButtons.push_back(opeBtn);

	TitleButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graphHandle = Master::mpResourceManager->LoadGraphics("Resource/2D/しゅうりょうぼたん (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graphHandle, &exitBtn.w, &exitBtn.h);
	exitBtn.isHover = false;
	mButtons.push_back(exitBtn);

	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	mNextScene = SceneManager::SCENE_TITLE;

	mbIsHoverNewGame = false;
	mFrameCount = 0;

	// 繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ陦ｨ遉ｺ逕ｨ縺ｮ逕ｻ蜒上ｒ繝ｭ繝ｼ繝会ｼ・xLib蛻晄悄蛹門ｾ後〒縺ｪ縺・→螟ｱ謨励☆繧九◆繧√％縺薙〒陦後≧・・
	rankImage[0] = Master::mpResourceManager->LoadGraphics("Resource/2D/1位.png");
	rankImage[1] = Master::mpResourceManager->LoadGraphics("Resource/2D/2位.png");
	rankImage[2] = Master::mpResourceManager->LoadGraphics("Resource/2D/3位.png");
	rankingTitleImage = Master::mpResourceManager->LoadGraphics("Resource/2D/ランキング.png");
	pointImg = Master::mpResourceManager->LoadGraphics("Resource/2D/点.png");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;
	SetMouseDispFlag(true); // 繝ｦ繝ｼ繧ｶ繝ｼ縺後・繧ｿ繝ｳ謫堺ｽ懊〒縺阪ｋ繧医≧縺ｫ繝槭え繧ｹ繧定｡ｨ遉ｺ
	Master::mpScore->LoadRanking(); // 繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ陦ｨ遉ｺ逕ｨ縺ｫ菫晏ｭ倥＆繧後◆繧ｹ繧ｳ繧｢繧偵Ο繝ｼ繝・
	Master::mpCamera->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmTitle);
}

void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, mnTitleGraphHandle, FALSE);

	int ufoDrawY = mUfoY;

	// 繝峨Λ繝・げ荳ｭ莉･螟悶・繧ｵ繧､繝ｳ豕｢縺ｧUFO縺ｮ豬ｮ驕翫い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧帝←逕ｨ縺吶ｋ
	if (!mbIsDraggingUfo)
	{
		float ufoWave = sin(mFrameCount * 0.03f) * 20.0f;
		ufoDrawY += (int)ufoWave;
	}

	int ufoSize = 360;
	DrawExtendGraph(
		mUfoX,
		ufoDrawY,
		mUfoX + ufoSize,
		ufoDrawY + ufoSize,
		mnUfoGraphHandle,
		TRUE
	);

	// 繝帙ヰ繝ｼ譎ゅ↓繝懊ち繝ｳ繧呈僑螟ｧ謠冗判縺励∬ｦ冶ｦ夂噪縺ｪ繝輔ぅ繝ｼ繝峨ヰ繝・け繧剃ｸ弱∴繧・
	for (int i = 0; i < mButtons.size(); i++)
	{
		float wave = sin(mFrameCount * 0.05f + (i * 1.5f)) * 10.0f;
		int drawY = mButtons[i].y + (int)wave;

		if (mButtons[i].isHover == true)
		{
			int expand = 15;
			DrawExtendGraph(
				mButtons[i].x - expand,
				drawY - expand,
				mButtons[i].x + mButtons[i].w + expand,
				drawY + mButtons[i].h + expand,
				mButtons[i].graphHandle, TRUE);
		}
		else
		{
			DrawGraph(mButtons[i].x, drawY, mButtons[i].graphHandle, TRUE);
		}
	}

	
	DrawRankingUI();
	if (mFadeState != SceneFade_None) {
		Scene::Fade(mFadeState);
	}
}

void TitleScene::Update()
{
	mFrameCount++;

	if (mFadeState != SceneFade_Out)
	{
		mCowVoiceTimer--;
		if (mCowVoiceTimer <= 0)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeCow); // 繧ｿ繧､繝医Ν逕ｻ髱｢縺ｮ貍泌・縺ｨ縺励※螳壽悄逧・↓蜀咲函
			mCowVoiceTimer = GetRand(600) + 300; // 蜀咲函髢馴囈繧偵Λ繝ｳ繝繝蛹悶＠蜊倩ｪｿ縺輔ｒ髦ｲ縺・
		}
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

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	// UFO繧ｯ繝ｪ繝・け譎ゅ↓髫縺苓ｦ∫ｴ縺ｮ閾ｪ蜍募ｷ｡蝗槭Δ繝ｼ繝峨ｒ襍ｷ蜍輔☆繧・
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouse_x_ >= mUfoX && mouse_x_ <= mUfoX + ufoSize &&
			mouse_y_ >= mUfoY && mouse_y_ <= mUfoY + ufoSize)
		{
			mIsAutoPatrol = true;
			mAutoPatrolTimer = 240; // 4遘帝俣・・40繝輔Ξ繝ｼ繝・芽・蜍募ｷ｡蝗槭ｒ陦後≧
			mbIsDraggingUfo = true;
		}
	}
	else
	{
		mbIsDraggingUfo = false;
	}

	// 讌募・霆碁％縺ｮ謨ｰ蠑上ｒ逕ｨ縺・※UFO繧定・蜍募ｷ｡蝗槭＆縺帙ｋ
	if (mIsAutoPatrol)
	{
		mUfoAngle += 0.02f;
		mUfoX = 800 - (ufoSize / 2) + (int)(cos(mUfoAngle) * 400.0f);
		mUfoY = 450 - (ufoSize / 2) + (int)(sin(mUfoAngle) * 200.0f);

		mAutoPatrolTimer--;
		if (mAutoPatrolTimer <= 0)
		{
			mAutoPatrolTimer = 0;
			mIsAutoPatrol = false;
		}
	}

	// UFO謫堺ｽ應ｸｭ莉･螟悶↓繝懊ち繝ｳ縺ｮ繝槭え繧ｹ蛻､螳壹→繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ隕∵ｱゅｒ陦後≧
	if (mbIsDraggingUfo == false)
	{
		for (int i = 0; i < mButtons.size(); i++)
		{
			if (mouse_x_ >= mButtons[i].x && mouse_x_ <= mButtons[i].x + mButtons[i].w &&
				mouse_y_ >= mButtons[i].y && mouse_y_ <= mButtons[i].y + mButtons[i].h)
			{
				mButtons[i].isHover = true;

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					switch (mButtons[i].type)
					{
					case SelectionManager::Title::NewGame:
						mNextScene = SceneManager::SCENE_LOADING;
						mFadeState = SceneFade_Out;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::Tutorial:
						mNextScene = SceneManager::SCENE_TUTORIAL;
						mFadeState = SceneFade_Out;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::OperationProcedures:
						mNextScene = SceneManager::SCENE_RULE;
						mFadeState = SceneFade_Out;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::titleOUT:
						PostQuitMessage(0); // OS縺ｫ繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ邨ゆｺ・ｦ∵ｱゅｒ騾∽ｿ｡
						break;
					}
				}
			}
			else
			{
				mButtons[i].isHover = false;
			}
		}
	}
	else
	{
		// UFO繝峨Λ繝・げ荳ｭ縺ｮ繝懊ち繝ｳ隱､蜿榊ｿ懊ｒ髦ｲ縺舌◆繧√・繝舌・迥ｶ諷九ｒ隗｣髯､
		for (int i = 0; i < mButtons.size(); i++) {
			mButtons[i].isHover = false;
		}
	}

	Scene::Update();
}

void TitleScene::Finalize()
{
	DeleteGraph(mnTitleGraphHandle);
	DeleteGraph(mnUfoGraphHandle);

	for (int i = 0; i < mButtons.size(); i++)
	{
		DeleteGraph(mButtons[i].graphHandle);
	}

	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(rankImage[i]);
	}
	SetMouseDispFlag(false); // 繧ｲ繝ｼ繝荳ｭ縺ｮ隱､謫堺ｽ懊ｒ髦ｲ縺舌◆繧√・繧ｦ繧ｹ繧帝撼陦ｨ遉ｺ蛹・
	DeleteGraph(rankingTitleImage);
	DeleteGraph(pointImg);
	Master::mpSoundManager->StopBGM();
}

// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 逕ｻ髱｢荳翫↓繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ謨ｰ蛟､縺ｨ繧｢繧､繧ｳ繝ｳ繧呈緒逕ｻ
void TitleScene::DrawRankingUI()
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
