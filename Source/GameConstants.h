#pragma once
#include <string>

// ゲームパラメータを定数として定義するデータ駆動設計の参照先 (Data-Driven Design)
namespace GameConstants
{
	namespace ImagePaths
	{
		const std::string kCombo = "Resource/2D/ComboText.png";
		const std::string kLimit = "Resource/2D/TimeLimitText.png";
		const std::string kBeam = "Resource/2D/PlayerCaptureBeam.png";
		const std::string kCowFrame = "Resource/2D/CowGaugeFrame.png";
		const std::string kResult = "Resource/2D/ResultBackground.png";
		const std::string kRank1 = "Resource/2D/RankingFirst.png";
		const std::string kRank2 = "Resource/2D/RankingSecond.png";
		const std::string kRank3 = "Resource/2D/RankingThird.png";
		const std::string kRankingTitle = "Resource/2D/RankingTitle.png";
		const std::string kScoreTitle = "Resource/2D/ScoreTitle.png";
		const std::string kPoint = "Resource/2D/PointText.png";
		const std::string kBtnStart = "Resource/2D/ButtonStart.png";
		const std::string kBtnExit = "Resource/2D/ButtonExit.png";
		const std::string kBtnTutorial = "Resource/2D/ButtonTutorial.png";
		const std::string kBtnSetting = "Resource/2D/ButtonSettings.png";
		const std::string kSettingsBg = "Resource/2D/SettingsBackground.png";
		const std::string kFontBack = "Resource/2D/FontPanelBackground.png";
		const std::string kScoreWord = "Resource/2D/ScoreHudText.png";
		const std::string kMinus = "Resource/2D/MinusText.png";
		const std::string kTitleBg = "Resource/2D/TitleBackground.png";
		const std::string kTitleUfo = "Resource/2D/TitleCowUfo.png";
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
	const CharacterParams kCowDefault  = { 10.0f,  30.0f,  10.0f, 1900.0f, "Resource/3D/Cow/NormalCow.mv1" };
	const CharacterParams kCow2        = { 10.0f,  20.0f,  10.0f, 1900.0f, "Resource/3D/Cow/NormalCow.mv1" };
	const CharacterParams kCowGold     = { 15.0f,  30.0f,  20.0f, 1900.0f, "Resource/3D/GoldCow/GoldCow.mv1" };
	const CharacterParams kCowTutorial = { 10.0f,  30.0f, 150.0f,  700.0f, "Resource/3D/Cow/NormalCow.mv1" };

	// 動物（敵キャラクター）
	const CharacterParams kAnimalSheep   = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/Sheep.mv1" };
	const CharacterParams kAnimalChicken = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/Chicken.mv1" };
	const CharacterParams kAnimalBear    = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Animal/BlackBear.mv1" };
}

