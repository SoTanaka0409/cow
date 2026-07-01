#pragma once

#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "AnimalMove.h"
#include "CreatureManager.h"

// ステージ上の一般アニマル（牛以外）の動的生成、更新、タグ判定、解放などを一括管理するクラス
class AnimalManager : public CreatureManager<AnimalMove, AnimalMove::Tag_animal>
{
public:
	AnimalManager();

	/*
	 * @brief 指定された種類の動物をランダムな位置に複数生成する
	 * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 動物の種類識別タグ, count: 生成個数
	 * [出力] なし
	 * [副作用] 動的メモリ確保された動物オブジェクトが管理リストに追加される、またはプールから復帰する
	 */
	void SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count);

protected:
	AnimalMove::Tag_animal GetTag(AnimalMove* creature) override { return creature->GetTag_animal(); }
};
