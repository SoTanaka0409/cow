#pragma once
#include "Object3D.h"
#include <vector>
#include <string>

// ゲーム内の床面（平面ポリゴン）を生成・描画するクラス
class Floor : public Object3D
{
public:
	/*
	 * @brief 床ポリゴンのテクスチャ画像と範囲パラメータを設定して初期化する
	 * [入力] filename: テクスチャ画像へのパス, centerPos: 床の基準中心座標, topLeft: 左上端の相対座標, bottomRight: 右下端の相対座標
	 * [出力] なし
	 * [副作用] テクスチャがメモリにロードされ、頂点バッファ(vertex)が設定される
	 */
	Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);
	virtual ~Floor();

	void Update() override;
	void Draw() override;

private:
	int graphHandle;      // 床のテクスチャグラフィックハンドル
	VERTEX3D vertex[4];    // 床面を構成する4つの頂点データ
};

