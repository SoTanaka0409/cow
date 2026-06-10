#pragma once

// プレイヤーの獲得スコア、ランキング、ネーム入力（ネームエントリー）を管理・保存するクラス
class Score
{
public:
	// ランキング用のプレイヤー名と獲得スコアを保持する構造体
	struct RankData
	{
		char name[64];
		int score;
	};

	Score();
	~Score();

	/*
	 * @brief スコアオブジェクトの初期化を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] scoreの初期化
	 */
	void Initialize();

	const RankData& GetRanking(int index) const;

	/*
	 * @brief スコアを加算する（加算後が負になる場合は0に丸める）
	 * [入力] value: 加算するスコア値
	 * [出力] なし
	 * [副作用] scoreの加算
	 */
	void AddScore(int value);
	
	/*
	 * @brief スコアをゼロクリアする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] scoreを0に設定
	 */
	void ResetScore();

	int GetScore() const;
	const char* GetName() const;

	/*
	 * @brief タイムアップ後のプレイヤー名（ネームエントリー）の入力を開始する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] nameInputModeの有効化、キー入力ハンドルの新規作成とアクティブ化
	 */
	void StartNameInput();

	/*
	 * @brief プレイヤー名の入力受付を毎フレーム処理し、画面にテキストを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] playerName配列の更新、ENTERキー押下での入力モード終了
	 */
	void UpdateNameInput();

	bool IsNameInputFinished() const;

	/*
	 * @brief 獲得したスコアをランキングリストに登録する（上位3位以内に収まれば挿入）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ranking 配列の挿入・並び替え
	 */
	void AddRanking();

	/*
	 * @brief 現在のランキングデータを rank.txt に保存する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] rank.txt ファイルの書き込み・作成
	 */
	void SaveRanking();

	/*
	 * @brief 保存されたランキングデータを rank.txt から読み込む（存在しない場合は初期データを作成）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ranking 配列の更新、rank.txt がない場合は新規保存
	 */
	void LoadRanking();

	/*
	 * @brief ランキング上位3名をテキスト形式で指定座標に描画する（デバッグ用）
	 * [入力] x, y: 描画開始画面座標
	 * [出力] なし
	 * [副作用] 文字列描画コマンドの実行
	 */
	void DrawRanking(int x, int y);

	/*
	 * @brief 現在のゲーム結果スコアとランキングデータを score.txt に保存する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] score.txt ファイルの書き込み・作成
	 */
	void Save();

	/*
	 * @brief 前回のゲーム結果スコアとランキングデータを score.txt から読み込む
	 * [入力] なし
	 * [出力] なし
	 * [副作用] score の更新、ranking 配列の更新
	 */
	void Load();

	/*
	 * @brief プレイ中の画面左上に「SCORE」テキスト画像と現在のスコア数値を描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] バックバッファへの画像描画
	 */
	void Draw();

	/*
	 * @brief 整数値を画像フォントを用いて指定座標に描画する
	 * [入力] x, y: 描画開始座標, value: 描画する数値（負数対応）
	 * [出力] なし
	 * [副作用] バックバッファへの数字画像描画
	 */
	void DrawNumber(int x, int y, int value, float scale = 1.0f, int minDigits = 1);

	/*
	 * @brief コンボ数に応じたスコア倍率を適用してスコアを加算する
	 * [入力] baseScore: 基本スコア, comboCount: 現在のコンボ数
	 * [出力] なし
	 * [副作用] 倍率計算後のスコア加算
	 */
	void AddScoreWithCombo(int baseScore, int comboCount);

	// 遷移時受け渡し用のスタティックリザルトスコア管理
	static void SetResultScore(int value);
	static int GetResultScore();

private:
	int inputHandle;              // DxLibのネーム入力用キーボードインプットハンドル
	char playerName[64];          // プレイヤー名の入力テキストバッファ
	bool nameInputMode;           // 名前入力モードがアクティブかどうかのフラグ

	int score;                    // 現在プレイ中のスコア値

	int scoreImage;               // （未使用・予約）
	int numberImg[10];            // 0〜9 のデジタル数字画像アセットハンドル配列
	int minusImg;                 // マイナス記号の画像アセットハンドル

	int nameIndex;                // （未使用・予約）
	int scoreTextImage;           // 「SCORE」の文字画像アセットハンドル

	RankData ranking[3];          // ハイスコア上位3名のデータ構造体配列

	static int resultScore;       // ゲームシーン終了時点での獲得最終スコアスタティック変数
};
