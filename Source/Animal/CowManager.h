#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"
#include "CreatureManager.h"
// 牛固有の生成ルールやゲーム進行に応じた状態を管理するため。
class CowManager : public CreatureManager<CowMove, CowMove::TagCow>
{
public:
	CowManager();
	/*
	 * 状態（フィーバー等）に応じた牛の出現制御を行うため。
	 * [入力] filename:モデルパス, pos:基準座標, scale:拡大率, tag:牛種類, count:生成個数, is_fever:フィーバー中か
	 * [出力] なし
	 * [副作用] 管理リストへ牛を追加、またはプールから復帰
	 */
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever = false, float scatterRadius = 200.0f);
protected:
	CowMove::TagCow GetTag(CowMove* creature) override { return creature->GetTagCow(); }
};
