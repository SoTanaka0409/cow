#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "GameConstants.h"
#include "InputManager.h"
#include "SelectionManager.h"

// 副作用：各種UIボタンの生成と配置、フェード状態の初期化
TitleScene::TitleScene()
	: Scene()
{
	// 賑やかしとして、タイトル画面で一定時間ごとに再生する環境音の間隔カウンター
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

	// 各メニュー項目を画面右側に縦一列に等間隔（200px）で並べるための座標初期化
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

	// ランキング描画用の各テクスチャ。DxLibの初期化完了後に呼び出す必要がある仕様上の制約
	rank_image_[0] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank1);
	rank_image_[1] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank2);
	rank_image_[2] = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRank3);
	ranking_title_image_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kRankingTitle);
	point_img_ = Master::mpResourceManager->LoadGraphics(GameConstants::ImagePaths::kPoint);
}

TitleScene::~TitleScene()
{
}

// 副作用：システムフラグ・カメラ設定・BGMの再生、ファイルからのスコア読み込み
void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;
	// メニュー選択操作を行うため、ゲーム本編中（非表示）と異なりカーソルを可視化する
	SetMouseDispFlag(true);
	Master::mpScore->LoadRanking();
	Master::camera_->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmTitle);
}

// 副作用：各種背景・UFO・ボタン・ランキングUIの描画、デバッグログの出力
void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, title_graph_handle_, FALSE);

	int ufoDrawY = ufo_y_;

	// 静止時の違和感を減らし生き生きと見せるため、非ドラッグ時は自動でサイン波浮遊させる
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

	// ボタン配置が単調になるのを防ぐため、サイン波の初期位相（i * 1.5f）をずらして波打たせる
	for (int i = 0; i < buttons_.size(); i++)
	{
		float wave = sin(frame_count_ * 0.05f + (i * 1.5f)) * 10.0f;
		int drawY = buttons_[i].y + (int)wave;

		// 選択中の視覚的なフィードバックを強調するため、ホバー時は中心から15px拡大する
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

	// 初期化直後のリソースバインド成否を検証するため、描画2フレーム目のみログを追記する
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

// 副作用：フレームカウンターの加算、各種タイマーの更新、入力状態に基づく座標変更
void TitleScene::Update()
{
	frame_count_++;

	if (fade_state_ != kSceneFadeOut)
	{
		cow_voice_timer_--;
		if (cow_voice_timer_ <= 0)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeCow);
			// 機械的な周期感を無くし自然な環境音にするため、次回鳴動までの間隔を5〜15秒で散らす
			cow_voice_timer_ = GetRand(600) + 300;
		}
	}

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	// イースターエッグ（隠し要素）として、UFO突っつき時に一定時間自動巡回モードへ移行させる
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouse_x_ >= ufo_x_ && mouse_x_ <= ufo_x_ + ufoSize &&
			mouse_y_ >= ufo_y_ && mouse_y_ <= ufo_y_ + ufoSize)
		{
			is_auto_patrol_ = true;
			auto_patrol_timer_ = 240;
			is_dragging_ufo_ = true;
		}
	}
	else
	{
		is_dragging_ufo_ = false;
	}

	// 画面中央（800, 450）を起点とした綺麗な長楕円の軌道を描かせるための極座標計算
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

	// メニューボタンのインタラクション処理。UFOドラッグ中は誤爆を防ぐため判定をパスする
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
						// Win32 API のメッセージループを終了させ、アプリケーションを閉じる
						PostQuitMessage(0);
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
		// UFO操作中の画面のちらつきや誤動作を防止するため、全ボタンのホバー演出を消去する
		for (int i = 0; i < buttons_.size(); i++) {
			buttons_[i].is_hover = false;
		}
	}

	Scene::Update();
}

// 副作用：マウスカーソル非表示化、BGMの停止
void TitleScene::Finalize()
{
	// グラフィックメモリは ResourceManager が一括管理・自動解放するため、個別Deleteは行わない
	SetMouseDispFlag(false);
	Master::mpSoundManager->StopBGM();
}

// 副作用：ランキングタイトル、上位3名の順位バッジおよびハイスコアの画面描画
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

	// ハイスコア上位3名分のデータを抽出し、等間隔（80px）で縦並びにする描画ループ
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

		// 画像フォントの等倍サイズ（80px）をベースに、UIレイアウトに適した縮尺へ調整
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
