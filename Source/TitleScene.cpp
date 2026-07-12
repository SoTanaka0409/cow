#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	cow_voice_timer_ = 180; // 初回再生までの征E��フレーム設宁E
	
	title_graph_handle_ = Master::mpResourceManager->LoadGraphics("Resource/2D/タイトルシーン.png");
	ufo_graph_handle_ = Master::mpResourceManager->LoadGraphics("Resource/2D/牛とUFO.png");
	GetGraphSize(ufo_graph_handle_, &ufo_w_, &ufo_h_);

	ufo_x_ = 540;
	ufo_y_ = 30;

	is_dragging_ufo_ = false;
	offset_x_ = 0;
	offset_y_ = 0;

	ufo_vx_ = 2.0f;
	ufo_vy_ = 1.5f;

	ufo_angle_ = 0.0f;
	is_auto_patrol_ = false;
	auto_patrol_timer_ = 0;

	TitleButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::NewGame;
	newGameBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graph_handle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.is_hover = false;
	buttons_.push_back(newGameBtn);

	TitleButton tutorialBtn;
	tutorialBtn.type = SelectionManager::Title::Tutorial;
	tutorialBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/チュートリアルぼたん.png");
	tutorialBtn.x = 900;
	tutorialBtn.y = 250;
	GetGraphSize(tutorialBtn.graph_handle, &tutorialBtn.w, &tutorialBtn.h);
	tutorialBtn.is_hover = false;
	buttons_.push_back(tutorialBtn);

	TitleButton opeBtn;
	opeBtn.type = SelectionManager::Title::OperationProcedures;
	opeBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/せっていぼたん1.png");
	opeBtn.x = 915;
	opeBtn.y = 450;
	GetGraphSize(opeBtn.graph_handle, &opeBtn.w, &opeBtn.h);
	opeBtn.is_hover = false;
	buttons_.push_back(opeBtn);

	TitleButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/しゅうりょうぼたん (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graph_handle, &exitBtn.w, &exitBtn.h);
	exitBtn.is_hover = false;
	buttons_.push_back(exitBtn);

	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	next_scene_ = SceneManager::kSceneTitle;

	is_hover_new_game_ = false;
	frame_count_ = 0;

	// ランキング表示用の画像をロード！ExLib初期化後でなぁE��失敗するためここで行う�E�E
	rank_image_[0] = Master::mpResourceManager->LoadGraphics("Resource/2D/1位.png");
	rank_image_[1] = Master::mpResourceManager->LoadGraphics("Resource/2D/2位.png");
	rank_image_[2] = Master::mpResourceManager->LoadGraphics("Resource/2D/3位.png");
	ranking_title_image_ = Master::mpResourceManager->LoadGraphics("Resource/2D/ランキング.png");
	point_img_ = Master::mpResourceManager->LoadGraphics("Resource/2D/点.png");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;
	SetMouseDispFlag(true); // ユーザーが�Eタン操作できるようにマウスを表示
	Master::mpScore->LoadRanking(); // ランキング表示用に保存されたスコアをローチE
	Master::camera_->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmTitle);
}

void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, title_graph_handle_, FALSE);

	int ufoDrawY = ufo_y_;

	// ドラチE��中以外�Eサイン波でUFOの浮遊アニメーションを適用する
	if (!is_dragging_ufo_)
	{
		float ufoWave = sin(frame_count_ * 0.03f) * 20.0f;
		ufoDrawY += (int)ufoWave;
	}

	int ufoSize = 360;
	DrawExtendGraph(
		ufo_x_,
		ufoDrawY,
		ufo_x_ + ufoSize,
		ufoDrawY + ufoSize,
		ufo_graph_handle_,
		TRUE
	);

	// ホバー時にボタンを拡大描画し、視覚的なフィードバチE��を与えめE
	for (int i = 0; i < buttons_.size(); i++)
	{
		float wave = sin(frame_count_ * 0.05f + (i * 1.5f)) * 10.0f;
		int drawY = buttons_[i].y + (int)wave;

		if (buttons_[i].is_hover == true)
		{
			int expand = 15;
			DrawExtendGraph(
				buttons_[i].x - expand,
				drawY - expand,
				buttons_[i].x + buttons_[i].w + expand,
				drawY + buttons_[i].h + expand,
				buttons_[i].graph_handle, TRUE);
		}
		else
		{
			DrawGraph(buttons_[i].x, drawY, buttons_[i].graph_handle, TRUE);
		}
	}

	
	DrawRankingUI();
	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
	
	// DEBUG
	DrawFormatString(10, 10, GetColor(255, 0, 0), "TitleHandle: %d, Fade: %f, Res: %d", title_graph_handle_, GetFadeAlpha(), Master::mpResourceManager->GetTotalResource());
}

