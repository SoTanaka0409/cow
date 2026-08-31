#include "Cow_2.h"
#include "CapsuleCollider.h"

/// @brief 特殊なパラメータを持つ牛を生成するため
/// @param filename モデルファイルパス
/// @param initPos 初期座標
/// @details スコア、経験値、コライダー半径が個別に設定される
Cow_2::Cow_2(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	score_ = 20;
	xp_ = 10;
	collider_radius_ = 50.0f;
	SetTagCow(CowMove::kCow2);
}

/// @brief 終了処理
Cow_2::~Cow_2()
{
}
