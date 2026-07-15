#pragma once
#include <string>
#include <vector>

// CSVファイルからステージオブジェクトを一括生成するローダー
// Scene3D::Initialize() の直書きオブジェクト配置をデータドリブンに置き換えるために使用する
class StageLoader
{
public:
	// csvPath で指定した CSV ファイルを読み込み、行ごとにオブジェクトを生成する
	// [入力] csvPath: Shift-JIS エンコードの CSV ファイルパス
	// [出力] なし
	// [副作用] ObjectManager に各オブジェクトが登録される
	static void LoadFromCSV(const std::string& csvPath);

private:
	// type=object_stage / type=rock 行を処理してObject_Stageを生成する
	static void SpawnObjectStage(const std::vector<std::string>& cols);

	// type=mountain 行を処理してMountainを生成する
	static void SpawnMountain(const std::vector<std::string>& cols);

	// type=wall 行を処理してWallを生成する
	static void SpawnWall(const std::vector<std::string>& cols);

	// CSV の1行をカンマで分割して返す
	static std::vector<std::string> SplitCSV(const std::string& line);

	// 文字列をfloatに変換する。変換失敗時は 0.0f を返す
	static float ToFloat(const std::string& s);
};