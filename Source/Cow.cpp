#include "Cow.h"
#include "CapsuleCollider.h"
#include "Master.h"

Cow::Cow(std::string filename, VECTOR initPos, float num)
	: CowMove(filename, initPos)
{
	score = num;
	xp = 10;
	SetTag_cow(CowMove::Cow_1);
	currentState = STATE_IDLE;
}

Cow::~Cow()
{
}
