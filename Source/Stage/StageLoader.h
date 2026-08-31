#pragma once
#include <string>
#include <vector>

/// @brief CSVファイルからステージオブジェクトをまとめて生成するローダー
/// @details GameScene::Initialize() の直書き配置をデータ駆動に置き換えるために使用する
class StageLoader
{
public:
	/// @brief 指定されたCSVを読み込み、行ごとにオブジェクトを生成する
	static void LoadFromCSV(const std::string& csvPath);

private:
	/// @brief type=object_stage / type=rock の行を処理する
	static void SpawnObjectStage(const std::vector<std::string>& cols);

	/// @brief type=mountain の行を処理する
	static void SpawnMountain(const std::vector<std::string>& cols);

	/// @brief type=wall の行を処理する
	static void SpawnWall(const std::vector<std::string>& cols);

	/// @brief CSVの1行をカンマで分割する
	static std::vector<std::string> SplitCSV(const std::string& line);

	/// @brief 文字列をfloatに変換する。失敗時は0.0fを返す
	static float ToFloat(const std::string& s);
};
