#pragma once
#include "CowMove.h"

class GoldCow : public CowMove
{
public:
	/// @brief オブジェクトの振る舞いを切り替えるための状態定義
	enum TagFever
	{
		kFever,
		kNoFever,
	};

public:
	/// @brief ゴールドカウの初期化を行う
	/// @param filename モデルのファイルパス
	/// @param initPos 初期位置
	/// @param fever フィーバー由来のスポーンか
	GoldCow(std::string filename, VECTOR initPos, TagFever fever);

	/// @brief デストラクタ
	virtual ~GoldCow();

	/// @brief 状態をリセットし、再配置する
	/// @param pos 再配置する位置
	/// @details 死亡タイマーを初期化する
	virtual void Reset(VECTOR pos) override;

	/// @brief 毎フレームの更新処理
	/// @details 寿命に達した際に自身を破棄する
	void Update() override;

	/// @brief 死亡時の処理を行う
	/// @param reason 死亡の理由
	/// @details 条件を満たす場合、フィーバー状態をトリガーする
	void Die(DeathReason reason) override;

	/// @brief プール再利用時にフィーバー状態を上書き設定する
	/// @param fever 新しいフィーバー状態
	/// @details fever_を更新する
	void SetFever(TagFever fever) { fever_ = fever; }

private:
	TagFever fever_;  ///< 内部状態を管理する値
	int death_count_; ///< 時間経過や処理間隔を管理するカウンター
	int death_timer_; ///< 時間経過や処理間隔を管理するカウンター
};
