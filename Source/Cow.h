#pragma once
#include "CowMove.h"

// タグCow_1の標準キャラクター定義（基本行動はCowMoveに準拠）
class Cow : public CowMove
{
public:
	/*
	 * @brief オブジェクト初期化
	 * [入力] filename: モデルパス, initPos: 初期配置座標, num: 基礎スコア値
	 * [出力] なし
	 * [副作用] スコアと経験値の設定
	 */
	Cow(std::string filename, VECTOR initPos, float num);
	virtual ~Cow();
};
