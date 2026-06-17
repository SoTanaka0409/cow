#pragma once

// プレイヤーのスコア、ランキング、ネーム入力を管理する
class Score
{
public:
	// ランキング保持用
	struct RankData
	{
		char name[64];
		int score;
	};

	Score();
	~Score();

	/*
	 * @brief スコア初期化
	 * [入力] なし [出力] なし [副作用] scoreの初期化
	 */
	void Initialize();

	const RankData& GetRanking(int index) const;

	/*
	 * @brief 負数にならないようスコアを加算
	 * [入力] value: 加算スコア [出力] なし [副作用] scoreの更新
	 */
	void AddScore(int value);
	
	/*
	 * @brief スコアリセット
	 * [入力] なし [出力] なし [副作用] scoreを0に設定
	 */
	void ResetScore();

	int GetScore() const;
	const char* GetName() const;

	/*
	 * @brief タイムアップ後のネームエントリー開始
	 * [入力] なし [出力] なし [副作用] キー入力ハンドルの有効化
	 */
	void StartNameInput();

	/*
	 * @brief ネームエントリーの毎フレーム処理
	 * [入力] なし [出力] なし [副作用] playerNameの更新、ENTERでの終了
	 */
	void UpdateNameInput();

	bool IsNameInputFinished() const;

	/*
	 * @brief スコアをランキング(上位3位)に登録
	 * [入力] なし [出力] なし [副作用] rankingの更新
	 */
	void AddRanking();

	/*
	 * @brief ランキングをrank.txtに保存
	 * [入力] なし [出力] なし [副作用] rank.txtへの書き込み
	 */
	void SaveRanking();

	/*
	 * @brief rank.txtからランキングを読み込み（不在時は初期化）
	 * [入力] なし [出力] なし [副作用] rankingの更新、rank.txtの作成
	 */
	void LoadRanking();

	/*
	 * @brief ランキングのデバッグ描画
	 * [入力] x, y: 描画座標 [出力] なし [副作用] 画面描画
	 */
	void DrawRanking(int x, int y);

	/*
	 * @brief 現在のスコアとランキングをscore.txtに保存
	 * [入力] なし [出力] なし [副作用] score.txtへの書き込み
	 */
	void Save();

	/*
	 * @brief score.txtから前回のスコアとランキングを読み込み
	 * [入力] なし [出力] なし [副作用] scoreとrankingの更新
	 */
	void Load();

	/*
	 * @brief プレイ中のスコア描画
	 * [入力] なし [出力] なし [副作用] 画面描画
	 */
	void Draw();

	/*
	 * @brief 任意の数値を画像フォントで描画
	 * [入力] x,y: 座標, value: 数値 [出力] なし [副作用] 画面描画
	 */
	void DrawNumber(int x, int y, int value, float scale = 1.0f, int minDigits = 1);

	/*
	 * @brief コンボ数に応じた倍率でスコア加算
	 * [入力] baseScore: 基本値, comboCount: コンボ数 [出力] なし [副作用] scoreの更新
	 */
	void AddScoreWithCombo(int baseScore, int comboCount);

	// シーン間引き継ぎ用
	static void SetResultScore(int value);
	static int GetResultScore();

private:
	int inputHandle;              // DxLibネーム入力用
	char playerName[64];          // 入力テキストバッファ
	bool nameInputMode;           // 入力モードフラグ

	int score;                    // 現在のスコア

	int scoreImage;               // 予約
	int numberImg[10];            // 数字画像ハンドル(0-9)
	int minusImg;                 // マイナス画像ハンドル

	int nameIndex;                // 予約
	int scoreTextImage;           // 「SCORE」画像ハンドル

	RankData ranking[3];          // ハイスコア上位3名

	static int resultScore;       // 最終獲得スコア
};
