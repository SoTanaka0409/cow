#include "Animal.h"
#include "CapsuleCollider.h"

Animal::Animal(std::string filename, VECTOR initPos)
	: AnimalMove(filename, initPos)
{
	mfXp = 10;
	SetTag_animal(AnimalMove::Animal_1);
}

Animal::~Animal()
{
	// 親クラスである AnimalMove のデストラクタで mpModel および mpCapsuleCollider は安全に破棄されます。
}

void Animal::Update()
{
	MoveAnimal();

	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationAnimal();
	}

	ColliderMove();
	mpModel->Update();
}
