#include "Cow_2.h"
#include "CapsuleCollider.h"

Cow_2::Cow_2(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 20;
	mfXp = 10;
	mColliderRadius = 50.0f;
	SetTag_cow(CowMove::Cow_2);
}

Cow_2::~Cow_2()
{
}
