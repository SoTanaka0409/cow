#include"Scene.h"
#include "Rule.h"
#include "DxLib.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"

Rule::Rule()
{
	ruleGraph = LoadGraph("Resource/2D/settings_bg.png");
	titleFontHandle = CreateFontToHandle("メイリオ", 80, 5);
	fontHandle = CreateFontToHandle("メイリオ", 50, 3);
	
	selectedIndex = MENU_BGM;
	playSeDelay = 0;
}

Rule::~Rule()
{
	// シーン破棄時のメモリリークを防ぐため明示的に解放を呼ぶ
	Finalize();
}

void Rule::Update()
{
	if (fadeState == SceneFade_Out)
	{
		Master::soundManager->SetBGMVolume((Master::soundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::sceneManager->SetNextScene((SceneManager::SCENE_TYPE)nextScene);
			return;
		}
	}

	sceneFrames++;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int mouseInput = GetMouseInput();
	static int prevMouseInput = 0;
	bool isMouseClicked = (mouseInput & MOUSE_INPUT_LEFT) != 0 && (prevMouseInput & MOUSE_INPUT_LEFT) == 0;
	bool isMouseHeld = (mouseInput & MOUSE_INPUT_LEFT) != 0;

	// 遷移前の入力持ち越しによる誤操作を防ぐため30フレーム待機
	if (sceneFrames < 30)
	{
		prevMouseInput = mouseInput;
		return;
	}

	if (playSeDelay > 0) playSeDelay--;

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < MENU_MAX; i++)
	{
		int y = startY + i * gapY;
		if (mouseY >= y && mouseY <= y + 60)
		{
			if (selectedIndex != (MenuType)i)
			{
				selectedIndex = (MenuType)i;
			}
		}
	}

	if (isMouseHeld)
	{
		if (selectedIndex == MENU_BGM || selectedIndex == MENU_SE)
		{
			int y = startY + (int)selectedIndex * gapY;
			if (mouseY >= y && mouseY <= y + 60)
			{
				int barStartX = startX + 350;
				int barEndX = barStartX + (255 * 2);
				if (mouseX >= barStartX && mouseX <= barEndX)
				{
					int newVol = (mouseX - barStartX) / 2;
					if (newVol < 0) newVol = 0;
					if (newVol > 255) newVol = 255;

					if (selectedIndex == MENU_BGM)
					{
						Master::soundManager->SetMasterBGMVolume(newVol);
					}
					else if (selectedIndex == MENU_SE)
					{
						Master::soundManager->SetMasterSEVolume(newVol);
						if (playSeDelay <= 0)
						{
							Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
							// SEが重なって爆音になるのを防ぐため、再生間隔を制限する
							playSeDelay = 10;
						}
					}
				}
			}
		}
	}

	if (isMouseClicked && selectedIndex == MENU_BACK)
	{
		int y = startY + MENU_BACK * gapY;
		if (mouseY >= y && mouseY <= y + 60)
		{
			Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
			fadeState = SceneFade_Out;
			nextScene = SceneManager::SCENE_TITLE;
		}
	}

	prevMouseInput = mouseInput;

	if (InputManager::CheckDownKey(KEY_INPUT_UP) || InputManager::CheckDownKey(KEY_INPUT_W))
	{
		selectedIndex = (MenuType)((int)selectedIndex - 1);
		if (selectedIndex < 0) selectedIndex = (MenuType)(MENU_MAX - 1);
		Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN) || InputManager::CheckDownKey(KEY_INPUT_S))
	{
		selectedIndex = (MenuType)((int)selectedIndex + 1);
		if (selectedIndex >= MENU_MAX) selectedIndex = MENU_BGM;
		Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
	}

	int volChange = 0;
	if (InputManager::CheckPressKey(KEY_INPUT_LEFT) || InputManager::CheckPressKey(KEY_INPUT_A))  volChange = -2;
	if (InputManager::CheckPressKey(KEY_INPUT_RIGHT) || InputManager::CheckPressKey(KEY_INPUT_D)) volChange = 2;

	if (volChange != 0)
	{
		if (selectedIndex == MENU_BGM)
		{
			int currentVol = Master::soundManager->GetMasterBGMVolume();
			Master::soundManager->SetMasterBGMVolume(currentVol + volChange);
		}
		else if (selectedIndex == MENU_SE)
		{
			int currentVol = Master::soundManager->GetMasterSEVolume();
			Master::soundManager->SetMasterSEVolume(currentVol + volChange);
			
			if (playSeDelay <= 0)
			{
				Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
				playSeDelay = 10;
			}
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_RETURN) || InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (selectedIndex == MENU_BACK)
		{
			Master::soundManager->PlaySE(SoundManager::SE_DECIDE);
			fadeState = SceneFade_Out;
			nextScene = SceneManager::SCENE_TITLE;
		}
	}
}

void Rule::Draw()
{
	// 元のシーン画像を透かして設定画面であることを強調するため、半透明の暗転を重ねる
	DrawExtendGraph(0, -100, 1600, 1000, ruleGraph, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(650, 100, "SETTINGS", GetColor(255, 255, 255), titleFontHandle);

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < MENU_MAX; i++)
	{
		int y = startY + i * gapY;
		int color = (selectedIndex == i) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

		if (selectedIndex == i)
		{
			DrawStringToHandle(startX - 60, y, "?", color, fontHandle);
		}

		if (i == MENU_BGM)
		{
			int vol = Master::soundManager->GetMasterBGMVolume();
			DrawFormatStringToHandle(startX, y, color, fontHandle, "BGM Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, fontHandle, "%3d", (vol * 100) / 255);
		}
		else if (i == MENU_SE)
		{
			int vol = Master::soundManager->GetMasterSEVolume();
			DrawFormatStringToHandle(startX, y, color, fontHandle, "SE Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, fontHandle, "%3d", (vol * 100) / 255);
		}
		else if (i == MENU_BACK)
		{
			DrawFormatStringToHandle(startX, y, color, fontHandle, "Back to Title");
		}
	}

	Scene::Draw();
}

void Rule::Initialize()
{
	fadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	selectedIndex = MENU_BGM;
	sceneFrames = 0;
	Master::soundManager->PlayBGM(SoundManager::BGM_RULE);
}

void Rule::Finalize()
{
	DeleteGraph(ruleGraph);
	DeleteFontToHandle(fontHandle);
	DeleteFontToHandle(titleFontHandle);
	Master::soundManager->StopBGM();
}
