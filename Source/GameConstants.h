#pragma once
#include <string>

// ゲームパラメータを定数として定義するデータ駆動設計の参照先 (Data-Driven Design)
namespace GameConstants
{
	struct CharacterParams
	{
		float speed;
		float score;
		float xp;
		float death_time_height;
		std::string model_path;
	};

	// 牛
	const CharacterParams kCowDefault  = { 10.0f,  30.0f,  10.0f, 1900.0f, "Resource/3D/Cow/uploads_files_3880923_Cow.mv1" };
	const CharacterParams kCow2        = { 10.0f,  20.0f,  10.0f, 1900.0f, "Resource/3D/Cow/uploads_files_3880923_Cow.mv1" };
	const CharacterParams kCowGold     = { 15.0f,  30.0f,  20.0f, 1900.0f, "Resource/3D/GOLDCow/GoldCow.mv1" };
	const CharacterParams kCowTutorial = { 10.0f,  30.0f, 150.0f,  700.0f, "Resource/3D/Cow/uploads_files_3880923_Cow.mv1" };

	// 動物（敵キャラクター）
	const CharacterParams kAnimalSheep   = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/uploads_files_3880923_Sheep.mv1" };
	const CharacterParams kAnimalChicken = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/uploads_files_3881253_Chicken_Low.mv1" };
	const CharacterParams kAnimalBear    = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/uploads_files_3887296_Black_Bear.mv1" };
}
