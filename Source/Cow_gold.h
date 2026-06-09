#pragma once
#include "CowMove.h"

// 獲得時に高スコアおよびフィーバー状態を誘発する特別な金の牛（タグ: Cow_gold）を制御するクラス
class Cow_gold : public CowMove
{
public:
	// フィーバー中に出現したかどうかのフラグ定義
	enum Tag_fever
	{
		fever,      // フィーバー状態中にスポーンした金の牛
		Nofever,    // 通常状態中にスポーンした金の牛
	};

public:
	/*
	 * @brief 金の牛オブジェクトを初期座標に配置し、スコア・衝突判定半径・フィーバー状態を設定する
	 * [入力] filename: モデルのファイルパス, initPos: 初期スポーン座標, fever: スポーン時のフィーバー状態
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_gold(std::string filename, VECTOR initPos, Tag_fever fever);
	virtual ~Cow_gold();

	void Update() override;
	void MoveCow() override;
	void CowDied();
	void KilledByBait() override;

private:
	Tag_fever mnFever;  // スポーン時のフィーバー状態
	int DeathCount;     // 生存フレームカウンター
	int DeathTimer;     // 自動消滅までの制限時間フレーム数
};
