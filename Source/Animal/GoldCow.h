#pragma once
#include "CowMove.h"

class GoldCow : public CowMove
{
public:
	// オブジェクトの振る舞いを切り替えるための状態定義
	enum TagFever
	{
		kFever,
		kNoFever,
	};

public:
	/*
	 * ゴールドカウの初期化を行う
	 * [入力] filename: モデルのファイルパス, initPos: 初期位置, fever: フィーバー由来のスポーンか
	 * [出力] なし
	 * [副作用] なし
	 */
	GoldCow(std::string filename, VECTOR initPos, TagFever fever);

	/*
	 * デストラクタ
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~GoldCow();

	/*
	 * 状態をリセットし、再配置する
	 * [入力] pos: 再配置する位置
	 * [出力] なし
	 * [副作用] 死亡タイマーを初期化する
	 */
	virtual void Reset(VECTOR pos) override;

	/*
	 * 毎フレームの更新処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 寿命に達した際に自身を破棄する
	 */
	void Update() override;

	/*
	 * 死亡時の処理を行う
	 * [入力] reason: 死亡の理由
	 * [出力] なし
	 * [副作用] 条件を満たす場合、フィーバー状態をトリガーする
	 */
	void Die(DeathReason reason) override;

	/*
	 * プール再利用時にフィーバー状態を上書き設定する
	 * [入力] fever: 新しいフィーバー状態
	 * [出力] なし
	 * [副作用] fever_を更新する
	 */
	void SetFever(TagFever fever) { fever_ = fever; }

private:
	TagFever fever_;
	int death_count_;
	int death_timer_;
};
