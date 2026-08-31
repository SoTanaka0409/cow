#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"
/// @brief プレイヤーの対象となる標準キャラクターを初期化する。
/// @param filename モデルパス
/// @param initPos 初期位置
/// @param num 基礎スコア値
/// @details スコアと経験値が設定される
Cow::Cow(std::string filename, VECTOR initPos, float num)
	: CowMove(filename, initPos)
{
	score_ = num;
	xp_ = 10;
	SetTagCow(CowMove::kCow1);
	mCurrentState = STATE_IDLE;
}
/// @brief リソース解放。
Cow::~Cow()
{
}
