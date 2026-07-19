#pragma once
#include "CowMove.h"

// 標準の牛とはスコアやパラメータが異なるため拡張
class Cow_2 : public CowMove
{
public:
	/*
	 * 特殊なパラメータを持つ牛を生成するため
	 * [入力] filename: モデルファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] スコア、経験値、コライダー半径が個別に設定される
	 */
	Cow_2(std::string filename, VECTOR initPos);

	/*
	 * 終了処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Cow_2();
};
