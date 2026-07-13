#pragma once

#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "AnimalMove.h"
#include "CreatureManager.h"

// 仕様制約: 熊以外の一般環境生物群のライフサイクルを一括管理する
// メモリ断片化と頻繁なnew/deleteによる負荷を防ぐため、基底クラスのオブジェクトプール機能で運用する
class AnimalManager : public CreatureManager<AnimalMove, AnimalMove::TagAnimal>
{
public:
	AnimalManager();

	// 入力: filename, pos, scale, tag, count | 出力: なし
	// 副作用: 新規メモリ確保またはプールからの復帰が発生する。フレーム落ち回避のため大量生成(count)に注意
	void SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::TagAnimal tag, int count);

protected:
	// 仕様制約: 基底クラス側で対象インスタンスの種別（タグ）を型安全に特定・分類させるためオーバーライド必須
	AnimalMove::TagAnimal GetTag(AnimalMove* creature) override { return creature->GetTagAnimal(); }
};