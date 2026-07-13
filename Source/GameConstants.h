#pragma once
#include <string>

// ゲームパラメータを定数として定義するデータ駆動設計の参照先 (Data-Driven Design)
namespace GameConstants
{
	namespace ImagePaths
	{
		const std::string kCombo = "Resource/2D/COMBO.png";
		const std::string kLimit = "Resource/2D/rimit.png";
		const std::string kBeam = "Resource/2D/green_beam_transparent.png";
		const std::string kCowFrame = "Resource/2D/cow_frame.png";
		const std::string kResult = "Resource/2D/リザルト.png";
		const std::string kRank1 = "Resource/2D/1位.png";
		const std::string kRank2 = "Resource/2D/2位.png";
		const std::string kRank3 = "Resource/2D/3位.png";
		const std::string kRankingTitle = "Resource/2D/ランキング.png";
		const std::string kScoreTitle = "Resource/2D/スコア.png";
		const std::string kPoint = "Resource/2D/点.png";
		const std::string kBtnStart = "Resource/2D/スタートボタン1.png";
		const std::string kBtnExit = "Resource/2D/しゅうりょうぼたん (1).png";
		const std::string kBtnTutorial = "Resource/2D/チュートリアルぼたん.png";
		const std::string kBtnSetting = "Resource/2D/せっていぼたん1.png";
		const std::string kSettingsBg = "Resource/2D/settings_bg.png";
		const std::string kFontBack = "Resource/2D/fontback.png";
		const std::string kScoreWord = "Resource/2D/SCORE.png";
		const std::string kMinus = "Resource/2D/マイナス.png";
		const std::string kTitleBg = "Resource/2D/タイトルシーン.png";
		const std::string kTitleUfo = "Resource/2D/牛とUFO.png";
	}
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
