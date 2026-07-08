#include"Scene.h"
#include "Rule.h"
#include "DxLib.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"

Rule::Rule()
{
	mRuleGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/settings_bg.png");
	mTitleFontHandle = CreateFontToHandle("繝｡繧､繝ｪ繧ｪ", 80, 5);
	mFontHandle = CreateFontToHandle("繝｡繧､繝ｪ繧ｪ", 50, 3);
	
	mSelectedIndex = MENU_BGM;
	mPlaySeDelay = 0;
}

Rule::~Rule()
{
	// 繧ｷ繝ｼ繝ｳ遐ｴ譽・凾縺ｮ繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ繧帝亟縺舌◆繧∵・遉ｺ逧・↓隗｣謾ｾ繧貞他縺ｶ
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

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	static int prevMouseInput = 0;
	bool isMouseClicked = (mouseInput & MOUSE_INPUT_LEFT) != 0 && (prevMouseInput & MOUSE_INPUT_LEFT) == 0;
	bool isMouseHeld = (mouseInput & MOUSE_INPUT_LEFT) != 0;

	// 驕ｷ遘ｻ蜑阪・蜈･蜉帶戟縺｡雜翫＠縺ｫ繧医ｋ隱､謫堺ｽ懊ｒ髦ｲ縺舌◆繧・0繝輔Ξ繝ｼ繝蠕・ｩ・
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
		if (mouse_y_ >= y && mouse_y_ <= y + 60)
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
			if (mouse_y_ >= y && mouse_y_ <= y + 60)
			{
				int barStartX = startX + 350;
				int barEndX = barStartX + (255 * 2);
				if (mouse_x_ >= barStartX && mouse_x_ <= barEndX)
				{
					int newVol = (mouse_x_ - barStartX) / 2;
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
							Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
							// SE縺碁㍾縺ｪ縺｣縺ｦ辷・浹縺ｫ縺ｪ繧九・繧帝亟縺舌◆繧√∝・逕滄俣髫斐ｒ蛻ｶ髯舌☆繧・
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
		if (mouse_y_ >= y && mouse_y_ <= y + 60)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
			mFadeState = SceneFade_Out;
			mNextScene = SceneManager::SCENE_TITLE;
		}
	}

	prevMouseInput = mouseInput;

	if (InputManager::CheckDownKey(KEY_INPUT_UP) || InputManager::CheckDownKey(KEY_INPUT_W))
	{
		mSelectedIndex = (MenuType)((int)mSelectedIndex - 1);
		if (mSelectedIndex < 0) mSelectedIndex = (MenuType)(MENU_MAX - 1);
		Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN) || InputManager::CheckDownKey(KEY_INPUT_S))
	{
		mSelectedIndex = (MenuType)((int)mSelectedIndex + 1);
		if (mSelectedIndex >= MENU_MAX) mSelectedIndex = MENU_BGM;
		Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
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
				Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
				mPlaySeDelay = 10;
			}
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_RETURN) || InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (mSelectedIndex == MENU_BACK)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
			mFadeState = SceneFade_Out;
			mNextScene = SceneManager::SCENE_TITLE;
		}
	}
}

void Rule::Draw()
{
	// 蜈・・繧ｷ繝ｼ繝ｳ逕ｻ蜒上ｒ騾上°縺励※險ｭ螳夂判髱｢縺ｧ縺ゅｋ縺薙→繧貞ｼｷ隱ｿ縺吶ｋ縺溘ａ縲∝濠騾乗・縺ｮ證苓ｻ｢繧帝㍾縺ｭ繧・
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
	Master::mpSoundManager->PlayBGM(SoundManager::kBgmRule);
}

void Rule::Finalize()
{
	DeleteFontToHandle(mFontHandle);
	DeleteFontToHandle(mTitleFontHandle);
	Master::mpSoundManager->StopBGM();
}