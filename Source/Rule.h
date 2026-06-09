#pragma once
#include"Scene.h"

// ゲームの各種環境設定（BGM音量、SE音量など）の調整を行うシーンクラス
class Rule : public Scene
{
public:
	Rule();
	~Rule();

	/*
	 * @brief 設定画面表示用フォントや背景画像の読み込み、音量設定の読み込みを行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各種ハンドル初期化、設定画面専用BGMの再生開始
	 */
	void Initialize() override;

	/*
	 * @brief キー入力（上下キーで項目選択、左右キーで音量変更）およびマウス操作の処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 選択インデックス、基準音量設定(Master)の更新、BGM/SEの即時反映
	 */
	void Update() override;

	/*
	 * @brief 背景、タイトルロゴ、各設定バーおよび音量数値の描画
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面描画コマンドの実行
	 */
	void Draw() override;

	/*
	 * @brief 設定画面で使用した画像・フォントハンドルの一括解放
	 * [入力] なし
	 * [出力] なし
	 * [副作用] アセットのメモリ解放、BGMの停止
	 */
	void Finalize() override;

	// メニュー項目の識別ID
	enum MenuType
	{
		MENU_BGM,   // BGM音量調整
		MENU_SE,    // SE音量調整
		MENU_BACK,  // タイトル画面へ戻る選択肢
		MENU_MAX
	};

private:
	int mRuleGraph;                 // 設定画面背景の画像アセットハンドル
	int mFontHandle;                // 設定項目表示用のフォントハンドル
	int mTitleFontHandle;           // タイトルロゴ表示用のフォントハンドル

	MenuType mSelectedIndex = MENU_BGM; // 現在選択中の設定項目ID
	int mPlaySeDelay = 0;           // 音量変更時の確認SE連打防止用タイマー
	int mSceneFrames = 0;           // 誤入力防止用（遷移直後のフレームカウンター）
};
