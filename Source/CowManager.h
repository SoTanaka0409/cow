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
	 * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 牛の種類識別タグ, count: 生成個数, fever: フィーバー中かどうか
	 * [出力] なし
	 * [副作用] 動的メモリ確保された牛オブジェクトが管理リストに追加されるか、プールから復帰する
	 */
	void SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool fever = false);

	/*
	 * @brief 管理している全ての牛の更新および不要な牛のリスト整理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各牛のUpdate実行と、削除フラグの立った牛のプール返却
	 */
	void Update();

	/*
	 * @brief 管理している牛の描画処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief 削除フラグ(deleteFlag)が立っている牛オブジェクトを管理リストから除外しプールへ返す
	 * [入力] なし
	 * [出力] なし
	 * [副作用] リストからのポインタ除外、プールへの追加
	 */
	void EraseCow();

private:
	std::vector<CowMove*>cows;         // 生成された牛オブジェクトのポインタ配列
	std::map<CowMove::Tag_cow, std::vector<CowMove*>> pools; // オブジェクトプール
};
