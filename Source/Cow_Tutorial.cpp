#include "Cow_Tutorial.h"

Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 30;
	// 短時間でレベルアチE�Eを体験させるため経験値を高く設宁E
	mfXp = 150;
	SetTagCow(CowMove::kCowT);
}

Cow_Tutorial::~Cow_Tutorial()
{
	// 親クラスCowMoveでリソース解放されるため、E
	// 多重解放によるクラチE��ュを防ぐべくここでは手動deleteしなぁE
}