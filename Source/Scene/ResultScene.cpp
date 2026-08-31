#include "ResultScene.h"
#include "Master.h"
#include "GameConstants.h"
#include "SceneManager.h"

/// @brief UIリソースの読み込み、ボタンの初期化、リザルトBGMの再生
ResultScene::ResultScene()
{
	flag_ = true;
	count_ = 0;

	result_graph_handle_ = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kResult);
	rank_image_[0] = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kRank1);
	rank_image_[1] = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kRank2);
	rank_image_[2] = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kRank3);
	ranking_title_image_ = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kRankingTitle);
	your_score_text_img_ = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kScoreTitle);
	point_img_ = Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kPoint);

	// 暫定対応: リザルト画面でのボタン操作（タイトルへ戻る、ゲーム終了など）は現在無効化されているが、
	// UI描画レイアウトの崩れを防ぐため初期化のみ残す（期限：UIリファクタリング完了まで）
	UIButton newGameBtn;
	newGameBtn.Initialize(SelectionManager::Title::title, Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kBtnStart), 920, 50, 0.0f);
	buttons_.push_back(newGameBtn);

	UIButton exitBtn;
	exitBtn.Initialize(SelectionManager::Title::titleOUT, Master::resource_manager_->LoadGraphics(GameConstants::ImagePaths::kBtnExit), 920, 250, 1.5f);
	buttons_.push_back(exitBtn);

	Master::sound_manager_->PlayBGM(SoundManager::kBgmResult);
	Master::sound_manager_->SetBGMVolume(120);
	Master::score_manager_;
}

/// @brief 終了処理の呼び出し
ResultScene::~ResultScene()
{
	// シーン遷移時にBGMが鳴り続けたり、VRAMのメモリリークが発生するのを防ぐため、デストラクタで確実な破棄を保証する
	Finalize();
}

/// @brief フェード状態の初期化とローカルランキングの読み込み
void ResultScene::Initialize()
{
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	Master::score_manager_->LoadRanking();
}

/// @brief リザルト画面の全UIと最終スコアの描画
void ResultScene::Draw()
{
	DrawExtendGraph(0, 0, 1600, 900, result_graph_handle_, FALSE);

	DrawRankingUI();

	DrawExtendGraph(500, 300, 1100, 550, your_score_text_img_, TRUE);
	int score = Score::GetResultScore();
	int temp = score;
	int digitCount = 0;
	if (temp == 0) digitCount = 1;
	else
	{
		while (temp > 0)
		{
			temp /= 10;
			digitCount++;
		}
	}

	// スコアが0点などの場合でも、UI全体のレイアウト幅が崩れて見栄えが悪くなるのを防ぐため、最低4桁分の描画幅を担保する
	if (digitCount < 4) digitCount = 4;

	int startX = 850;
	if (digitCount > 3)
	{
		startX = 850 - (digitCount - 3) * 40;
	}

	Master::score_manager_->DrawNumber(startX, 490, score, 1.0f, 4);

	// スコアの桁数増減に合わせて「PT(ポイント)」画像のX座標を動的にずらし、数値と画像が被って見えなくなるバグを防ぐ
	int pointX = startX + digitCount * 80;
	DrawExtendGraph(pointX, 430, pointX + 200, 630, point_img_, TRUE);

	Scene::Draw();
	if (fade_state_ != kSceneFadeNone)
	{
		Scene::Fade(fade_state_);
	}
}

/// @brief シーン遷移タイマーの進行
void ResultScene::Update()
{
	count_++;

	// プレイヤーが操作せずとも自動でタイトルへ戻るアーケードゲーム風のUXを提供するため、約3秒(200F)で画面を遷移させる
	if (count_ >= 200 && fade_state_ != kSceneFadeOut)
	{
		fade_state_ = kSceneFadeOut;
		next_scene_ = SceneManager::kSceneTitle;
	}

	Scene::Update();
}

/// @brief 過去のトップ3ランキングのUI描画
void ResultScene::DrawRankingUI()
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
		const Score::RankData& data = Master::score_manager_->GetRanking(i);
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

		Master::score_manager_->DrawNumber(
			baseX + 180,
			drawY,
			data.score_,
			scale,
			4
		);

		int temp = data.score_;
		int digitCount = 0;
		if (temp == 0) digitCount = 1;
		else
		{
			while (temp > 0)
			{
				temp /= 10;
				digitCount++;
			}
		}

		// 自身のスコア描画時と同様に、レイアウト崩れ防止のため最低4桁分の描画余白を確保する
		if (digitCount < 4) digitCount = 4;
		int pointX = baseX + 180 + digitCount * w;
		DrawExtendGraph(pointX, drawY, pointX + w, drawY + h, point_img_, TRUE);
	}
}

/// @brief BGMの停止処理
void ResultScene::Finalize()
{
	Master::sound_manager_->StopBGM();
}
