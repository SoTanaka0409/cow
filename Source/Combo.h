#pragma once

// ボーナススコア計算やUI表示のためコンボ状態を管理する
class Combo
{
public:
	Combo();
	~Combo();

	/*
	 * @brief コンボ数とロゴをUIとして描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief コンボ継続判定のため毎フレーム状態を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 時間切れ時にReset()を呼び出しコンボ状態を破棄
	 */
	void Update();

	/*
	 * @brief 獲物獲得時にコンボ状態を更新・維持する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] comboCountをインクリメントし、タイマーを初期値に戻す
	 */
	void AddHit();

	/*
	 * @brief コンボ猶予時間超過などの理由で状態を初期化する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] comboCountを0にリセット
	 */
	void Reset();

	int GetCombo() const;
	float GetMultiplier() const;

private:
	int comboCount;             // スコア倍率計算の基準となる連続ヒット数
	float comboTimer;           // 0以下でコンボを終了させるための監視用タイマー
	float comboMaxTime;         // 難易度調整の基準となるコンボ維持猶予時間

	int comboImage;             // 描画負荷軽減のため保持しておくロゴ画像ハンドル
	bool comboShow;             // UIの表示/非表示を切り替えるためのフラグ
	float comboShowTimer;       // アニメーションなどの表示演出を管理する時間
	int numberImage[10];        // 描画負荷軽減のため保持しておく数字画像ハンドル配列
};

