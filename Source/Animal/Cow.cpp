#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"
/// @brief プレイヤーの対象となる標準キャラクターを初期化する。
/// @param filename モデルパス
/// @param init_pos 初期位置
/// @param num 基礎スコア値
/// @details スコアと経験値が設定される
Cow::Cow(const std::string& filename, VECTOR init_pos, float num)
	: CowMove(filename, init_pos)
{
	score_ = num;
	xp_ = 10;
	SetTagCow(CowMove::kCow1);
	ai_state_ = kStateIdle;
}
/// @brief リソース解放。
Cow::~Cow()
{
}
