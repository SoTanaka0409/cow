#pragma once
#include"dxlib.h"
#include"Scene.h"
#include"Texture.h"
#include"SelectionManager.h"
#include"vector"
#include "Score.h"

// リザルト画面におけるボタンのUI状態と描画データを保持する構造体
struct ResultButton
{
	SelectionManager::Title type;
	int graphHandle;
	int x, y;
	int w, h;
	bool isHover;
};

// ゲーム終了時に最終スコアとランキングを表示するシーン
class ResultScene : public Scene
{
public:
	ResultScene();
	virtual ~ResultScene();

	// [入力] なし [出力] なし [副作用] 最新ランキングのロードとフェード初期化
	void Initialize() override;

	// [入力] なし [出力] なし [副作用] 背景、スコア、ランキングUIの描画コマンド実行
	void Draw() override;

	// [入力] なし [出力] なし [副作用] 一定時間経過後にタイトル画面への遷移を要求
	void Update() override;

	// [入力] なし [出力] なし [副作用] 使用した画像ハンドルの破棄とBGM停止
	void Finalize() override;

private:
	SelectionManager* mnTitleSelect;
	Texture* mpTexture;
	Texture* mpTexture2;
	Texture* mpTexture3;

	int mNewGameX;
	int mNewGameY;
	int mNewGameW;
	int mNewGameH;

	bool mbIsHoverNewGame;

	std::vector<ResultButton> mButtons;
	int ResultGraphHandle;

	Score mScore;

	int rankingTitleImage;
	int rankImage[3];

	int yourScoreImage;
	int pointImage;

	// [入力] なし [出力] なし [副作用] 上位3名のスコアとメダルを描画
	void DrawRankingUI();

	int yourScoreTextImg;
	int pointImg;

	int mFrameCount; // UIアニメーション制御用タイマー
	int mnCount;     // 自動遷移用の経過時間タイマー
	bool mFlag;
};

