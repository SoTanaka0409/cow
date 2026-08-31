#pragma once

#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "AnimalMove.h"
#include "CreatureManager.h"

/// @brief 仕様制約: 熊以外の一般環境生物群のライフサイクルを一括管理する
/// @details メモリ断片化と頻繁なnew/deleteによる負荷を防ぐため、基底クラスのオブジェクトプール機能で運用する
class AnimalManager : public CreatureManager<AnimalMove, AnimalMove::TagAnimal>
{
public:
	AnimalManager();

	/// @brief filename
	/// @details pos
	/// @details scale
	/// @details tag
	/// @details count | 出力: なし
	/// @details 新規メモリ確保またはプールからの復帰が発生する。フレーム落ち回避のため大量生成(count)に注意
	void SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::TagAnimal tag, int count, float scatterRadius = 200.0f);

protected:
	/// @brief 仕様制約: 基底クラス側で対象インスタンスの種別（タグ）を型安全に特定・分類させるためオーバーライド必須
	AnimalMove::TagAnimal GetTag(AnimalMove* creature) override { return creature->GetTagAnimal(); }
};
