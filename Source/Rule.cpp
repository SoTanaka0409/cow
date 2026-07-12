#include"Scene.h"
#include "Rule.h"
#include "DxLib.h"
#include "Master.h"
#include "GameConstants.h"
#include "SceneManager.h"
#include "InputManager.h"

Rule::Rule()
{
	rule_graph_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kSettingsBg);
	title_font_handle_ = CreateFontToHandle("メイリオ", 80, 5);
	font_handle_ = CreateFontToHandle("メイリオ", 50, 3);
	
	selected_index_ = kMenuBgm;
	play_se_delay_ = 0;
}

Rule::~Rule()
{
	// シーン破?E??のメモリリークを防ぐため?E示?E??解放を呼ぶ
	Finalize();
}

void Rule::Update()
{
	Scene::Update();

	if (fade_state_ == kSceneFadeOut)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
			return;
		}
	}

	scene_frames_++;

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	static int prevMouseInput = 0;
	bool isMouseClicked = (mouseInput & MOUSE_INPUT_LEFT) != 0 && (prevMouseInput & MOUSE_INPUT_LEFT) == 0;
	bool isMouseHeld = (mouseInput & MOUSE_INPUT_LEFT) != 0;

	// 遷移前?E入力持ち越しによる誤操作を防ぐためE0フレーム征E??E
	if (scene_frames_ < 30)
	{
		prevMouseInput = mouseInput;
		return;
	}

	if (play_se_delay_ > 0) play_se_delay_--;

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < kMenuMax; i++)
	{
		int y = startY + i * gapY;
		if (mouse_y_ >= y && mouse_y_ <= y + 60)
		{
			if (selected_index_ != (MenuType)i)
			{
				selected_index_ = (MenuType)i;
			}
		}
	}

	if (isMouseHeld)
	{
		if (selected_index_ == kMenuBgm || selected_index_ == kMenuSe)
		{
			int y = startY + (int)selected_index_ * gapY;
			if (mouse_y_ >= y && mouse_y_ <= y + 60)
			{
				int barStartX = startX + 350;
				int barEndX = barStartX + (255 * 2);
				if (mouse_x_ >= barStartX && mouse_x_ <= barEndX)
				{
					int newVol = (mouse_x_ - barStartX) / 2;
					if (newVol < 0) newVol = 0;
					if (newVol > 255) newVol = 255;

					if (selected_index_ == kMenuBgm)
					{
						Master::mpSoundManager->SetMasterBGMVolume(newVol);
					}
					else if (selected_index_ == kMenuSe)
					{
						Master::mpSoundManager->SetMasterSEVolume(newVol);
						if (play_se_delay_ <= 0)
						{
							Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
							// SEが重なって?E??になる?Eを防ぐため、?E生間隔を制限すめE
							play_se_delay_ = 10;
						}
					}
				}
			}
		}
	}

	if (isMouseClicked && selected_index_ == kMenuBack)
	{
		int y = startY + kMenuBack * gapY;
		if (mouse_y_ >= y && mouse_y_ <= y + 60)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
			fade_state_ = kSceneFadeOut;
			next_scene_ = SceneManager::kSceneTitle;
		}
	}

	prevMouseInput = mouseInput;

	if (InputManager::CheckDownKey(KEY_INPUT_UP) || InputManager::CheckDownKey(KEY_INPUT_W))
	{
		selected_index_ = (MenuType)((int)selected_index_ - 1);
		if (selected_index_ < 0) selected_index_ = (MenuType)(kMenuMax - 1);
		Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN) || InputManager::CheckDownKey(KEY_INPUT_S))
	{
		selected_index_ = (MenuType)((int)selected_index_ + 1);
		if (selected_index_ >= kMenuMax) selected_index_ = kMenuBgm;
		Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
	}

	int volChange = 0;
	if (InputManager::CheckPressKey(KEY_INPUT_LEFT) || InputManager::CheckPressKey(KEY_INPUT_A))  volChange = -2;
	if (InputManager::CheckPressKey(KEY_INPUT_RIGHT) || InputManager::CheckPressKey(KEY_INPUT_D)) volChange = 2;

	if (volChange != 0)
	{
		if (selected_index_ == kMenuBgm)
		{
			int currentVol = Master::mpSoundManager->GetMasterBGMVolume();
			Master::mpSoundManager->SetMasterBGMVolume(currentVol + volChange);
		}
		else if (selected_index_ == kMenuSe)
		{
			int currentVol = Master::mpSoundManager->GetMasterSEVolume();
			Master::mpSoundManager->SetMasterSEVolume(currentVol + volChange);
			
			if (play_se_delay_ <= 0)
			{
				Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
				play_se_delay_ = 10;
			}
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_RETURN) || InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (selected_index_ == kMenuBack)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
			fade_state_ = kSceneFadeOut;
			next_scene_ = SceneManager::kSceneTitle;
		}
	}
}

void Rule::Draw()
{
	// 允E?Eシーン画像を透かして設定画面であることを強調するため、半透?Eの暗転を重ねめE
	DrawExtendGraph(0, -100, 1600, 1000, rule_graph_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(650, 100, "SETTINGS", GetColor(255, 255, 255), title_font_handle_);

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < kMenuMax; i++)
	{
		int y = startY + i * gapY;
		int color = (selected_index_ == i) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

		if (selected_index_ == i)
		{
			DrawStringToHandle(startX - 60, y, "?", color, font_handle_);
		}

		if (i == kMenuBgm)
		{
			int vol = Master::mpSoundManager->GetMasterBGMVolume();
			DrawFormatStringToHandle(startX, y, color, font_handle_, "BGM Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, font_handle_, "%3d", (vol * 100) / 255);
		}
		else if (i == kMenuSe)
		{
			int vol = Master::mpSoundManager->GetMasterSEVolume();
			DrawFormatStringToHandle(startX, y, color, font_handle_, "SE Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, font_handle_, "%3d", (vol * 100) / 255);
		}
		else if (i == kMenuBack)
		{
			DrawFormatStringToHandle(startX, y, color, font_handle_, "Back to Title");
		}
	}

	Scene::Draw();

	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
}

void Rule::Initialize()
{
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	selected_index_ = kMenuBgm;
	scene_frames_ = 0;
	Master::mpSoundManager->PlayBGM(SoundManager::kBgmRule);
}

void Rule::Finalize()
{
	DeleteFontToHandle(font_handle_);
	DeleteFontToHandle(title_font_handle_);
	Master::mpSoundManager->StopBGM();
}