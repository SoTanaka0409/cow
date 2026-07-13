#pragma once
class Score
{
public:
	struct RankData
	{
		char name_[64];
		int score_;
	};
	/*
	 * インスタンス化の際の初期化処理のため。
	 * [入力] なし [出力] なし [副作用] メンバ変数の初期化
	 */
	Score();
	/*
	 * メモリ解放と後処理のため。
	 * [入力] なし [出力] なし [副作用] リソースの解放
	 */
	~Score();
	/*
	 * オブジェクト再利用時に前回の状態をリセットするため。
	 * [入力] なし [出力] なし [副作用] スコアと入力状態の初期化
	 */
	void Initialize();
	/*
	 * 特定順位のランキングデータを取得するため。
	 * [入力] index: 取得する順位 [出力] RankDataの参照 [副作用] なし
	 */
	const RankData& GetRanking(int index) const;
	/*
	 * ゲーム進行に応じた評価を加味するため。
	 * [入力] value: 加算スコア [出力] なし [副作用] score_の更新
	 */
	void AddScore(int value);
	
	/*
	 * ゲームオーバー等でやり直すため。
	 * [入力] なし [出力] なし [副作用] score_を0に設定
	 */
	void ResetScore();
	/*
	 * 現在のスコアを参照するため。
	 * [入力] なし [出力] 現在のスコア [副作用] なし
	 */
	int GetScore() const;
	/*
	 * 入力されたプレイヤー名を参照するため。
	 * [入力] なし [出力] プレイヤー名文字列のポインタ [副作用] なし
	 */
	const char* GetName() const;
	/*
	 * ランキング登録に向けたユーザー入力が必要なため。
	 * [入力] なし [出力] なし [副作用] キー入力モードの開始
	 */
	void StartNameInput();
	/*
	 * ユーザーのキー入力を毎フレーム監視するため。
	 * [入力] なし [出力] なし [副作用] player_name_の更新、入力完了状態の変更
	 */
	void UpdateNameInput();
	/*
	 * 名前入力が終了したか判定するため。
	 * [入力] なし [出力] 終了している場合はtrue [副作用] なし
	 */
	bool IsNameInputFinished() const;
	/*
	 * 上位スコアを維持してプレイヤーの目標とするため。
	 * [入力] なし [出力] なし [副作用] ranking_の更新
	 */
	void AddRanking();
	/*
	 * 次回起動時にもランキングを保持するため。
	 * [入力] なし [出力] なし [副作用] rank.txtへの書き込み
	 */
	void SaveRanking();
	/*
	 * 過去のランキングデータを復元するため。
	 * [入力] なし [出力] なし [副作用] ranking_の更新、ファイル不在時は初期化
	 */
	void LoadRanking();
	/*
	 * ランキングを画面上に提示して結果を知らせるため。
	 * [入力] x, y: 描画座標 [出力] なし [副作用] 画面描画
	 */
	void DrawRanking(int x, int y);
	/*
	 * 現在の進行状況を保存するため。
	 * [入力] なし [出力] なし [副作用] score.txtへの書き込み
	 */
	void Save();
	/*
	 * 保存された進行状況から再開するため。
	 * [入力] なし [出力] なし [副作用] score_とranking_の更新
	 */
	void Load();
	/*
	 * プレイ中にスコア状況を可視化するため。
	 * [入力] なし [出力] なし [副作用] 画面描画
	 */
	void Draw();
	/*
	 * 数字を専用フォントで視覚的にわかりやすくするため。
	 * [入力] x,y: 座標 value: 数値 scale: 拡大率 minDigits: 最小桁数 [出力] なし [副作用] 画面描画
	 */
	void DrawNumber(int x, int y, int value, float scale = 1.0f, int minDigits = 1);
	
	/*
	 * スコアと特定のアイコンを合わせて表示するため。
	 * [入力] x, y: 座標 score: スコア scale: 拡大率 minDigits: 最小桁数 point_graph_handle: 画像ハンドル [出力] なし [副作用] 画面描画
	 */
	void DrawScoreWithPoint(int x, int y, int score, float scale, int minDigits, int point_graph_handle);
	/*
	 * 連続成功によるボーナス評価を与えるため。
	 * [入力] baseScore: 基本値 combo_count_: コンボ数 [出力] なし [副作用] score_の更新
	 */
	void AddScoreWithCombo(int baseScore, int combo_count_);
	/*
	 * シーン間でスコアを引き継ぐため。
	 * [入力] value: スコア [出力] なし [副作用] result_score_の更新
	 */
	static void SetResultScore(int value);
	/*
	 * シーン間で引き継いだスコアを参照するため。
	 * [入力] なし [出力] 最終獲得スコア [副作用] なし
	 */
	static int GetResultScore();
private:
	int input_handle_;
	char player_name_[64];
	bool name_input_mode_;
	int score_;
	int score_image_;
	int number_img_[10];
	int minus_img_;
	int name_index_;
	int score_text_image_;
	RankData ranking_[3];
	static int result_score_;
};
