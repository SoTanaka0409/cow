#pragma once
#include"dxlib.h"
#include "Scene.h"
#include "UIButton.h"
#include"Texture.h"
#include"SelectionManager.h"
#include"vector"
#include "Score.h"

/// @brief 設計ルール：ゲーム本編終了後に今回の最終スコアを表示し、歴代ハイスコアランキングを提示して次のアクション（再挑戦や終了）へ促すシーン
class ResultScene : public Scene
{
public:
	ResultScene();
	virtual ~ResultScene();

	/// @brief 今回の最終獲得スコアの登録、最新ランキングデータのロード、およびBGM再生
	void Initialize() override;

	/// @brief 最終スコアのポップ演出、ランクメダル、ハイスコアボード、各メニュー操作ボタンの描画
	void Draw() override;

	/// @brief ボタンに対するマウスクリック判定、および一定時間キー入力がない場合のタイトル画面自動遷移タイマーの更新
	void Update() override;

	/// @brief リザルト専用BGMの停止、ロードしたスプライト・テキストフォントハンドルの解放
	void Finalize() override;

private:
	/// @brief 画面レイアウトの崩れを防ぎ、決まったグリッド幅にスコアを表示するための動的配置テクスチャ
	Texture* texture_;  ///< 画面表示に使用するテクスチャ
	Texture* texture2_; ///< 画面表示に使用するテクスチャ
	Texture* texture3_; ///< 画面表示に使用するテクスチャ

	int new_game_w_; ///< 画面比率が変わった際もボタンの「ホバー判定エリア」を正しく一致させるための基準横幅
	int new_game_h_; ///< 画面比率が変わった際もボタンの「ホバー判定エリア」を正しく一致させるための基準縦幅

	bool is_hover_new_game_; ///< リトライボタンにマウスカーソルが乗った瞬間のハイライト表示用フラグ

	std::vector<UIButton> buttons_; ///< シーン内で選択可能なすべてのUIButton（再挑戦、タイトル、終了等）を管理する動的配列
	int result_graph_handle_;       ///< 画面全体の背景を彩る、高解像度のリザルト専用バックグラウンドテクスチャ

	Score score_; ///< スコアや成長値の管理に使用する値

	int ranking_title_image_; ///< ハイスコアボードの最上部に掲げる「RANKING」の見出しロゴ画像
	int rank_image_[3];       ///< 描画やリソース管理に使用するハンドル

	/// @brief 今回のスコアと歴代ベスト3のランキングを縦一列に画面左側へ描画
	void DrawRankingUI();

	int your_score_text_img_; ///< 今回の最終結果を強調するために描画する「YOUR SCORE」の飾り帯テキスト画像
	int point_img_;           ///< 数値（点数）の末尾に単位として付与する「pts」専用のフォントテクスチャ

	int frame_count_; ///< UIが出現する際のポップアップ（サイン波アニメーション）を動かすための経過フレーム数
	int count_;       ///< 放置された際にタイトルへ自動的に遷移させるためのタイムアウト計測用カウンター
	bool flag_;       ///< リザルト遷移直後の操作不能（ロック）期間を解除するための演出完了フラグ
};
