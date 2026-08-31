#pragma once
#include"dxlib.h"
#include "Scene.h"
#include "UIButton.h"
#include"Texture.h"
#include"SelectionManager.h"
#include"vector"
#include "Score.h"

// リザルト画面における?ボタンのUI状態と描画チE?Eタを保持する構造体


// ゲーム終了??に最終スコアとランキングを表示するシーン
class ResultScene : public Scene
{
public:
	ResultScene();
	virtual ~ResultScene();

	// [入力] なし[出力] なし[副作用] 最新ランキングのロードとフェード?E期化
	void Initialize() override;

	// [入力] なし[出力] なし[副作用] 背景、スコア、ランキングUIの描画コマンド実?E
	void Draw() override;

	// [入力] なし[出力] なし[副作用] 一定時間経過後にタイトル画面への遷移を要汁E
	void Update() override;

	// [入力] なし[出力] なし[副作用] 使用した画像ハンドルの破?E??BGM停止
	void Finalize() override;

private:
	SelectionManager* title_select_;
	Texture* texture_;
	Texture* texture2_;
	Texture* texture3_;

	int new_game_x_;
	int new_game_y_;
	int new_game_w_;
	int new_game_h_;

	bool is_hover_new_game_;

	std::vector<UIButton> buttons_;
	int result_graph_handle_;

	Score score_;

	int ranking_title_image_;
	int rank_image_[3];

	int your_score_image_;
	int point_image_;

	// [入力] なし[出力] なし[副作用] 上?E名?Eスコアとメダルを描画
	void DrawRankingUI();

	int your_score_text_img_;
	int point_img_;

	int frame_count_; // UIアニメーション制御用タイマ?E
	int count_;     // 自動?E移用の経過時間タイマ?E
	bool flag_;
};

