#pragma once
#include <string>
#include <vector>

// CSVファイルからステージオブジェクトをまとめて生成するローダー
// GameScene::Initialize() の直書き配置をデータ駆動に置き換えるために使用する
class StageLoader
{
public:
	// 指定されたCSVを読み込み、行ごとにオブジェクトを生成する
	static void LoadFromCSV(const std::string& csvPath);

private:
	// type=object_stage / type=rock の行を処理する
	static void SpawnObjectStage(const std::vector<std::string>& cols);

	// type=mountain の行を処理する
	static void SpawnMountain(const std::vector<std::string>& cols);

	// type=wall の行を処理する
	static void SpawnWall(const std::vector<std::string>& cols);

	// CSVの1行をカンマで分割する
	static std::vector<std::string> SplitCSV(const std::string& line);

	// 文字列をfloatに変換する。失敗時は0.0fを返す
	static float ToFloat(const std::string& s);
};
