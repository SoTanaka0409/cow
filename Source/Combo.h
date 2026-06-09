#pragma once

// 連続で獲物を獲得した際にボーナススコアを付与するコンボシステムクラス
class Combo
{
public:
	Combo();
	~Combo();

	/*
	 * @brief 現在のコンボ数を画面左側などにデジタル数字画像で描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief コンボ有効期限タイマーの減算および時間切れ時のリセット監視を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] タイマー経過によるResetの呼び出し
	 */
	void Update();

	/*
	 * @brief コンボヒット数を1加算し、有効維持タイマーを最大値(3秒)にリセットする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コンボカウント(comboCount)加算、タイマー回復
	 */
	void AddHit();

	/*
	 * @brief コンボを終了（リセット）し、カウントを0にする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] comboCountを0にリセット
	 */
	void Reset();

	int GetCombo() const;
	float GetMultiplier() const;

private:
	int comboCount;             // 現在の連続ヒット数
	float comboTimer;           // コンボ維持用の残り時間タイマー
	float comboMaxTime;         // コンボ維持の最大規定時間（3秒）

	int comboImage;             // "COMBO" ロゴのグラフィックハンドル
	bool comboShow;             // 表示中フラグ
	float comboShowTimer;       // 表示残り時間
	int numberImage[10];        // 0〜9のデジタル数字テクスチャハンドル配列
};
