#include "Cow_2.h"
#include "CapsuleCollider.h"

Cow_2::Cow_2(std::string filename, VECTOR initPos)
	: CowMove(filename, initPos)
{
	mfScore = 20;
	mfXp = 10;
	collider_radius_ = 50.0f;
	SetTag_cow(CowMove::kCow2);
}

Cow_2::~Cow_2()
{
}