#pragma once
#include "Scene.h"
#include"Texture.h"
#include "SelectionManager.h"
#include <vector>
#include "Score.h"
#include "SceneManager.h"

// タイトル画面のボタン情報と状態（位置、サイズ、ホバーなど）を保持する構造体
struct TitleButton
{
	SelectionManager::Title type; // ボタンの識別子（ゲーム開始、チュートリアルなど）
	int graphHandle;              // 描画する画像ハンドル
	int x, y;                     // ボタン左上の描画開始座標
	int w, h;                     // 画像の横幅と縦幅
	bool isHover;                 // マウスカーソルがボタン領域に乗っているかどうかの判定フラグ
};

// ゲーム起動時に最初に読み込まれるタイトル画面のシーンクラス
class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	/*
	 * @brief タイトルシーンのアセット初期設定（ランキング読み込み、タイトルBGM再生）を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] スコアの読み込み、BGMのループ再生開始
	 */
	virtual void Initialize() override;

	/*
	 * @brief マウス入力、ドラッグ、ボタンホバー、およびシーン遷移フェードの毎フレーム更新
	 * [入力] なし
	 * [出力] なし
	 * [副作用] マウス座標に基づく状態値、UFO座標、フェードアルファ値などの更新
	 */
	virtual void Update() override;

	/*
	 * @brief タイトル背景、ふわふわ動くUFO、各選択ボタン、およびランキングボードの描画
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面描画コマンドの実行
	 */
	virtual void Draw() override;

	/*
	 * @brief タイトルシーン終了時のリソース（背景画像、ボタン画像、ランキング画像）解放
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各種ロード画像ハンドルの削除、BGMの停止
	 */
	virtual void Finalize() override;

private:
	int mFrameCount;              // サイン波などを用いたふわふわ演出や定期タイマー用のフレームカウンター

	int mnTitleGraphHandle;       // タイトル背景画像ハンドル
	int mnNewGame;                // ゲーム開始ボタンの画像ハンドル（未使用・ボタン構造体へ移行）
	int mnTutorial;               // チュートリアルボタンの画像ハンドル（未使用・ボタン構造体へ移行）
	int mnOperationProcedures;    // 設定説明ボタンの画像ハンドル（未使用・ボタン構造体へ移行）

	int rankingTitleImage;        // ランキングボードのタイトル画像ハンドル

	SelectionManager* mnTitleSelect; // 選択マネージャー（未使用・ボタンリストへ移行）
	Texture* mpTexture;           // テクスチャオブジェクト（未使用・ボタン構造体へ移行）
	Texture* mpTexture2;
	Texture* mpTexture3;

	int mNewGameX;                // ゲーム開始ボタン初期配置X（未使用・ボタン構造体へ移行）
	int mNewGameY;                // ゲーム開始ボタン初期配置Y（未使用・ボタン構造体へ移行）
	int mNewGameW;                // ゲーム開始ボタンの横幅
	int mNewGameH;                // ゲーム開始ボタンの縦幅

	bool mbIsHoverNewGame;        // 開始ボタンにホバーしているか（未使用・ボタン構造体へ移行）

	std::vector<TitleButton> mButtons; // タイトル画面上のすべてのインタラクティブボタンのリスト

	/*
	 * @brief ハイスコア上位3名のランキングUIボードを画面左上に描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ランキング文字画像およびスコア数値の描画
	 */
	void DrawRankingUI();

	int rankImage[3];             // 1位、2位、3位のメダル等の画像ハンドル

	int mnUfoGraphHandle;         // タイトル画面でドラッグ可能なイースターエッグUFOの画像ハンドル
	int mUfoX, mUfoY;             // UFOの現在画面座標
	int mUfoW, mUfoH;             // UFO画像の幅と高さ
	bool mbIsDraggingUfo;         // マウス左クリックでUFOがドラッグ操作中かどうかのフラグ
	int mOffsetX, mOffsetY;       // ドラッグ時にクリックした箇所とUFO左上座標の相対オフセット値
	int mCowVoiceTimer;           // タイトル画面で定期的に牛の鳴き声を再生するための間隔タイマー
	float mUfoVX, mUfoVY;         // UFOの自動巡回時の移動速度ベクトル
	float mUfoAngle;              // UFOの自動円軌道巡回時の角度パラメータ
	bool  mIsAutoPatrol;          // クリックされた後に円軌道で自動巡回するモードの有効化フラグ
	int mAutoPatrolTimer;         // 自動巡回モードの有効時間カウンター
};
