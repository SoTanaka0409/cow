#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	mCowVoiceTimer = 180; // 初回に牛が鳴くまでの待機フレーム時間設定
	
	mnTitleGraphHandle = LoadGraph("Resource/2D/タイトルシーン.png");
	mnUfoGraphHandle = LoadGraph("Resource/2D/牛とUFO.png");
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

	// タイトル画面用のランキングUI用画像をロード
	rankImage[0] = LoadGraph("Resource/2D/1位.png");
	rankImage[1] = LoadGraph("Resource/2D/2位.png");
	rankImage[2] = LoadGraph("Resource/2D/3位.png");
	rankingTitleImage = LoadGraph("Resource/2D/ランキング.png");
	 
	// インタラクティブな各遷移ボタンのデータをリストに登録
	// 1. スタートボタン
	TitleButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::NewGame;
	newGameBtn.graphHandle = LoadGraph("Resource/2D/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	mButtons.push_back(newGameBtn);

	// 2. チュートリアルボタン
	TitleButton tutorialBtn;
	tutorialBtn.type = SelectionManager::Title::Tutorial;
	tutorialBtn.graphHandle = LoadGraph("Resource/2D/チュートリアルぼたん.png");
	tutorialBtn.x = 900;
	tutorialBtn.y = 250;
	GetGraphSize(tutorialBtn.graphHandle, &tutorialBtn.w, &tutorialBtn.h);
	tutorialBtn.isHover = false;
	mButtons.push_back(tutorialBtn);

	// 3. 操作説明（ルール）ボタン
	TitleButton opeBtn;
	opeBtn.type = SelectionManager::Title::OperationProcedures;
	opeBtn.graphHandle = LoadGraph("Resource/2D/せっていぼたん1.png");
	opeBtn.x = 915;
	opeBtn.y = 450;
	GetGraphSize(opeBtn.graphHandle, &opeBtn.w, &opeBtn.h);
	opeBtn.isHover = false;
	mButtons.push_back(opeBtn);

	// 4. ゲーム終了ボタン
	TitleButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graphHandle = LoadGraph("Resource/2D/しゅうりょうぼたん (1).png");
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
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;

	Master::mpScore->LoadRanking(); // 最新のハイスコアデータをストレージから読み込み
	Master::mpCamera->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

void TitleScene::Draw()
{
	DrawExtendGraph(0, 0, 1600, 900, mnTitleGraphHandle, FALSE);

	int ufoDrawY = mUfoY;

	// ドラッグされていない時はサイン波を用いてふわふわ浮かぶアニメーションを行う
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

	// 各ボタンを描画（マウスホバー時はサイズを少し拡大してアピール）
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

	Scene::Draw();
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
			Master::mpSoundManager->PlaySE(SoundManager::SE_COW); // タイトル画面の賑やかしとして定期的にSE再生
			mCowVoiceTimer = GetRand(600) + 300; // 次回再生までランダムに間隔を設定 (5秒〜15秒)
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

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	// UFOをクリックした際に自動巡回イースターエッグを発動させる判定
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouseX >= mUfoX && mouseX <= mUfoX + ufoSize &&
			mouseY >= mUfoY && mouseY <= mUfoY + ufoSize)
		{
			mIsAutoPatrol = true;
			mAutoPatrolTimer = 240; // 4秒間（240フレーム）自動巡回を行う
			mbIsDraggingUfo = true;
		}
	}
	else
	{
		mbIsDraggingUfo = false;
	}

	// UFOの自動巡回パス処理（サイン・コサインによる楕円軌道）
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

	// 各ボタンに対するマウスの当たり判定（ホバー状態の切り替え、クリック時のシーン要求）
	if (mbIsDraggingUfo == false)
	{
		for (int i = 0; i < mButtons.size(); i++)
		{
			if (mouseX >= mButtons[i].x && mouseX <= mButtons[i].x + mButtons[i].w &&
				mouseY >= mButtons[i].y && mouseY <= mButtons[i].y + mButtons[i].h)
			{
				mButtons[i].isHover = true;

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					switch (mButtons[i].type)
					{
					case SelectionManager::Title::NewGame:
						mNextScene = SceneManager::SCENE_3D;
						mFadeState = SceneFade_Out;
						Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
						break;
					case SelectionManager::Title::Tutorial:
						mNextScene = SceneManager::SCENE_TUTORIAL;
						mFadeState = SceneFade_Out;
						Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
						break;
					case SelectionManager::Title::OperationProcedures:
						mNextScene = SceneManager::SCENE_RULE;
						mFadeState = SceneFade_Out;
						Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
						break;
					case SelectionManager::Title::titleOUT:
						PostQuitMessage(0); // Windowsアプリケーションとしての安全な終了通知
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
		// UFO操作中は誤反応を防ぐため、ボタンのホバー状態を強制解除
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

	DeleteGraph(rankingTitleImage);
	Master::mpSoundManager->StopBGM();
}

/*
 * @brief スコアランキング情報UIの描画処理
 * [入力] なし
 * [出力] なし
 * [副作用] スコアデータから数値フォント画像の描画
 */
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

		Master::mpScore->DrawNumber(
			baseX + 180,
			y - 10,
			data.score
		);
	}
}
