#pragma once
#include "CowMove.h"

/// @brief 標準の牛とはスコアやパラメータが異なるため拡張
class Cow_2 : public CowMove
{
public:
	/// @brief 特殊なパラメータを持つ牛を生成するため
	/// @param filename モデルファイルパス
	/// @param initPos 初期座標
	/// @details スコア、経験値、コライダー半径が個別に設定される
	Cow_2(std::string filename, VECTOR initPos);

	/// @brief 終了処理
	virtual ~Cow_2();
};
