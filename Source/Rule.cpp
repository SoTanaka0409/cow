#include "Scene.h"
#include "Rule.h"
#include "DxLib.h"
#include "Master.h"
#include "GameConstants.h"
#include "SceneManager.h"
#include "InputManager.h"

// 入力：なし
// 出力：なし
// 副作用：設定画面で使用する専用背景テクスチャおよび各種UIフォントのVRAMロード
Rule::Rule()
{
	rule_graph_ = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kSettingsBg);
	title_font_handle_ = CreateFontToHandle("メイリオ", 80, 5);
	font_handle_ = CreateFontToHandle("メイリオ", 50, 3);

	selected_index_ = kMenuBgm;
	play_se_delay_ = 0;
}

Rule::~Rule()
{
	// メモリ管理：シーン切り替えが何度も行われた際に、DxLib内部の動的フォントハンドルがVRAMに蓄積して起きるリークを防止
	Finalize();
}

// 入力：なし
// 出力：なし
// 副作用：フレームカウントの進行、およびマウス・キーボード入力に同期した設定項目の更新
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

	// UI設計：前シーン（タイトル等）での決定クリックの入力残響が即座に反映され、予期せぬ誤操作を引き起こすバグを防ぐチャタリング防止猶予
	if (scene_frames_ < 30)
	{
		prevMouseInput = mouseInput;
		return;
	}

	UpdateMenu(mouse_x_, mouse_y_, mouseInput, isMouseClicked, isMouseHeld);

	prevMouseInput = mouseInput;
}

// 入力：なし
// 出力：フェードアウト処理中の場合は true、それ以外は false
// 副作用：BGM音量の線形減衰、およびフェード完了時のシーンマネージャーへの遷移リクエスト
bool Rule::UpdateFadeState()
{
	if (fade_state_ == kSceneFadeOut)
	{
		// 演出仕様：無音のタイトル画面へ急に遷移する際の聴覚的違和感を和らげるため、画面の暗転率（Alpha）に同期させて設定BGMをフェードアウト
		Master::sound_manager_->SetBGMVolume((Master::sound_manager_->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::scene_manager_->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
			return true;
		}
	}
	return false;
}

// 入力：mouse_x, mouse_y = 現在のマウス座標, mouseInput = 現在のクリック状態, isMouseClicked = 1フレームクリック判定, isMouseHeld = 押し続け判定
// 出力：なし
// 副作用：ボリュームの書き換え、変更音SEのトリガー、フェードアウトステートへの移行処理
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
						Master::sound_manager_->SetMasterBGMVolume(newVol);
					}
					else if (selected_index_ == kMenuSe)
					{
						Master::sound_manager_->SetMasterSEVolume(newVol);

						// 聴覚保護：ドラッグスクロール中にSE再生API（PlaySE）が毎フレーム重複して暴発し、音割れやクラッシュを引き起こすのを抑止
						if (play_se_delay_ <= 0)
						{
							Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
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
			Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
			fade_state_ = kSceneFadeOut;
			next_scene_ = SceneManager::kSceneTitle;
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_UP) || InputManager::CheckDownKey(KEY_INPUT_W))
	{
		selected_index_ = (MenuType)((int)selected_index_ - 1);
		if (selected_index_ < 0) selected_index_ = (MenuType)(kMenuMax - 1);
		Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN) || InputManager::CheckDownKey(KEY_INPUT_S))
	{
		selected_index_ = (MenuType)((int)selected_index_ + 1);
		if (selected_index_ >= kMenuMax) selected_index_ = kMenuBgm;
		Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
	}

	int volChange = 0;
	if (InputManager::CheckPressKey(KEY_INPUT_LEFT) || InputManager::CheckPressKey(KEY_INPUT_A))
	{
		volChange = -2;
	}
	if (InputManager::CheckPressKey(KEY_INPUT_RIGHT) || InputManager::CheckPressKey(KEY_INPUT_D))
	{
		volChange = 2;
	}

	if (volChange != 0)
	{
		if (selected_index_ == kMenuBgm)
		{
			int currentVol = Master::sound_manager_->GetMasterBGMVolume();
			Master::sound_manager_->SetMasterBGMVolume(currentVol + volChange);
		}
		else if (selected_index_ == kMenuSe)
		{
			int currentVol = Master::sound_manager_->GetMasterSEVolume();
			Master::sound_manager_->SetMasterSEVolume(currentVol + volChange);

			// マウスドラッグ操作時と同様、キー押しっぱなしによる短時間での大量のSE多重再生を制限するインターバル
			if (play_se_delay_ <= 0)
			{
				Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
				play_se_delay_ = 10;
			}
		}
	}

	if (InputManager::CheckDownKey(KEY_INPUT_RETURN) || InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (selected_index_ == kMenuShadow)
		{
			// 決定キーで影のON/OFFを反転させ、SE再生でフィードバックを与える
			Master::is_shadow_enabled_ = !Master::is_shadow_enabled_;
			Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
		}
		else if (selected_index_ == kMenuBack)
		{
			Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
			fade_state_ = kSceneFadeOut;
			next_scene_ = SceneManager::kSceneTitle;
		}
	}

	// マウスクリックでも影のON/OFFを切り替えられるようにする
	if (isMouseClicked && selected_index_ == kMenuShadow)
	{
		Master::is_shadow_enabled_ = !Master::is_shadow_enabled_;
		Master::sound_manager_->PlaySE(SoundManager::kSeDecide);
	}
}

