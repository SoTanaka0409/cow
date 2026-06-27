#pragma once

#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "AnimalMove.h"

// ステージ上の一般アニマル（牛以外）の動的生成、更新、タグ判定、解放などを一括管理するクラス
class AnimalManager
{
public:
	AnimalManager();
	~AnimalManager();

	/*
	 * @brief 指定された種類の動物をランダムな位置に複数生成する
	 * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 動物の種類識別タグ, count: 生成個数
	 * [出力] なし
	 * [副作用] 動的メモリ確保された動物オブジェクトが管理リストに追加される、またはプールから復帰する
	 */
	void SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count);

	/*
	 * @brief 管理している全ての動物の更新および不要な動物の解放処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各動物のUpdate実行と、削除フラグの立った動物のプール返却
	 */
	void Update();

	/*
	 * @brief 管理している動物の描画処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief 削除フラグ(deleteFlag)が立っている動物オブジェクトを管理リストから除外しプールへ返す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] リストからのポインタ除外、プールへの追加
	 */
	void EraseAnimal();

private:
	std::vector<AnimalMove*> animals;  // 生成された動物オブジェクトのポインタ配列
	std::map<AnimalMove::Tag_animal, std::vector<AnimalMove*>> pools; // オブジェクトプール
};
