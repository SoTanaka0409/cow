#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"

Cow::Cow(std::string filename, VECTOR initPos, float num)
	: CowMove(filename, initPos)
{
	mfScore = num;
	mfXp = 10;
	SetTag_cow(CowMove::Cow_1);
	mCurrentState = STATE_IDLE;
}

Cow::~Cow()
{
}

void Cow::Update()
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
