#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"

// ステージ上の牛たちの動的生成、更新、タグ判定、上限管理、解放などを一括管理するクラス
class CowManager
{
public:
	CowManager();
	~CowManager();

	/*
	 * @brief 指定された種類の牛をランダムな位置に複数生成する（最大30匹制限あり）
	 * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 牛の種類識別タグ, count: 生成個数, mfever: フィーバー中かどうか
	 * [出力] なし
	 * [副作用] 動的メモリ確保された牛オブジェクトが管理リストに追加される
	 */
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever = false);

	/*
	 * @brief 管理している牛のタグコンボ判定および不要な牛のリスト整理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update();
	void Draw();

	/*
	 * @brief 削除フラグ(mbDeleteFlag)が立っている牛オブジェクトを管理リストから除外する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] リストからのポインタ除外
	 */
	void EraseCow();

private:
	std::vector<CowMove*>mCows;         // 生成された牛オブジェクトのポインタ配列
	std::map<CowMove::Tag_cow, std::vector<CowMove*>> mPools; // オブジェクトプール
};
