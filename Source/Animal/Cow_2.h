#pragma once
#include "CowMove.h"

/// @brief 標準の牛とはスコアやパラメータが異なるため拡張
class Cow_2 : public CowMove
{
public:
	/// @brief 特殊なパラメータを持つ牛を生成するため
	/// @param filename モデルファイルパス
	/// @param init_pos 初期座標
	/// @details スコア、経験値、コライダー半径が個別に設定される
	Cow_2(const std::string& filename, VECTOR init_pos);

	/// @brief 終了処理
	virtual ~Cow_2();
};
