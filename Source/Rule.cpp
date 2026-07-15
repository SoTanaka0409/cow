#include "Scene.h"
#include "Rule.h"
#include "DxLib.h"
#include "Master.h"
#include "GameConstants.h"
#include "SceneManager.h"
#include "InputManager.h"

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 設定画面用のUI画像およびフォントリソースの読み込みと初期設定
 */
Rule::Rule()
{
	rule_graph_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kSettingsBg);
	title_font_handle_ = CreateFontToHandle("メイリオ", 80, 5);
	font_handle_ = CreateFontToHandle("メイリオ", 50, 3);

	selected_index_ = kMenuBgm;
	play_se_delay_ = 0;
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: リソースの明示的な解放処理の呼び出し
 */
Rule::~Rule()
{
	// シーン破棄時のフォントハンドルの解放漏れによるメモリリークを防ぐため、デストラクタで確実な破棄を保証する
	Finalize();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ユーザー入力による音量設定の更新およびシーン遷移処理
 */
void Rule::Update()
{
	Scene::Update();

	if (UpdateFadeState()) return;

	scene_frames_++;

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	static int prevMouseInput = 0;
	bool isMouseClicked = (mouseInput & MOUSE_INPUT_LEFT) != 0 && (prevMouseInput & MOUSE_INPUT_LEFT) == 0;
	bool isMouseHeld = (mouseInput & MOUSE_INPUT_LEFT) != 0;

	// 直前シーンのクリック保持等による誤操作防止のための待機処理
	if (scene_frames_ < 30)
	{
		prevMouseInput = mouseInput;
		return;
	}

	UpdateMenu(mouse_x_, mouse_y_, mouseInput, isMouseClicked, isMouseHeld);

	prevMouseInput = mouseInput;
}

bool Rule::UpdateFadeState()
{
	if (fade_state_ == kSceneFadeOut)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
			return true;
		}
	}
	return false;
}

void Rule::UpdateMenu(int mouse_x, int mouse_y, int mouseInput, bool isMouseClicked, bool isMouseHeld)
{
	if (play_se_delay_ > 0) play_se_delay_--;

	int startY = 350;
	int gapY = 150;
	int startX = 400;

	for (int i = 0; i < kMenuMax; i++)
	{
		int y = startY + i * gapY;
		if (mouse_y >= y && mouse_y <= y + 60)
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
			if (mouse_y >= y && mouse_y <= y + 60)
			{
				int barStartX = startX + 350;
				int barEndX = barStartX + (255 * 2);
				if (mouse_x >= barStartX && mouse_x <= barEndX)
				{
					int newVol = (mouse_x - barStartX) / 2;
					if (newVol < 0) newVol = 0;
					if (newVol > 255) newVol = 255;

					if (selected_index_ == kMenuBgm)
					{
						Master::mpSoundManager->SetMasterBGMVolume(newVol);
					}
					else if (selected_index_ == kMenuSe)
					{
						Master::mpSoundManager->SetMasterSEVolume(newVol);

						// スライダー操作時SE暴発、耳障り(音割れ)になるのを防ぐためのクールタイム
						if (play_se_delay_ <= 0)
						{
							Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
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
		if (mouse_y >= y && mouse_y <= y + 60)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
			fade_state_ = kSceneFadeOut;
			next_scene_ = SceneManager::kSceneTitle;
		}
	}

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

			// キー操作時SE防爆(マウス操作時と同様の対策)
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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 画面の暗転およびUIの描画
 */
void Rule::Draw()
{
	DrawBackground();
	DrawMenu();

	Scene::Draw();

	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
}

void Rule::DrawBackground()
{
	DrawExtendGraph(0, -100, 1600, 1000, rule_graph_, TRUE);

	// プレイヤーに「ゲーム本編ではなく設定画面を開いている」と直感的に認識させるため、背景に暗く半透明の覆いを重ねる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(650, 100, "SETTINGS", GetColor(255, 255, 255), title_font_handle_);
}

void Rule::DrawMenu()
{
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
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: フェード状態の初期化と設定画面用BGMの再生
 */
void Rule::Initialize()
{
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	selected_index_ = kMenuBgm;
	scene_frames_ = 0;
	Master::mpSoundManager->PlayBGM(SoundManager::kBgmRule);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 動的生成したフォントハンドルの破棄およびBGMの停止
 */
void Rule::Finalize()
{
	DeleteFontToHandle(font_handle_);
	DeleteFontToHandle(title_font_handle_);
	Master::mpSoundManager->StopBGM();
}