#pragma once
#include <string>

/// @brief ゲームパラメータを定数として定義するデータ駆動設計の参照先 (Data-Driven Design)
namespace GameConstants
{
	namespace ImagePaths
	{
		const std::string kCombo = "Resource/2D/コンボ/コンボ文字.png";
		const std::string kLimit = "Resource/2D/ゲーム画面/制限時間文字.png";
		const std::string kBeam = "Resource/2D/プレイヤー/吸い込みビーム.png";
		const std::string kCowFrame = "Resource/2D/プレイヤー/吸い込みゲージ枠.png";
		const std::string kResult = "Resource/2D/リザルト/リザルト背景.png";
		const std::string kRank1 = "Resource/2D/ランキング/ランキング１位.png";
		const std::string kRank2 = "Resource/2D/ランキング/ランキング２位.png";
		const std::string kRank3 = "Resource/2D/ランキング/ランキング３位.png";
		const std::string kRankingTitle = "Resource/2D/ランキング/ランキングタイトル.png";
		const std::string kScoreTitle = "Resource/2D/スコア/スコアタイトル.png";
		const std::string kPoint = "Resource/2D/スコア/ポイント文字.png";
		const std::string kBtnStart = "Resource/2D/ボタン/ボタン_スタート.png";
		const std::string kBtnExit = "Resource/2D/ボタン/ボタン_終了.png";
		const std::string kBtnTutorial = "Resource/2D/ボタン/ボタン_チュートリアル.png";
		const std::string kBtnSetting = "Resource/2D/ボタン/ボタン_設定.png";
		const std::string kSettingsBg = "Resource/2D/設定/設定背景.png";
		const std::string kFontBack = "Resource/2D/ゲーム画面/文字パネル背景.png";
		const std::string kScoreWord = "Resource/2D/スコア/スコア文字.png";
		const std::string kMinus = "Resource/2D/スコア/マイナス文字.png";
		const std::string kTitleBg = "Resource/2D/タイトル/タイトル背景.png";
		const std::string kTitleUfo = "Resource/2D/タイトル/タイトル牛UFO.png";
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
	const CharacterParams kCowDefault  = { 10.0f,  30.0f,  10.0f, 1900.0f, "Resource/3D/キャラクター/牛/通常の牛.mv1" }; ///< ゲーム内で使用する固定値
	const CharacterParams kCow2        = { 10.0f,  20.0f,  10.0f, 1900.0f, "Resource/3D/キャラクター/牛/通常の牛.mv1" }; ///< ゲーム内で使用する固定値
	const CharacterParams kCowGold     = { 15.0f,  30.0f,  20.0f, 1900.0f, "Resource/3D/キャラクター/牛/金の牛.mv1" };  ///< ゲーム内で使用する固定値
	const CharacterParams kCowTutorial = { 10.0f,  30.0f, 150.0f,  700.0f, "Resource/3D/キャラクター/牛/通常の牛.mv1" }; ///< ゲーム内で使用する固定値

	/// @brief 動物（敵キャラクター）
	const CharacterParams kAnimalSheep   = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/キャラクター/動物/羊.mv1" };    ///< ゲーム内で使用する固定値
	const CharacterParams kAnimalChicken = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/キャラクター/動物/ニワトリ.mv1" }; ///< ゲーム内で使用する固定値
	const CharacterParams kAnimalBear    = { 10.0f, -10.0f,   0.0f, 1000.0f, "Resource/3D/キャラクター/動物/熊.mv1" };    ///< ゲーム内で使用する固定値
}

