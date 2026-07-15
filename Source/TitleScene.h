#pragma once
#include "Scene.h"
#include "UIButton.h"
#include "Texture.h"
#include "SelectionManager.h"
#include <vector>
#include "Score.h"
#include "SceneManager.h"

// 設計ルール：ゲーム起動時にプレイヤーが最初に遷移し、ゲーム本編・チュートリアル・終了を選択する受付画面
class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	// 入力：なし
	// 出力：なし
	// 副作用：アセット初期化、ランキングデータ読み込み、タイトルBGM再生開始
	virtual void Initialize() override;

	// 入力：なし
	// 出力：なし
	// 副作用：マウス入力判定、メニューUIホバー状態の更新、イースターエッグ（UFO自動巡回）のタイマー更新
	virtual void Update() override;

	// 入力：なし
	// 出力：なし
	// 副作用：背景、ゆらゆらと動くメニューボタン、ハイスコアボード、UFOアニメーションの画面描画
	virtual void Draw() override;

	// 入力：なし
	// 出力：なし
	// 副作用：マウス表示フラグの復旧、タイトルBGMの停止
	virtual void Finalize() override;

private:
	void DrawBackground();
	void DrawMenuButtons();


	// 画面全体の共通演出（サイン波によるボタンやUFOの浮遊アニメーション）の位相を同期させるための共通タイマー
	int frame_count_;

	int title_graph_handle_;

	int ranking_title_image_;
	int point_img_;

	Texture* texture_;
	Texture* texture2_;
	Texture* texture3_;

	int new_game_w_;
	int new_game_h_;

	bool is_hover_new_game_;

	std::vector<UIButton> buttons_;

	// 入力：なし
	// 出力：なし
	// 副作用：ハイスコア上位3名のデータと順位アイコンの画面描画
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

	// イースターエッグ（UFOクリック時）として、画面中央を一定時間自動でぐるぐる回る演出用の制御変数群
	bool  is_auto_patrol_;
	int auto_patrol_timer_;

	void UpdateCowVoice();
	void UpdateUFOInteraction(int mouseInput, int mouse_x, int mouse_y, int ufoSize);
	void UpdateUFOAutoPatrol(int ufoSize);
	void UpdateMenuButtons(int mouse_x, int mouse_y);
};
