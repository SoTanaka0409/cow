#include "Animal.h"

Animal::Animal(std::string filename, VECTOR initPos)
	: AnimalMove(filename, initPos)
{
	mfXp = 10;
	SetTag_animal(AnimalMove::Animal_1);
}

Animal::~Animal()
{
}

void Animal::Update()
{
	MoveCharacter();

	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCharacter();
	}

	ColliderMove();
	CharacterDied();
	mpModel->Update();
}


