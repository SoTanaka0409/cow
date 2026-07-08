#pragma once
#include "CowMove.h"

// 獲得時に高スコア加算とフィーバー状態移行を行う特殊キャラクター制御クラス
class Cow_gold : public CowMove
{
public:
	// スポーン時のゲーム状態。フィーバー連鎖を制御するために使用
	enum TagFever
	{
		kFever,      // 連鎖終了判定用（フィーバー中スポーン）
		kNoFever,    // フィーバー開始判定用（通常時スポーン）
	};

public:
	/*
	 * @brief 固有パラメータ（スコア、判定半径等）で初期化する
	 * [入力] filename: モデルファイルパス, initPos: 初期座標, fever: スポーン時状態
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_gold(std::string filename, VECTOR initPos, TagFever fever);

	/*
	 * @brief デストラクタ
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Cow_gold();

	/*
	 * @brief オブジェクト再利用時に生存時間を初期化する
	 * [入力] pos: 再配置座標
	 * [出力] なし
	 * [副作用] 生存タイマーが0にリセットされる
	 */
	virtual void Reset(VECTOR pos) override;

	/*
	 * @brief 生存時間の更新とフィーバー終了時の自動消滅判定を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 制限時間超過などでオブジェクトが消滅(Die)する
	 */
	void Update() override;

	/*
	 * @brief プレイヤーによる捕獲時にフィーバー状態を開始させる
	 * [入力] reason: 消滅理由
	 * [出力] なし
	 * [副作用] 全体状態がフィーバー状態へ移行する
	 */
	void Die(DeathReason reason) override;

	/*
	 * @brief フィーバー状態の動的変更（プールからの再利用時などに使用）
	 * [入力] fever: 新しいフィーバー状態
	 * [出力] なし
	 * [副作用] なし
	 */
	void SetFever(TagFever fever) { fever_ = fever; }

private:
	TagFever fever_;  // フィーバー連鎖制御用の状態フラグ
	int death_count_;     // 画面内滞留時間の計測用
	int death_timer_;     // 画面残りによるメモリ圧迫や進行妨害を防ぐための寿命
};