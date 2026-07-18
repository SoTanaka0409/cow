#include "StageLoader.h"
#include "Object_Stage.h"
#include "Mountain.h"
#include "Wall.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/*
 * CSVファイルを読み込んでステージオブジェクトを生成するため
 * [入力] csvPath: Shift-JISエンコードのCSVファイルパス
 * [出力] なし
 * [副作用] ObjectManagerに各オブジェクトが登録される
 */
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

/*
 * Object_Stageを1個生成するため
 * [入力] cols: CSVの分割済み列リスト
 *   cols[1]=model_path, [2-4]=position(x,y,z), [5]=scale(scalar),
 *   [8-10]=rotation(rx,ry,rz), [11-14]=color(r,g,b,a) ※省略可
 * [出力] なし
 * [副作用] ObjectManagerにObject_Stageが登録される
 */
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

/*
 * Mountainを1個生成するため
 * [入力] cols: CSVの分割済み列リスト
 *   cols[1]=model_path, [2-4]=position(x,y,z), [5-7]=scale(sx,sy,sz),
 *   [8-10]=rotation(rx,ry,rz), [11-14]=color(r,g,b,a) ※省略可
 * [出力] なし
 * [副作用] ObjectManagerにMountainが登録される
 */
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

/*
 * Wallを1枚生成するため
 * [入力] cols: CSVの分割済み列リスト
 *   cols[1]=model_path(空可), [2-4]=centerPos(x,y,z),
 *   [5-7]=topLeft(x,y,z), [8-10]=bottomRight(x,y,z)
 * [出力] なし
 * [副作用] ObjectManagerにWallが登録される
 */
void StageLoader::SpawnWall(const std::vector<std::string>& cols)
{
	if (cols.size() < 11) return;

	const std::string& modelPath = cols[1];
	VECTOR center      = VGet(ToFloat(cols[2]), ToFloat(cols[3]), ToFloat(cols[4]));
	VECTOR topLeft     = VGet(ToFloat(cols[5]), ToFloat(cols[6]), ToFloat(cols[7]));
	VECTOR bottomRight = VGet(ToFloat(cols[8]), ToFloat(cols[9]), ToFloat(cols[10]));

	new Wall(modelPath, center, topLeft, bottomRight);
}

/*
 * CSV の1行をカンマで分割するため
 * [入力] line: 処理対象の文字列
 * [出力] 分割結果の文字列ベクター
 * [副作用] なし
 */
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

/*
 * 文字列をfloatに変換するため
 * [入力] s: 変換対象の文字列
 * [出力] 変換結果のfloat値。変換失敗時は 0.0f
 * [副作用] なし
 */
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
