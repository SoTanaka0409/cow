#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include <vector>

/// @brief 空間の区切りや衝突判定の基準として扱うため
class Wall : public Object3D
{
public:
	/// @brief 描画用リソースと空間内の配置位置を初期化するため
	/// @param filename 画像パス
	/// @param centerPos 中心座標
	/// @param topLeft 左上相対位置
	/// @param bottomRight 右下相対位置
	/// @details 画像メモリの確保と頂点配列の構築を行う
	Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);
	virtual ~Wall();

	/// @brief 壁の動的な状態変化を毎フレーム反映させるため
	/// @details 状態変数を更新する
	void Update() override;

	/// @brief プレイヤーに壁の存在を視覚的に伝えるため
	/// @details 画面にポリゴンを描画する
	void Draw() override;

	/// @brief 外部クラスが壁との当たり判定を計算できるようにするため
	/// @return 壁を構成する4つの頂点データ
	std::vector<VERTEX3D> GetVertex();

private:
	int graph_handle_;   ///< 描画やリソース管理に使用するハンドル
	VERTEX3D vertex_[4]; ///< 描画や当たり判定に使用する頂点情報
};
