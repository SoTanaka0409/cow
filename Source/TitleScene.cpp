#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	cowVoiceTimer = 180; // 初回再生までの待機フレーム設定
	
	titleGraphHandle = LoadGraph("Resource/2D/タイトルシーン.png");
	ufoGraphHandle = LoadGraph("Resource/2D/牛とUFO.png");
	GetGraphSize(ufoGraphHandle, &ufoW, &ufoH);

	ufoX = 540;
	ufoY = 30;

	isDraggingUfo = false;
	offsetX = 0;
	offsetY = 0;

	ufoVX = 2.0f;
	ufoVY = 1.5f;

	ufoAngle = 0.0f;
	isAutoPatrol = false;
	autoPatrolTimer = 0;

	TitleButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::NewGame;
	newGameBtn.graphHandle = LoadGraph("Resource/2D/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	buttons.push_back(newGameBtn);

	TitleButton tutorialBtn;
	tutorialBtn.type = SelectionManager::Title::Tutorial;
	tutorialBtn.graphHandle = LoadGraph("Resource/2D/チュートリアルぼたん.png");
	tutorialBtn.x = 900;
	tutorialBtn.y = 250;
	GetGraphSize(tutorialBtn.graphHandle, &tutorialBtn.w, &tutorialBtn.h);
	tutorialBtn.isHover = false;
	buttons.push_back(tutorialBtn);

	TitleButton opeBtn;
	opeBtn.type = SelectionManager::Title::OperationProcedures;
	opeBtn.graphHandle = LoadGraph("Resource/2D/せっていぼたん1.png");
	opeBtn.x = 915;
	opeBtn.y = 450;
	GetGraphSize(opeBtn.graphHandle, &opeBtn.w, &opeBtn.h);
	opeBtn.isHover = false;
	buttons.push_back(opeBtn);

	TitleButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graphHandle = LoadGraph("Resource/2D/しゅうりょうぼたん (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graphHandle, &exitBtn.w, &exitBtn.h);
	exitBtn.isHover = false;
	buttons.push_back(exitBtn);

	fadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	nextScene = SceneManager::SCENE_TITLE;

	isHoverNewGame = false;
	frameCount = 0;

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
	Master::gameFinishFlag = false;
	SetMouseDispFlag(true); // ユーザーがボタン操作できるようにマウスを表示
	Master::score->LoadRanking(); // ランキング表示用に保存されたスコアをロード
	Master::camera->Initialize();

	Master::soundManager->PlayBGM(SoundManager::BGM_TITLE);
}

void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, titleGraphHandle, FALSE);

	int ufoDrawY = ufoY;

	// ドラッグ中以外はサイン波でUFOの浮遊アニメーションを適用する
	if (!isDraggingUfo)
	{
		float ufoWave = sin(frameCount * 0.03f) * 20.0f;
		ufoDrawY += (int)ufoWave;
	}

	int ufoSize = 360;
	DrawExtendGraph(
		ufoX,
		ufoDrawY,
		ufoX + ufoSize,
		ufoDrawY + ufoSize,
		ufoGraphHandle,
		TRUE
	);

	// ホバー時にボタンを拡大描画し、視覚的なフィードバックを与える
	for (int i = 0; i < buttons.size(); i++)
	{
		float wave = sin(frameCount * 0.05f + (i * 1.5f)) * 10.0f;
		int drawY = buttons[i].y + (int)wave;

		if (buttons[i].isHover == true)
		{
			int expand = 15;
			DrawExtendGraph(
				buttons[i].x - expand,
				drawY - expand,
				buttons[i].x + buttons[i].w + expand,
				drawY + buttons[i].h + expand,
				buttons[i].graphHandle, TRUE);
		}
		else
		{
			DrawGraph(buttons[i].x, drawY, buttons[i].graphHandle, TRUE);
		}
	}

	
	DrawRankingUI();
	if (fadeState != SceneFade_None) {
		Scene::Fade(fadeState);
	}
}

void TitleScene::Update()
{
	frameCount++;

	if (fadeState != SceneFade_Out)
	{
		cowVoiceTimer--;
		if (cowVoiceTimer <= 0)
		{
			Master::soundManager->PlaySE(SoundManager::SE_COW); // タイトル画面の演出として定期的に再生
			cowVoiceTimer = GetRand(600) + 300; // 再生間隔をランダム化し単調さを防ぐ
		}
	}

	if (fadeState == SceneFade_Out)
	{
		Master::soundManager->SetBGMVolume((Master::soundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::sceneManager->SetNextScene((SceneManager::SCENE_TYPE)nextScene);
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
		if (mouseX >= ufoX && mouseX <= ufoX + ufoSize &&
			mouseY >= ufoY && mouseY <= ufoY + ufoSize)
		{
			isAutoPatrol = true;
			autoPatrolTimer = 240; // 4秒間（240フレーム）自動巡回を行う
			isDraggingUfo = true;
		}
	}
	else
	{
		isDraggingUfo = false;
	}

	// 楕円軌道の数式を用いてUFOを自動巡回させる
	if (isAutoPatrol)
	{
		ufoAngle += 0.02f;
		ufoX = 800 - (ufoSize / 2) + (int)(cos(ufoAngle) * 400.0f);
		ufoY = 450 - (ufoSize / 2) + (int)(sin(ufoAngle) * 200.0f);

		autoPatrolTimer--;
		if (autoPatrolTimer <= 0)
		{
			autoPatrolTimer = 0;
			isAutoPatrol = false;
		}
	}

	// UFO操作中以外にボタンのマウス判定とシーン遷移要求を行う
	if (isDraggingUfo == false)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w &&
				mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h)
			{
				buttons[i].isHover = true;

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					switch (buttons[i].type)
					{
					case SelectionManager::Title::NewGame:
						nextScene = SceneManager::SCENE_3D;
						fadeState = SceneFade_Out;
						Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
						break;
					case SelectionManager::Title::Tutorial:
						nextScene = SceneManager::SCENE_TUTORIAL;
						fadeState = SceneFade_Out;
						Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
						break;
					case SelectionManager::Title::OperationProcedures:
						nextScene = SceneManager::SCENE_RULE;
						fadeState = SceneFade_Out;
						Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
						break;
					case SelectionManager::Title::titleOUT:
						PostQuitMessage(0); // OSにアプリケーション終了要求を送信
						break;
					}
				}
			}
			else
			{
				buttons[i].isHover = false;
			}
		}
	}
	else
	{
		// UFOドラッグ中のボタン誤反応を防ぐためホバー状態を解除
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i].isHover = false;
		}
	}

	Scene::Update();
}

void TitleScene::Finalize()
{
	DeleteGraph(titleGraphHandle);
	DeleteGraph(ufoGraphHandle);

	for (int i = 0; i < buttons.size(); i++)
	{
		DeleteGraph(buttons[i].graphHandle);
	}

	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(rankImage[i]);
	}
	SetMouseDispFlag(false); // ゲーム中の誤操作を防ぐためマウスを非表示化
	DeleteGraph(rankingTitleImage);
	DeleteGraph(pointImg);
	Master::soundManager->StopBGM();
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
		const Score::RankData& data = Master::score->GetRanking(i);
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

		Master::score->DrawNumber(
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

