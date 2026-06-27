#include "Cow_Tutorial.h"

Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	score = 30;
	// 短時間でレベルアップを体験させるため経験値を高く設定
	xp = 150;
	SetTag_cow(CowMove::Cow_T);
}

Cow_Tutorial::~Cow_Tutorial()
{
	// 親クラスCowMoveでリソース解放されるため、
	// 多重解放によるクラッシュを防ぐべくここでは手動deleteしない
}
