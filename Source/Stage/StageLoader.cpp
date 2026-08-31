#include "StageLoader.h"
#include "Object_Stage.h"
#include "Mountain.h"
#include "Wall.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/// @brief CSVファイルを読み込んでステージオブジェクトを生成するため
/// @param csvPath Shift-JISエンコードのCSVファイルパス
/// @details ObjectManagerに各オブジェクトが登録される
void StageLoader::LoadFromCSV(const std::string& csvPath)
{
	std::ifstream file(csvPath);
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	bool isFirstLine = true;

	while (std::getline(file, line))
	{
		// ヘッダー行をスキップ
		if (isFirstLine)
		{
			isFirstLine = false;
			continue;
		}

		// 空行・コメント行をスキップ
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		// Windows 改行コード(\r\n)対応
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		auto cols = SplitCSV(line);
		if (cols.empty()) continue;

		const std::string& type = cols[0];

		if (type == "object_stage" || type == "rock")
		{
			SpawnObjectStage(cols);
		}
		else if (type == "mountain")
		{
			SpawnMountain(cols);
		}
		else if (type == "wall")
		{
			SpawnWall(cols);
		}
	}
}

/// @brief Object_Stageを1個生成するため
/// @param cols CSVの分割済み列リスト
/// @details cols[1]=model_path, [2-4]=position(x,y,z), [5]=scale(scalar),
/// @details =rotation(rx,ry,rz), [11-14]=color(r,g,b,a) ※省略可
/// @details ObjectManagerにObject_Stageが登録される
void StageLoader::SpawnObjectStage(const std::vector<std::string>& cols)
{
	if (cols.size() < 11) return;

	const std::string& modelPath = cols[1];
	VECTOR pos    = VGet(ToFloat(cols[2]), ToFloat(cols[3]), ToFloat(cols[4]));
	float  scale  = ToFloat(cols[5]);
	VECTOR rot    = VGet(ToFloat(cols[8]), ToFloat(cols[9]), ToFloat(cols[10]));

	auto obj = new Object_Stage(modelPath, pos, scale, rot);

	// カラー列が存在し、空でなければSetColorを呼ぶ
	if (cols.size() >= 15 && !cols[11].empty())
	{
		float cr = ToFloat(cols[11]);
		float cg = ToFloat(cols[12]);
		float cb = ToFloat(cols[13]);
		float ca = ToFloat(cols[14]);
		obj->SetColor(cr, cg, cb, ca);
	}
}

/// @brief Mountainを1個生成するため
/// @param cols CSVの分割済み列リスト
/// @details cols[1]=model_path, [2-4]=position(x,y,z), [5-7]=scale(sx,sy,sz),
/// @details =rotation(rx,ry,rz), [11-14]=color(r,g,b,a) ※省略可
/// @details ObjectManagerにMountainが登録される
void StageLoader::SpawnMountain(const std::vector<std::string>& cols)
{
	if (cols.size() < 11) return;

	const std::string& modelPath = cols[1];
	VECTOR pos   = VGet(ToFloat(cols[2]), ToFloat(cols[3]), ToFloat(cols[4]));
	VECTOR scale = VGet(ToFloat(cols[5]), ToFloat(cols[6]), ToFloat(cols[7]));
	VECTOR rot   = VGet(ToFloat(cols[8]), ToFloat(cols[9]), ToFloat(cols[10]));

	auto mountain = new Mountain(modelPath, pos, scale, rot);

	// カラー列が存在し、空でなければSetColorを呼ぶ
	if (cols.size() >= 15 && !cols[11].empty())
	{
		float cr = ToFloat(cols[11]);
		float cg = ToFloat(cols[12]);
		float cb = ToFloat(cols[13]);
		float ca = ToFloat(cols[14]);
		mountain->SetColor(cr, cg, cb, ca);
	}
}

/// @brief Wallを1枚生成するため
/// @param cols CSVの分割済み列リスト
/// @details cols[1]=model_path(空可), [2-4]=centerPos(x,y,z),
/// @details =topLeft(x,y,z), [8-10]=bottomRight(x,y,z)
/// @details ObjectManagerにWallが登録される
void StageLoader::SpawnWall(const std::vector<std::string>& cols)
{
	if (cols.size() < 11) return;

	const std::string& modelPath = cols[1];
	VECTOR center      = VGet(ToFloat(cols[2]), ToFloat(cols[3]), ToFloat(cols[4]));
	VECTOR topLeft     = VGet(ToFloat(cols[5]), ToFloat(cols[6]), ToFloat(cols[7]));
	VECTOR bottomRight = VGet(ToFloat(cols[8]), ToFloat(cols[9]), ToFloat(cols[10]));

	new Wall(modelPath, center, topLeft, bottomRight);
}

/// @brief CSV の1行をカンマで分割するため
/// @param line 処理対象の文字列
/// @return 分割結果の文字列ベクター
std::vector<std::string> StageLoader::SplitCSV(const std::string& line)
{
	std::vector<std::string> result;
	std::stringstream ss(line);
	std::string token;

	while (std::getline(ss, token, ','))
	{
		result.push_back(token);
	}

	return result;
}

/// @brief 文字列をfloatに変換するため
/// @param s 変換対象の文字列
/// @return 変換結果のfloat値。変換失敗時は 0.0f
float StageLoader::ToFloat(const std::string& s)
{
	if (s.empty()) return 0.0f;
	try
	{
		return std::stof(s);
	}
	catch (...)
	{
		return 0.0f;
	}
}
