#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	mCowVoiceTimer = 180; // 初回再生までの待機フレーム設定
	
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

	TitleButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::NewGame;
	newGameBtn.graphHandle = LoadGraph("Resource/2D/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	mButtons.push_back(newGameBtn);

	TitleButton tutorialBtn;
	tutorialBtn.type = SelectionManager::Title::Tutorial;
	tutorialBtn.graphHandle = LoadGraph("Resource/2D/チュートリアルぼたん.png");
	tutorialBtn.x = 900;
	tutorialBtn.y = 250;
	GetGraphSize(tutorialBtn.graphHandle, &tutorialBtn.w, &tutorialBtn.h);
	tutorialBtn.isHover = false;
	mButtons.push_back(tutorialBtn);

	TitleButton opeBtn;
	opeBtn.type = SelectionManager::Title::OperationProcedures;
	opeBtn.graphHandle = LoadGraph("Resource/2D/せっていぼたん1.png");
	opeBtn.x = 915;
	opeBtn.y = 450;
	GetGraphSize(opeBtn.graphHandle, &opeBtn.w, &opeBtn.h);
	opeBtn.isHover = false;
	mButtons.push_back(opeBtn);

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

	// ランキング表示用の画像をロード（DxLib初期化後でないと失敗するためここで行う）
	rankImage[0] = LoadGraph("Resource/2D/1位.png");
	rankImage[1] = LoadGraph("Resource/2D/2位.png");
	rankImage[2] = LoadGraph("Resource/2D/3位.png");
	rankingTitleImage = LoadGraph("Resource/2D/ランキング.png");
	pointImg = LoadGraph("Resource/2D/点.png");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;
	SetMouseDispFlag(true); // ユーザーがボタン操作できるようにマウスを表示
	Master::mpScore->LoadRanking(); // ランキング表示用に保存されたスコアをロード
	Master::mpCamera->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, mnTitleGraphHandle, FALSE);

	int ufoDrawY = mUfoY;

	// ドラッグ中以外はサイン波でUFOの浮遊アニメーションを適用する
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

	// ホバー時にボタンを拡大描画し、視覚的なフィードバックを与える
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
			Master::mpSoundManager->PlaySE(SoundManager::SE_COW); // タイトル画面の演出として定期的に再生
			mCowVoiceTimer = GetRand(600) + 300; // 再生間隔をランダム化し単調さを防ぐ
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

	// UFOクリック時に隠し要素の自動巡回モードを起動する
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

	// 楕円軌道の数式を用いてUFOを自動巡回させる
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

	// UFO操作中以外にボタンのマウス判定とシーン遷移要求を行う
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
						PostQuitMessage(0); // OSにアプリケーション終了要求を送信
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
		// UFOドラッグ中のボタン誤反応を防ぐためホバー状態を解除
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
	SetMouseDispFlag(false); // ゲーム中の誤操作を防ぐためマウスを非表示化
	DeleteGraph(rankingTitleImage);
	DeleteGraph(pointImg);
	Master::mpSoundManager->StopBGM();
}

// [入力] なし [出力] なし [副作用] 画面上にランキング数値とアイコンを描画
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
			data.score,
			scale,
			4
		);

		int temp = data.score;
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

