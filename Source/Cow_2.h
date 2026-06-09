#pragma once
#include "CowMove.h"

// 少し価値の高い中型の牛キャラクター（タグ: Cow_2）を制御するクラス
class Cow_2 : public CowMove
{
public:
	/*
	 * @brief 中型の牛オブジェクトを初期座標に配置し、スコア・コライダー半径などを設定する
	 * [入力] filename: モデルのファイルパス, initPos: 初期スポーン座標
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_2(std::string filename, VECTOR initPos);
	virtual ~Cow_2();

	void Update() override;
};
