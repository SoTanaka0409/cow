#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include <vector>

// ゲーム内の壁面（四角形ポリゴン）を生成・描画するクラス
class Wall : public Object3D
{
public:
	/*
	 * @brief 壁面のテクスチャ画像と範囲パラメータを設定して初期化する
	 * [入力] filename: テクスチャ画像へのパス, centerPos: 壁の中心座標, topLeft: 左上の相対座標, bottomRight: 右下の相対座標
	 * [出力] なし
	 * [副作用] テクスチャがロード時にロードされ、頂点バッファ(mVertex)が設定される
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
	std::vector<VERTEX3D> GetVertex();

private:
	int mnGraphHandle;      // 壁テクスチャのグラフィックハンドル
	VERTEX3D mVertex[4];    // 壁面を構成する4つの頂点データ
};
