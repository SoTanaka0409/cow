#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"

Cow::Cow(std::string filename, VECTOR initPos, float num)
	: CowMove(filename, initPos)
{
	mfScore = num;
	mfXp = 10;
	SetTagCow(CowMove::kCow1);
	mCurrentState = STATE_IDLE;
}

Cow::~Cow()
{
}