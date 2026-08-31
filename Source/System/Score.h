#pragma once
class Score
{
public:
	struct RankData
	{
		char name_[64]; ///< 名前文字列を保持する値
		int score_;     ///< スコアや成長値の管理に使用する値
	};
	/// @brief インスタンス化の際の初期化処理のため。
	/// @details なし [出力] なし [副作用] メンバ変数の初期化
	Score();
	/// @brief メモリ解放と後処理のため。
	/// @details なし [出力] なし [副作用] リソースの解放
	~Score();
	/// @brief オブジェクト再利用時に前回の状態をリセットするため。
	/// @details なし [出力] なし [副作用] スコアと入力状態の初期化
	void Initialize();
	/// @brief 特定順位のランキングデータを取得するため。
	/// @param index 取得する順位 [出力] RankDataの参照 [副作用] なし
	const RankData& GetRanking(int index) const;
	/// @brief ゲーム進行に応じた評価を加味するため。
	/// @param value 加算スコア [出力] なし [副作用] score_の更新
	void AddScore(int value);

	/// @brief ゲームオーバー等でやり直すため。
	/// @details なし [出力] なし [副作用] score_を0に設定
	void ResetScore();
	/// @brief 現在のスコアを参照するため。
	/// @details なし [出力] 現在のスコア [副作用] なし
	int GetScore() const;
	/// @brief 入力されたプレイヤー名を参照するため。
	/// @details なし [出力] プレイヤー名文字列のポインタ [副作用] なし
	const char* GetName() const;
	/// @brief ランキング登録に向けたユーザー入力が必要なため。
	/// @details なし [出力] なし [副作用] キー入力モードの開始
	void StartNameInput();
	/// @brief ユーザーのキー入力を毎フレーム監視するため。
	/// @details なし [出力] なし [副作用] player_name_の更新
	/// @details 入力完了状態の変更
	void UpdateNameInput();
	/// @brief 名前入力が終了したか判定するため。
	/// @details なし [出力] 終了している場合はtrue [副作用] なし
	bool IsNameInputFinished() const;
	/// @brief 上位スコアを維持してプレイヤーの目標とするため。
	/// @details なし [出力] なし [副作用] ranking_の更新
	void AddRanking();
	/// @brief 次回起動時にもランキングを保持するため。
	/// @details なし [出力] なし [副作用] rank.txtへの書き込み
	void SaveRanking();
	/// @brief 過去のランキングデータを復元するため。
	/// @details なし [出力] なし [副作用] ranking_の更新
	/// @details ファイル不在時は初期化
	void LoadRanking();
	/// @brief ランキングを画面上に提示して結果を知らせるため。
	/// @param y 描画座標 [出力] なし [副作用] 画面描画
	/// @details x
	void DrawRanking(int x, int y);
	/// @brief 現在の進行状況を保存するため。
	/// @details なし [出力] なし [副作用] score.txtへの書き込み
	void Save();
	/// @brief 保存された進行状況から再開するため。
	/// @details なし [出力] なし [副作用] score_とranking_の更新
	void Load();
	/// @brief プレイ中にスコア状況を可視化するため。
	/// @details なし [出力] なし [副作用] 画面描画
	void Draw();
	/// @brief 数字を専用フォントで視覚的にわかりやすくするため。
	/// @param y 座標 value: 数値 scale: 拡大率 minDigits: 最小桁数 [出力] なし [副作用] 画面描画
	/// @details x
	void DrawNumber(int x, int y, int value, float scale = 1.0f, int minDigits = 1);

	/// @brief スコアと特定のアイコンを合わせて表示するため。
	/// @param y 座標 score: スコア scale: 拡大率 minDigits: 最小桁数 point_graph_handle: 画像ハンドル [出力] なし [副作用] 画面描画
	/// @details x
	void DrawScoreWithPoint(int x, int y, int score, float scale, int minDigits, int point_graph_handle);
	/// @brief 連続成功によるボーナス評価を与えるため。
	/// @param baseScore 基本値 combo_count_: コンボ数 [出力] なし [副作用] score_の更新
	void AddScoreWithCombo(int baseScore, int combo_count_);
	/// @brief シーン間でスコアを引き継ぐため。
	/// @param value スコア [出力] なし [副作用] result_score_の更新
	static void SetResultScore(int value);
	/// @brief シーン間で引き継いだスコアを参照するため。
	/// @details なし [出力] 最終獲得スコア [副作用] なし
	static int GetResultScore();
private:
	int input_handle_;        ///< 描画やリソース管理に使用するハンドル
	char player_name_[64];    ///< 名前文字列を保持する値
	bool name_input_mode_;    ///< 名前文字列を保持する値
	int score_;               ///< スコアや成長値の管理に使用する値
	int score_image_;         ///< 描画やリソース管理に使用するハンドル
	int number_img_[10];      ///< 内部状態を管理する値
	int minus_img_;           ///< 内部状態を管理する値
	int name_index_;          ///< 名前文字列を保持する値
	int score_text_image_;    ///< 描画やリソース管理に使用するハンドル
	RankData ranking_[3];     ///< スコアや成長値の管理に使用する値
	static int result_score_; ///< スコアや成長値の管理に使用する値
};
