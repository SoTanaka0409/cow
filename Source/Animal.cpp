#include "Animal.h"

Animal::Animal(std::string filename, VECTOR initPos)
	: AnimalMove(filename, initPos)
{
	// Animal_1をデフォルトとし、固有の経験値を設定
	mfXp = 10;
	SetTag_animal(AnimalMove::Animal_1);
}

Animal::~Animal()
{
}

void Animal::Update()
{
	MoveCharacter();

	// 吸い込まれ中は不自然な回転を防ぐため回転処理をスキップする
	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCharacter();
	}

	ColliderMove();
	CharacterDied();
	mpModel->Update();
}
