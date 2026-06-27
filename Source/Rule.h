#pragma once
#include"Scene.h"

// プレイヤー環境に合わせた音量調整を提供するため、設定シーンを分離
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

	enum MenuType
	{
		MENU_BGM,
		MENU_SE,
		MENU_BACK,
		MENU_MAX
	};

private:
	int ruleGraph;
	int fontHandle;
	int titleFontHandle;

	MenuType selectedIndex = MENU_BGM;
	// 音量スライダー操作時にSEが毎フレーム鳴るのを防ぐためのインターバル
	int playSeDelay = 0;
	// シーン遷移直後の入力持ち越しによる誤動作を防ぐため
	int sceneFrames = 0;
};
