#pragma once
#include"dxlib.h"
#include"Scene.h"
#include"Texture.h"
#include"SelectionManager.h"
#include"vector"
#include "Score.h"

// リザルト（結果表示）画面内の選択ボタン情報を表す構造体
struct ResultButton
{
	SelectionManager::Title type; // ボタンの識別子（タイトルへ戻る、ゲーム終了など）
	int graphHandle;              // 描画する画像アセットハンドル
	int x, y;                     // ボタン左上の描画開始座標
	int w, h;                     // 画像の横幅と縦幅
	bool isHover;                 // マウスカーソルがボタン領域内にあるかどうかのフラグ
};

// ゲーム制限時間終了後に遷移し、最終スコアとランキングボードを表示するシーンクラス
class ResultScene : public Scene
{
public:
	ResultScene();
	virtual ~ResultScene();

	/*
	 * @brief リザルトシーンのアセット初期設定（最新ランキング読み込み、フェード設定）を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ランキングデータの読み込み、フェードアルファ設定
	 */
	void Initialize() override;

	/*
	 * @brief 最終スコアテキスト、獲得数、およびランキングボードの描画
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面描画コマンドの実行
	 */
	void Draw() override;

	/*
	 * @brief 一定フレーム経過後（自動的にタイトル画面へ戻るため）のフェードアウト処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 経過時間カウントアップ、および自動タイトル遷移のフェード要求
	 */
	void Update() override;

	/*
	 * @brief リザルトシーン終了時のグラフィックリソース（スコア、ランキング画像、背景）解放
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ロードされた各種画像ハンドルの削除、BGMの停止
	 */
	void Finalize() override;

private:
	SelectionManager* mnTitleSelect; // 選択マネージャー（予約・未使用）
	Texture* mpTexture;              // テクスチャオブジェクト（予約・未使用）
	Texture* mpTexture2;
	Texture* mpTexture3;

	int mNewGameX;                   // ボタン座標・サイズ一時保持用（未使用）
	int mNewGameY;
	int mNewGameW;
	int mNewGameH;

	bool mbIsHoverNewGame;           // ホバー状態一時フラグ（未使用）

	std::vector<ResultButton> mButtons; // 画面上に配置されるリザルトボタンのリスト
	int ResultGraphHandle;              // リザルト背景の画像ハンドル

	Score mScore;                       // スコア計算・描画用のオブジェクトインスタンス

	int rankingTitleImage;              // ランキングボードヘッダー画像ハンドル
	int rankImage[3];                   // 1位、2位、3位のメダル等画像ハンドル

	int yourScoreImage;                 // スコアフレーム用画像ハンドル（未使用・yourScoreTextImgへ集約）
	int pointImage;                     // スコア単位「点」の画像ハンドル（未使用・pointImgへ集約）

	/*
	 * @brief ハイスコア上位3名のランキングUIボードを画面左上に描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ランキングボードヘッダーおよび各順位メダル・スコア値の描画
	 */
	void DrawRankingUI();

	int yourScoreTextImg;               // 「YOUR SCORE」のテキスト画像ハンドル
	int pointImg;                       // スコア単位「点」の画像ハンドル

	int mFrameCount;                    // フレームカウンター
	int mnCount;                        // シーン開始からの経過時間（フレーム数）カウンター
	bool mFlag;                         // フラグ変数（リザーブ）
};
