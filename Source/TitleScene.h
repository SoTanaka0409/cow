#pragma once
#include "Scene.h"
#include"Texture.h"
#include "SelectionManager.h"
#include <vector>
#include "Score.h"
#include "SceneManager.h"

// UI状態管琁E��描画用のチE�Eタをまとめるための構造佁E
struct TitleButton
{
	SelectionManager::Title type;
	int graph_handle;
	int x, y;
	int w, h;
	bool is_hover;
};

// アプリケーション起動時の初期画面
class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	// [入力] なぁE[出力] なぁE[副作用] アセチE��初期化、スコア読み込み、BGM再生
	virtual void Initialize() override;

	// [入力] なぁE[出力] なぁE[副作用] マウス入力やUI状態、フェード更新
	virtual void Update() override;

	// [入力] なぁE[出力] なぁE[副作用] 背景やUI、UFOの描画コマンド発衁E
	virtual void Draw() override;

	// [入力] なぁE[出力] なぁE[副作用] リソースの解放、BGM停止
	virtual void Finalize() override;

private:
	int frame_count_; // 演�Eアニメーション用のタイマ�E

	int title_graph_handle_;
	int new_game_;
	int tutorial_;
	int operation_procedures_;

	int ranking_title_image_;
	int point_img_;

	SelectionManager* title_select_;
	Texture* texture_;
	Texture* texture2_;
	Texture* texture3_;

	int new_game_x_;
	int new_game_y_;
	int new_game_w_;
	int new_game_h_;

	bool is_hover_new_game_;

	std::vector<TitleButton> buttons_;

	// [入力] なぁE[出力] なぁE[副作用] 上佁E名�EスコアUIを描画
	void DrawRankingUI();

	int rank_image_[3];

	int ufo_graph_handle_;
	int ufo_x_, ufo_y_;
	int ufo_w_, ufo_h_;
	bool is_dragging_ufo_;
	int offset_x_, offset_y_;
	int cow_voice_timer_;
	float ufo_vx_, ufo_vy_;
	float ufo_angle_;
	bool  is_auto_patrol_;
	int auto_patrol_timer_;
};
