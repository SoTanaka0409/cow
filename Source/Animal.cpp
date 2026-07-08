#include "Animal.h"

Animal::Animal(std::string filename, VECTOR initPos)
	: AnimalMove(filename, initPos)
{
	// Animal_1繧偵ョ繝輔か繝ｫ繝医→縺励∝崋譛峨・邨碁ｨ灘､繧定ｨｭ螳・
	mfXp = 10;
	SetTag_animal(AnimalMove::kAnimal1);
}

Animal::~Animal()
{
}

void Animal::Update()
{
	MoveCharacter();

	// 蜷ｸ縺・ｾｼ縺ｾ繧御ｸｭ縺ｯ荳崎・辟ｶ縺ｪ蝗櫁ｻ｢繧帝亟縺舌◆繧∝屓霆｢蜃ｦ逅・ｒ繧ｹ繧ｭ繝・・縺吶ｋ
	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCharacter();
	}

	ColliderMove();
	CharacterDied();
	model_->Update();
}