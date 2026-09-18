#pragma once
#include <string>

/// @brief ゲームパラメータを定数として定義するデータ駆動設計の参照先 (Data-Driven Design)
namespace GameConstants
{
	namespace ImagePaths
	{
		const std::string kCombo = "Resource/2D/Combo/ComboText.png";
		const std::string kLimit = "Resource/2D/InGame/Text_089b.png";
		const std::string kBeam = "Resource/2D/Player/VacuumBeam.png";
		const std::string kCowFrame = "Resource/2D/Player/VacuumGaugeFrame.png";
		const std::string kResult = "Resource/2D/Result/ResultBackground.png";
		const std::string kRank1 = "Resource/2D/Ranking/Ranking1st.png";
		const std::string kRank2 = "Resource/2D/Ranking/Ranking2nd.png";
		const std::string kRank3 = "Resource/2D/Ranking/Ranking3rd.png";
		const std::string kRankingTitle = "Resource/2D/Ranking/RankingTitle.png";
		const std::string kScoreTitle = "Resource/2D/Score/ScoreTitle.png";
		const std::string kPoint = "Resource/2D/Score/PointText.png";
		const std::string kBtnStart = "Resource/2D/Button/Button__64e1.png";
		const std::string kBtnExit = "Resource/2D/Button/Button_Exit.png";
		const std::string kBtnTutorial = "Resource/2D/Button/Button_Tutorial.png";
		const std::string kBtnSetting = "Resource/2D/Button/Button_Settings.png";
		const std::string kSettingsBg = "Resource/2D/Settings/SettingsBackground.png";
		const std::string kFontBack = "Resource/2D/InGame/TextBackground_c378.png";
		const std::string kScoreWord = "Resource/2D/Score/ScoreText.png";
		const std::string kMinus = "Resource/2D/Score/MinusText.png";
		const std::string kTitleBg = "Resource/2D/Title/TitleBackground.png";
		const std::string kTitleUfo = "Resource/2D/Title/TitleCowUFO.png";
	};
	
	struct CharacterParams
	{
		float speed;             ///< 移動や回転の計算に使用する値
		float score;             ///< スコアや成長値の管理に使用する値
		float xp;                ///< スコアや成長値の管理に使用する値
		float death_time_height; ///< 時間経過や処理間隔を管理するカウンター
		std::string model_path;  ///< 3Dモデルの管理に使用する情報
	};

	/// @brief 牛
	const CharacterParams kCowDefault  = { 10.0f,  30.0f,  10.0f, 1900.0f, "Resource/3D/Character/Cow/NormalCow_5f02.mv1" }; ///< ゲーム内で使用する固定値
	const CharacterParams kCow2        = { 10.0f,  20.0f,  10.0f, 1900.0f, "Resource/3D/Character/Cow/NormalCow_5f02.mv1" }; ///< ゲーム内で使用する固定値
	const CharacterParams kCowGold     = { 15.0f,  30.0f,  20.0f, 1900.0f, "Resource/3D/Character/Cow/Cow_f6cf.mv1" };  ///< ゲーム内で使用する固定値
	const CharacterParams kCowTutorial = { 10.0f,  30.0f, 150.0f,  700.0f, "Resource/3D/Character/Cow/NormalCow_5f02.mv1" }; ///< ゲーム内で使用する固定値

	/// @brief 動物（敵キャラクター）
	const CharacterParams kAnimalSheep   = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Character/Animal/Sheep.mv1" };    ///< ゲーム内で使用する固定値
	const CharacterParams kAnimalChicken = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Character/Animal/Chicken.mv1" }; ///< ゲーム内で使用する固定値
	const CharacterParams kAnimalBear    = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/Character/Animal/Bear.mv1" };    ///< ゲーム内で使用する固定値
}

