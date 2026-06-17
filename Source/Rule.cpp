#include"Scene.h"
#include "Rule.h"
#include "DxLib.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"

Rule::Rule()
{
	mRuleGraph = LoadGraph("Resource/2D/settings_bg.png");
	mTitleFontHandle = CreateFontToHandle("メイリオ", 80, 5);
	mFontHandle = CreateFontToHandle("メイリオ", 50, 3);
	
	mSelectedIndex = MENU_BGM;
	mPlaySeDelay = 0;
}

Rule::~Rule()
{
	// シーン破棄時のメモリリークを防ぐため明示的に解放を呼ぶ
	Finalize();
}

void Rule::Update()
{
	if (mFadeState == SceneFade_Out)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)mNextScene);
			return;
		}
	}

	mSceneFrames++;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int mouseInput = GetMouseInput();
	static int prevMouseInput = 0;
	bool isMouseClicked = (mouseInput & MOUSE_INPUT_LEFT) != 0 && (prevMouseInput & MOUSE_INPUT_LEFT) == 0;
	bool isMouseHeld = (mouseInput & MOUSE_INPUT_LEFT) != 0;

	// 遷移前の入力持ち越しによる誤操作を防ぐため30フレーム待機
	if (mSceneFrames < 30)
	{
		prevMouseInput = mouseInput;
		return;
	}

	if (mPlaySeDelay > 0) mPlaySeDelay--;

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < MENU_MAX; i++)
	{
		int y = startY + i * gapY;
		if (mouseY >= y && mouseY <= y + 60)
		{
			if (mSelectedIndex != (MenuType)i)
			{
				mSelectedIndex = (MenuType)i;
			}
		}
	}

	if (isMouseHeld)
	{
		if (mSelectedIndex == MENU_BGM || mSelectedIndex == MENU_SE)
		{
			int y = startY + (int)mSelectedIndex * gapY;
			if (mouseY >= y && mouseY <= y + 60)
			{
				int barStartX = startX + 350;
				int barEndX = barStartX + (255 * 2);
				if (mouseX >= barStartX && mouseX <= barEndX)
				{
					int newVol = (mouseX - barStartX) / 2;
					if (newVol < 0) newVol = 0;
					if (newVol > 255) newVol = 255;

					if (mSelectedIndex == MENU_BGM)
					{
						Master::mpSoundManager->SetMasterBGMVolume(newVol);
					}
					else if (mSelectedIndex == MENU_SE)
					{
						Master::mpSoundManager->SetMasterSEVolume(newVol);
						if (mPlaySeDelay <= 0)
						{
							Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
							// SEが重なって爆音になるのを防ぐため、再生間隔を制限する
							mPlaySeDelay = 10;
						}
					}
				}
			}
		}
	}

	if (isMouseClicked && mSelectedIndex == MENU_BACK)
	{
		int y = startY + MENU_BACK * gapY;
		if (mouseY >= y && mouseY <= y + 60)
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			mFadeState = SceneFade_Out;
			mNextScene = SceneManager::SCENE_TITLE;
		}
	}

	prevMouseInput = mouseInput;

	if (InputManager::CheckDownKey(KEY_INPUT_UP) || InputManager::CheckDownKey(KEY_INPUT_W))
	{
		mSelectedIndex = (MenuType)((int)mSelectedIndex - 1);
		if (mSelectedIndex < 0) mSelectedIndex = (MenuType)(MENU_MAX - 1);
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN) || InputManager::CheckDownKey(KEY_INPUT_S))
	{
		mSelectedIndex = (MenuType)((int)mSelectedIndex + 1);
		if (mSelectedIndex >= MENU_MAX) mSelectedIndex = MENU_BGM;
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
	}

	int volChange = 0;
	if (InputManager::CheckPressKey(KEY_INPUT_LEFT) || InputManager::CheckPressKey(KEY_INPUT_A))  volChange = -2;
	if (InputManager::CheckPressKey(KEY_INPUT_RIGHT) || InputManager::CheckPressKey(KEY_INPUT_D)) volChange = 2;

	if (volChange != 0)
	{
		if (mSelectedIndex == MENU_BGM)
		{
			int currentVol = Master::mpSoundManager->GetMasterBGMVolume();
			Master::mpSoundManager->SetMasterBGMVolume(currentVol + volChange);
		}
		else if (mSelectedIndex == MENU_SE)
		{
			int currentVol = Master::mpSoundManager->GetMasterSEVolume();
			Master::mpSoundManager->SetMasterSEVolume(currentVol + volChange);
			
			if (mPlaySeDelay <= 0)
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
				mPlaySeDelay = 10;
			}
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_RETURN) || InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (mSelectedIndex == MENU_BACK)
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			mFadeState = SceneFade_Out;
			mNextScene = SceneManager::SCENE_TITLE;
		}
	}
}

void Rule::Draw()
{
	// 元のシーン画像を透かして設定画面であることを強調するため、半透明の暗転を重ねる
	DrawExtendGraph(0, -100, 1600, 1000, mRuleGraph, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(650, 100, "SETTINGS", GetColor(255, 255, 255), mTitleFontHandle);

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < MENU_MAX; i++)
	{
		int y = startY + i * gapY;
		int color = (mSelectedIndex == i) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

		if (mSelectedIndex == i)
		{
			DrawStringToHandle(startX - 60, y, "?", color, mFontHandle);
		}

		if (i == MENU_BGM)
		{
			int vol = Master::mpSoundManager->GetMasterBGMVolume();
			DrawFormatStringToHandle(startX, y, color, mFontHandle, "BGM Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, mFontHandle, "%3d", (vol * 100) / 255);
		}
		else if (i == MENU_SE)
		{
			int vol = Master::mpSoundManager->GetMasterSEVolume();
			DrawFormatStringToHandle(startX, y, color, mFontHandle, "SE Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, mFontHandle, "%3d", (vol * 100) / 255);
		}
		else if (i == MENU_BACK)
		{
			DrawFormatStringToHandle(startX, y, color, mFontHandle, "Back to Title");
		}
	}

	Scene::Draw();
}

void Rule::Initialize()
{
	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	mSelectedIndex = MENU_BGM;
	mSceneFrames = 0;
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RULE);
}

void Rule::Finalize()
{
	DeleteGraph(mRuleGraph);
	DeleteFontToHandle(mFontHandle);
	DeleteFontToHandle(mTitleFontHandle);
	Master::mpSoundManager->StopBGM();
}
