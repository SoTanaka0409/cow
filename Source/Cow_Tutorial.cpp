#include "Cow_Tutorial.h"

/*
 * チュートリアル用の固有パラメータを設定するため
 * [入力] filename: モデルファイルパス, initPos: 初期座標
 * [出力] なし
 * [副作用] スコアと経験値が設定される
 */
Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	score_ = 30;
	// 短時間でレベルアップを体験させるため経験値を高く設定
	xp_ = 150;
	SetTagCow(CowMove::kCowT);
}

/*
 * リソース解放によりメモリリークを防ぐため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Cow_Tutorial::~Cow_Tutorial()
{
}