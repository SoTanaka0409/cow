#pragma once

#include <vector>
#include <string>
#include "DxLib.h"
#include "AnimalMove.h"

// ステージ上の一般アニマル（牛以外）の動的生成、更新、タグ判定、解放などを一括管理するクラス
class AnimalManager
{
public:
	// コンボやレベル判定で使用するタグの履歴バッファ
	struct SaveTagDate
	{
		AnimalMove::Tag_animal tag1;
		AnimalMove::Tag_animal tag2;
		AnimalMove::Tag_animal tag3;
	};

public:
	AnimalManager();
	~AnimalManager();

	/*
	 * @brief 指定された種類の動物をランダムな位置に複数生成する
	 * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 動物の種類識別タグ, count: 生成個数
	 * [出力] なし
	 * [副作用] 動的メモリ確保された動物オブジェクトが管理リストに追加される
	 */
	void SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count);

	/*
	 * @brief 管理している動物の当たり判定判定および不要な動物の解放処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update();
	void Draw();

	/*
	 * @brief 死亡した（吸引された）動物のタグ履歴をチェックし、特定のコンボ条件を満たした場合にプレイヤーに経験値を与える
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 条件成立時にプレイヤーのXP加算、履歴バッファのクリア
	 */
	void judgmentAnimal();

	/*
	 * @brief 削除フラグ(mbDeleteFlag)が立っている動物オブジェクトを管理リストから除外する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] リストからのポインタ除外
	 */
	void EraseAnimal();

private:
	std::vector<AnimalMove*> mAnimals;  // 生成された動物オブジェクトのポインタ配列
	SaveTagDate s;                      // タグコンボ判定用のバッファデータ
	int mnTagCount;                     // 現在バッファに入っているタグの個数
};
