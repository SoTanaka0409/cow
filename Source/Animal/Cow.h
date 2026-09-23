#pragma once
#include "CowMove.h"

/// @brief Cow1型の標準的な敵キャラクターとして振る舞いを定義するため
class Cow : public CowMove
{
public:
	/// @brief 標準の牛キャラクターとしてスコアと経験値を設定するため
	/// @param filename モデルパス
	/// @param init_pos 初期配置座標
	/// @param num 基礎スコア値
	/// @details 基礎スコアと経験値を初期化
	Cow(const std::string& filename, VECTOR init_pos, float num);

	/// @brief 派生クラスでの安全なリソース解放を保証するため
	virtual ~Cow();
};
