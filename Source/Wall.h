#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include <vector>

// 空間の区切りや衝突判定の基準として扱うため
class Wall : public Object3D
{
public:
	/*
	 * 描画用リソースと空間内の配置位置を初期化するため
	 * [入力] filename: 画像パス, centerPos: 中心座標, topLeft: 左上相対位置, bottomRight: 右下相対位置
	 * [出力] なし
	 * [副作用] 画像メモリの確保と頂点配列の構築を行う
	 */
	Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);
	virtual ~Wall();

	/*
	 * 壁の動的な状態変化を毎フレーム反映させるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 状態変数を更新する
	 */
	void Update() override;

	/*
	 * プレイヤーに壁の存在を視覚的に伝えるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面にポリゴンを描画する
	 */
	void Draw() override;

	/*
	 * 外部クラスが壁との当たり判定を計算できるようにするため
	 * [入力] なし
	 * [出力] 壁を構成する4つの頂点データ
	 * [副作用] なし
	 */
	std::vector<VERTEX3D> GetVertex();

private:
	int graph_handle_;
	VERTEX3D vertex_[4];
};