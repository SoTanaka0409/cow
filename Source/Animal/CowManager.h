#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"
#include "CreatureManager.h"
/// @brief 牛固有の生成ルールやゲーム進行に応じた状態を管理するため。
class CowManager : public CreatureManager<CowMove, CowMove::TagCow>
{
public:
	CowManager();
	/// @brief 状態（フィーバー等）に応じた牛の出現制御を行うため。
	/// @param filename モデルパス
	/// @param pos 基準座標
	/// @param scale 拡大率
	/// @param tag 牛種類
	/// @param count 生成個数
	/// @param is_fever フィーバー中か
	/// @details 管理リストへ牛を追加、またはプールから復帰
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::TagCow tag, int count, bool is_fever = false, float scatterRadius = 200.0f);
protected:
	CowMove::TagCow GetTag(CowMove* creature) override { return creature->GetTagCow(); }
};
