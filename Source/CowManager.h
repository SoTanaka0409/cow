#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"
#include "CreatureManager.h"

// ステージ上の牛たちの動的生成、更新、タグ判定、上限管理、解放などを一括管理するクラス
class CowManager : public CreatureManager<CowMove, CowMove::Tag_cow>
{
public:
	CowManager();

	/*
	 * @brief 指定された種類の牛をランダムな位置に複数生成する（最大30匹制限あり）
	 * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 牛の種類識別タグ, count: 生成個数, mfever: フィーバー中かどうか
	 * [出力] なし
	 * [副作用] 動的メモリ確保された牛オブジェクトが管理リストに追加されるか、プールから復帰する
	 */
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever = false);

protected:
	CowMove::Tag_cow GetTag(CowMove* creature) override { return creature->GetTag_cow(); }
};
