#include "Cow_Tutorial.h"

/// @brief チュートリアル用の固有パラメータを設定するため
/// @param filename モデルファイルパス
/// @param initPos 初期座標
/// @details スコアと経験値が設定される
Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	score_ = 30;
	// 短時間でレベルアップを体験させるため経験値を高く設定
	xp_ = 150;
	SetTagCow(CowMove::kCowT);
}

/// @brief リソース解放によりメモリリークを防ぐため
Cow_Tutorial::~Cow_Tutorial()
{
}