void TitleScene::Update()
{
	frame_count_++;

	if (fade_state_ != kSceneFadeOut)
	{
		cow_voice_timer_--;
		if (cow_voice_timer_ <= 0)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeCow); // タイトル画面の演�Eとして定期皁E��再生
			cow_voice_timer_ = GetRand(600) + 300; // 再生間隔をランダム化し単調さを防ぁE
		}
	}

	if (fade_state_ == kSceneFadeOut)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
		}
		return;
	}

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	// UFOクリチE��時に隠し要素の自動巡回モードを起動すめE
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouse_x_ >= ufo_x_ && mouse_x_ <= ufo_x_ + ufoSize &&
			mouse_y_ >= ufo_y_ && mouse_y_ <= ufo_y_ + ufoSize)
		{
			is_auto_patrol_ = true;
			auto_patrol_timer_ = 240; // 4秒間�E�E40フレーム�E��E動巡回を行う
			is_dragging_ufo_ = true;
		}
	}
	else
	{
		is_dragging_ufo_ = false;
	}

	// 楕�E軌道の数式を用ぁE��UFOを�E動巡回させる
	if (is_auto_patrol_)
	{
		ufo_angle_ += 0.02f;
		ufo_x_ = 800 - (ufoSize / 2) + (int)(cos(ufo_angle_) * 400.0f);
		ufo_y_ = 450 - (ufoSize / 2) + (int)(sin(ufo_angle_) * 200.0f);

		auto_patrol_timer_--;
		if (auto_patrol_timer_ <= 0)
		{
			auto_patrol_timer_ = 0;
			is_auto_patrol_ = false;
		}
	}

	// UFO操作中以外にボタンのマウス判定とシーン遷移要求を行う
	if (is_dragging_ufo_ == false)
	{
		for (int i = 0; i < buttons_.size(); i++)
		{
			if (mouse_x_ >= buttons_[i].x && mouse_x_ <= buttons_[i].x + buttons_[i].w &&
				mouse_y_ >= buttons_[i].y && mouse_y_ <= buttons_[i].y + buttons_[i].h)
			{
				buttons_[i].is_hover = true;

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					switch (buttons_[i].type)
					{
					case SelectionManager::Title::NewGame:
						next_scene_ = SceneManager::kSceneLoading;
						fade_state_ = kSceneFadeOut;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::Tutorial:
						next_scene_ = SceneManager::kSceneTutorial;
						fade_state_ = kSceneFadeOut;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::OperationProcedures:
						next_scene_ = SceneManager::kSceneRule;
						fade_state_ = kSceneFadeOut;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::titleOUT:
						PostQuitMessage(0); // OSにアプリケーション終亁E��求を送信
						break;
					}
				}
			}
			else
			{
				buttons_[i].is_hover = false;
			}
		}
	}
	else
	{
		// UFOドラチE��中のボタン誤反応を防ぐため�Eバ�E状態を解除
		for (int i = 0; i < buttons_.size(); i++) {
			buttons_[i].is_hover = false;
		}
	}

	Scene::Update();
}

void TitleScene::Finalize()
{
	SetMouseDispFlag(false); // ゲーム中の誤操作を防ぐためマウスを非表示化
	Master::mpSoundManager->StopBGM();
}

// [入力] なぁE[出力] なぁE[副作用] 画面上にランキング数値とアイコンを描画
void TitleScene::DrawRankingUI()
{
	int baseX = 40;
	int baseY = 40;

	DrawExtendGraph(
		baseX - 50,
		baseY - 100,
		baseX + 420,
		baseY + 120,
		ranking_title_image_,
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
			rank_image_[i],
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
		DrawExtendGraph(pointX, drawY, pointX + w, drawY + h, point_img_, TRUE);
	}
}
