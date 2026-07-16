#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"
/*
 * プレイヤーの対象となる標準キャラクターを初期化する。
 * [入力] filename: モデルパス, initPos: 初期位置, num: 基礎スコア値
 * [出力] なし
 * [副作用] スコアと経験値が設定される
 */
Cow::Cow(std::string filename, VECTOR initPos, float num)
	: CowMove(filename, initPos)
{
	score_ = num;
	xp_ = 10;
	SetTagCow(CowMove::kCow1);
	mCurrentState = STATE_IDLE;
}
/*
 * リソース解放。
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Cow::~Cow()
{
}
