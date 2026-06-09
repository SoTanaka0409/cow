#include "Cow_Tutorial.h"

Cow_Tutorial::Cow_Tutorial(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 30;
	mfXp = 150;
	SetTag_cow(CowMove::Cow_T);
}

Cow_Tutorial::~Cow_Tutorial()
{
	// 親クラスである CowMove のデストラクタで mpModel は安全に破棄されるため、
	// 多重解放（クラッシュ原因）を防ぐためにここでの手動 delete は行いません。
}

void Cow_Tutorial::Update()
{
	MoveCow();

	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCow();
	}

	if (mpCowVm != nullptr)
	{
		mpCowVm->Update();
	}

	ColliderMove();
	CowDied();
	mpModel->Update();
}
