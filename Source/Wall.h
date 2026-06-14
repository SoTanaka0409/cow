#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include <vector>

// ゲーム内の壁面（平面ポリゴン）を生成・描画するクラス
class Wall : public Object3D
{
public:
	/*
	 * @brief 壁面のテクスチャ画像と範囲パラメータを設定して初期化する
	 * [入力] filename: テクスチャ画像へのパス, centerPos: 壁の中心座標, topLeft: 左上端の相対座標, bottomRight: 右下端の相対座標
	 * [出力] なし
	 * [副作用] テクスチャがメモリにロードされ、頂点バッファ(mVertex)が設定される
	 */
	Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);
	virtual ~Wall();

	void Update() override;
	void Draw() override;

	/*
	 * @brief 壁を構成する4つの頂点データを取得する
	 * [入力] なし
	 * [出力] VERTEX3D構造体のベクター
	 * [副作用] なし
	 */
	std::vector<VERTEX3D> GetVertex()
	{
		std::vector<VERTEX3D> result;
		result.push_back(mVertex[0]);
		result.push_back(mVertex[1]);
		result.push_back(mVertex[2]);
		result.push_back(mVertex[3]);
		return result;
	}

private:
	int mnGraphHandle;      // 壁テクスチャのグラフィックハンドル
	VERTEX3D mVertex[4];    // 壁面を構成する4つの頂点データ
};

