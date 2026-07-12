#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "GameConstants.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	// 鳴き声タイマー：一定時間ごとに牛の鳴き声を再生するためのカウンター
	cow_voice_timer_ = 180;

	title_graph_handle_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kTitleBg);
	ufo_graph_handle_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kTitleUfo);
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

		UIButton newGameBtn;
	newGameBtn.Initialize(SelectionManager::Title::NewGame, Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kBtnStart), 920, 50, 0.0f);
	buttons_.push_back(newGameBtn);

		UIButton tutorialBtn;
	tutorialBtn.Initialize(SelectionManager::Title::Tutorial, Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kBtnTutorial), 920, 250, 1.5f);
	buttons_.push_back(tutorialBtn);

		UIButton opeBtn;
	opeBtn.Initialize(SelectionManager::Title::OperationProcedures, Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kBtnSetting), 920, 450, 3.0f);
	buttons_.push_back(opeBtn);

		UIButton exitBtn;
	exitBtn.Initialize(SelectionManager::Title::titleOUT, Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kBtnExit), 920, 650, 4.5f);
	buttons_.push_back(exitBtn);

	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	next_scene_ = SceneManager::kSceneTitle;

	is_hover_new_game_ = false;
	frame_count_ = 0;

	// ランキング用のリソース読み込み。DxLibの初期化後に読み込む必要がある画像アセット
	rank_image_[0] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank1);
	rank_image_[1] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank2);
	rank_image_[2] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank3);
	ranking_title_image_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRankingTitle);
	point_img_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kPoint);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;
	SetMouseDispFlag(true); // タイトル画面ではマウスカーソルを表示（ゲーム中の誤操作を防ぐ設定の解除）
	Master::mpScore->LoadRanking(); // スコアデータをロードしてランキング配列へ格納
	Master::camera_->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmTitle);
}

void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, title_graph_handle_, FALSE);

	int ufoDrawY = ufo_y_;

	// UFOをドラッグしていない時は、サイン波（sin）を使って上下に浮遊するアニメーションを付ける
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

	// 各メニューボタンの描画。少しずつタイミングをずらしたサイン波で、ボタンをゆらゆらと上下させる
	for (int i = 0; i < buttons_.size(); i++)
	{
		float wave = sin(frame_count_ * 0.05f + (i * 1.5f)) * 10.0f;
		int drawY = buttons_[i].y + (int)wave;

		// マウスホバー時はボタンを少し拡大して描画
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

	if (frame_count_ == 2) {
		FILE* fp = NULL;
		fopen_s(&fp, "debug_log.txt", "a");
		if (fp) {
			int w, h;
			int res = GetGraphSize(title_graph_handle_, &w, &h);
			fprintf(fp, "TitleScene Draw: handle=%d, res=%d, w=%d, h=%d\n", title_graph_handle_, res, w, h);
			fclose(fp);
		}
	}

	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
}

void TitleScene::Update()
{
	frame_count_++;

	if (fade_state_ != kSceneFadeOut)
	{
		cow_voice_timer_--;
		if (cow_voice_timer_ <= 0)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeCow); // 一定時間経ったら牛の鳴き声SEを再生
			cow_voice_timer_ = GetRand(600) + 300; // 次に鳴くまでの時間をランダムで再設定（5?15秒程度）
		}
	}

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	// UFOをクリックした時の処理（隠し要素：一定時間自動で画面内をグルグル巡回するモードへ）
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouse_x_ >= ufo_x_ && mouse_x_ <= ufo_x_ + ufoSize &&
			mouse_y_ >= ufo_y_ && mouse_y_ <= ufo_y_ + ufoSize)
		{
			is_auto_patrol_ = true;
			auto_patrol_timer_ = 240; // 約4秒間（60fps想定で240フレーム）自動巡回させる
			is_dragging_ufo_ = true;
		}
	}
	else
	{
		is_dragging_ufo_ = false;
	}

	// 自動巡回モード中のUFOの座標計算（画面中央を中心に円軌道で移動）
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

	// 各メニューボタンのマウスクリックおよびホバー判定判定
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
						PostQuitMessage(0); // OSにアプリケーションの終了を通知
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
		// UFOを触っている時は、メニューボタンのホバー判定をすべて強制リセットする
		for (int i = 0; i < buttons_.size(); i++) {
			buttons_[i].is_hover = false;
		}
	}

	Scene::Update();
}

void TitleScene::Finalize()
{
	// ResourceManagerで管理しているためDeleteGraphは呼ばない

	SetMouseDispFlag(false); // ゲーム中の誤操作を防ぐためマウスを非表示化
	Master::mpSoundManager->StopBGM();
}

// ランキングUI描画：スコアデータを取得して順位画像と数字（点数）を画面左側に並べて描画
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

				Master::mpScore->DrawScoreWithPoint(
			baseX + 180,
			drawY,
			data.score_,
			scale,
			4,
			point_img_
		);
	}
}