#include "Cow_2.h"
#include "CapsuleCollider.h"

Cow_2::Cow_2(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	score = 20;
	xp = 10;
	colliderRadius = 50.0f;
	SetTag_cow(CowMove::Cow_2);
}

Cow_2::~Cow_2()
{
}