// 入力：なし
// 出力：なし
// 副作用：暗転背景、ボリュームスライダー、各種文字項目のバックバッファへの転送
void Rule::Draw()
{
	DrawBackground();
	DrawMenu();

	Scene::Draw();

	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
}

// 入力：なし
// 出力：なし
// 副作用：背景の拡大描画、半透明の暗幕矩形描画、およびタイトルロゴテキストの描画
void Rule::DrawBackground()
{
	DrawExtendGraph(0, -100, 1600, 1000, rule_graph_, TRUE);

	// 視認性確保：明るい色調の背景画像がロードされた場合でも、重ねて描画される白いフォントや黄色の選択マーカーが潰れないように遮光する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(650, 100, "SETTINGS", GetColor(255, 255, 255), title_font_handle_);
}

// 入力：なし
// 出力：なし
// 副作用：選択状態に連動したカーソル記号、および各メニュー項目の描画
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
			int vol = Master::sound_manager_->GetMasterBGMVolume();
			DrawFormatStringToHandle(startX, y, color, font_handle_, "BGM Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, font_handle_, "%3d", (vol * 100) / 255);
		}
		else if (i == kMenuSe)
		{
			int vol = Master::sound_manager_->GetMasterSEVolume();
			DrawFormatStringToHandle(startX, y, color, font_handle_, "SE Volume");
			DrawBox(startX + 350, y + 15, startX + 350 + (vol * 2), y + 45, color, TRUE);
			DrawBox(startX + 350, y + 15, startX + 350 + (255 * 2), y + 45, GetColor(255, 255, 255), FALSE);
			DrawFormatStringToHandle(startX + 880, y, color, font_handle_, "%3d", (vol * 100) / 255);
		}
		else if (i == kMenuShadow)
		{
			// 現在の影の状態を文字列で表示し、クリック or Enterで切り替えできることを示す
			const char* shadowState = Master::is_shadow_enabled_ ? "[ ON  ]" : "[ OFF ]";
			unsigned int stateColor = Master::is_shadow_enabled_ ? GetColor(0, 255, 100) : GetColor(180, 180, 180);
			DrawFormatStringToHandle(startX, y, color, font_handle_, "Shadow");
			DrawFormatStringToHandle(startX + 370, y, stateColor, font_handle_, shadowState);
		}
		else if (i == kMenuBack)
		{
			DrawFormatStringToHandle(startX, y, color, font_handle_, "Back to Title");
		}
	}
}

// 入力：なし
// 出力：なし
// 副作用：フェード状態の初期化、選択インデックスの規定、および設定画面用BGM（kBgmRule）の再生開始
void Rule::Initialize()
{
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	selected_index_ = kMenuBgm;
	scene_frames_ = 0;
	Master::sound_manager_->PlayBGM(SoundManager::kBgmRule);
}

// 入力：なし
// 出力：なし
// 副作用：作成したフォントハンドルの破棄およびBGM再生の即時停止
void Rule::Finalize()
{
	DeleteFontToHandle(font_handle_);
	DeleteFontToHandle(title_font_handle_);
	Master::sound_manager_->StopBGM();
}
