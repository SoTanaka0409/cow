#include"ResultScene.h"
#include"Master.h"
#include"SceneManager.h"

ResultScene::ResultScene()
{
	mFlag = true;
	mnCount = 0;
	
	// アセットリソースの一括ロード
	ResultGraphHandle = LoadGraph("Resource/2D/リザルト.png");
	rankImage[0] = LoadGraph("Resource/2D/1位.png");
	rankImage[1] = LoadGraph("Resource/2D/2位.png");
	rankImage[2] = LoadGraph("Resource/2D/3位.png");
	rankingTitleImage = LoadGraph("Resource/2D/ランキング.png");
	yourScoreTextImg = LoadGraph("Resource/2D/スコア.png");
	pointImg = LoadGraph("Resource/2D/点.png");

	// タイトルへ戻るボタンの設定
	ResultButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::title;
	newGameBtn.graphHandle = LoadGraph("Resource/2D/スタートボタン1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graphHandle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.isHover = false;
	mButtons.push_back(newGameBtn);

	// 終了ボタンの設定
	ResultButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graphHandle = LoadGraph("Resource/2D/しゅうりょうぼたん (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graphHandle, &exitBtn.w, &exitBtn.h);
	exitBtn.isHover = false;
	mButtons.push_back(exitBtn);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);
	Master::mpSoundManager->SetBGMVolume(120);
}

ResultScene::~ResultScene()
{
	Finalize(); // オブジェクト破棄時のメモリ解放漏れを防ぐ
}

void ResultScene::Initialize()
{
	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	Master::mpScore->LoadRanking();
}

void ResultScene::Draw()
{
	DrawExtendGraph(0, 0, 1600, 900, ResultGraphHandle, FALSE);

	DrawRankingUI();

	// プレイヤーの最終獲得スコアを描画
	DrawExtendGraph(500, 300, 1100, 550, yourScoreTextImg, TRUE);
	int score = Score::GetResultScore();
	Master::mpScore->DrawNumber(850, 490, score);
	DrawExtendGraph(1050, 430, 1250, 630, pointImg, TRUE);

	// プレイ中に回収した牛の総数を描画
	SetFontSize(40);
	DrawFormatString(500, 600, GetColor(0, 0, 0), "Cows Caught :");
	Master::mpScore->DrawNumber(850, 580, Master::mnCaughtCowCount);
	SetFontSize(16); // 元のフォントサイズ設定に復帰

	Scene::Draw();
	if (mFadeState != SceneFade_None) {
		Scene::Fade(mFadeState);
	}
}

void ResultScene::Update()
{
	mnCount++;

	// 200フレーム経過（約3.3秒）後、自動的にタイトル画面へ戻るフェードアウト処理を開始
	if (mnCount >= 200 && mFadeState != SceneFade_Out)
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_TITLE;
	}

	if (mFadeState == SceneFade_Out)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)mNextScene);
		}
		return;
	}

	Scene::Update();
}

/*
 * @brief スコアランキング情報の表示用UIを描画する
 * [入力] なし
 * [出力] なし
 * [副作用] 各順位の数字画像やスコアフォントの描画
 */
void ResultScene::DrawRankingUI()
{
	int baseX = 40;
	int baseY = 40;

	DrawExtendGraph(
		baseX - 50,
		baseY - 100,
		baseX + 420,
		baseY + 120,
		rankingTitleImage,
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
			rankImage[i],
			TRUE
		);

		Master::mpScore->DrawNumber(
			baseX + 180,
			y - 10,
			data.score
		);
	}
}

/*
 * @brief シーン終了時のアセット・グラフィック解放処理
 * [入力] なし
 * [出力] なし
 * [副作用] ロードされたすべての画像アセットの削除とBGMの停止
 */
void ResultScene::Finalize()
{
	DeleteGraph(ResultGraphHandle);

	for (int i = 0; i < mButtons.size(); i++)
	{
		DeleteGraph(mButtons[i].graphHandle);
	}

	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(rankImage[i]);
	}

	DeleteGraph(rankingTitleImage);
	DeleteGraph(yourScoreTextImg);
	DeleteGraph(pointImg);

	Master::mpSoundManager->StopBGM();
}
