#pragma once
#include <string>

// ?Q?[?????p?????[?^??????????\???????Q (Data-Driven Design)
namespace GameConstants
{
	struct CharacterParams
	{
		float speed;
		float score;
		float xp;
		float deathTimeHeight;
		std::string modelPath;
	};

	// ?????
	const CharacterParams COW_DEFAULT  = { 10.0f,  30.0f,  10.0f, 1900.0f, "Resource/3D/Cow/uploads_files_3880923_Cow.mv1" };
	const CharacterParams COW_2        = { 10.0f,  20.0f,  10.0f, 1900.0f, "Resource/3D/Cow/uploads_files_3880923_Cow.mv1" };
	const CharacterParams COW_GOLD     = { 15.0f,  30.0f,  20.0f, 1900.0f, "Resource/3D/GOLDCow/GoldCow.mv1" };
	const CharacterParams COW_TUTORIAL = { 10.0f,  30.0f, 150.0f,  700.0f, "Resource/3D/Cow/uploads_files_3880923_Cow.mv1" };

	// ?????????????
	const CharacterParams ANIMAL_SHEEP   = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/uploads_files_3880923_Sheep.mv1" };
	const CharacterParams ANIMAL_CHICKEN = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/uploads_files_3881253_Chicken_Low.mv1" };
	const CharacterParams ANIMAL_BEAR    = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/uploads_files_3887296_Black_Bear.mv1" };
}
