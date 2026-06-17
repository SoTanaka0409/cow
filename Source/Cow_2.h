#pragma once
#include "CowMove.h"

// タグCow_2用クラス（スコア等のパラメータが標準と異なるための拡張）
class Cow_2 : public CowMove
{
public:
	/*
	 * @brief 初期化
	 * [入力] filename: モデルパス, initPos: 初期配置座標
	 * [出力] なし
	 * [副作用] 個別のスコア・コライダー半径の設定
	 */
	Cow_2(std::string filename, VECTOR initPos);
	virtual ~Cow_2();
};
