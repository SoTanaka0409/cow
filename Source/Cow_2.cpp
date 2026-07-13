#include "Cow_2.h"
#include "CapsuleCollider.h"

/*
 * 特殊なパラメータを持つ牛を生成するため
 * [入力] filename: モデルファイルパス, initPos: 初期座標
 * [出力] なし
 * [副作用] スコア、経験値、コライダー半径が個別に設定される
 */
Cow_2::Cow_2(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 20;
	mfXp = 10;
	collider_radius_ = 50.0f;
	SetTagCow(CowMove::kCow2);
}

/*
 * 終了処理
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Cow_2::~Cow_2()
{
}
