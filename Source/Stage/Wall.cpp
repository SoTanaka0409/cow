#include "Wall.h"
#include "DxLib.h"
#include "Master.h"

/*
 * 描画用リソースと空間内の配置位置を初期化するため
 * [入力] filename: 画像パス, centerPos: 中心座標, topLeft: 左上相対位置, bottomRight: 右下相対位置
 * [出力] なし
 * [副作用] 画像メモリの確保と頂点配列の構築を行う
 */
Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	SetTag(Object3D::kTag3dWall);
	if (!filename.empty()) { graph_handle_ = Master::resource_manager_->LoadGraphics(filename.c_str()); } else { graph_handle_ = -1; }

	vertex_[0].pos = VAdd(centerPos, topLeft);
	vertex_[0].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[0].dif = GetColorU8(255, 255, 255, 255);
	vertex_[0].spc = GetColorU8(0, 0, 0, 0);
	vertex_[0].u = 0.0f;
	vertex_[0].v = 0.0f;
	vertex_[0].su = 0.0f;
	vertex_[0].sv = 0.0f;

	vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	vertex_[1].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[1].dif = GetColorU8(255, 255, 255, 255);
	vertex_[1].spc = GetColorU8(0, 0, 0, 0);
	vertex_[1].u = 1.0f;
	vertex_[1].v = 0.0f;
	vertex_[1].su = 1.0f;
	vertex_[1].sv = 0.0f;

	vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	vertex_[2].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[2].dif = GetColorU8(255, 255, 255, 255);
	vertex_[2].spc = GetColorU8(0, 0, 0, 0);
	vertex_[2].u = 0.0f;
	vertex_[2].v = 1.0f;
	vertex_[2].su = 0.0f;
	vertex_[2].sv = 1.0f;

	vertex_[3].pos = VAdd(centerPos, bottomRight);
	vertex_[3].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[3].dif = GetColorU8(255, 255, 255, 255);
	vertex_[3].spc = GetColorU8(0, 0, 0, 0);
	vertex_[3].u = 1.0f;
	vertex_[3].v = 1.0f;
	vertex_[3].su = 1.0f;
	vertex_[3].sv = 1.0f;

	VECTOR norm = VCross(
		VSub(vertex_[0].pos, vertex_[1].pos),
		VSub(vertex_[0].pos, vertex_[2].pos)
	);
	norm = VNorm(norm);
	vertex_[0].norm = norm;
	vertex_[1].norm = norm;
	vertex_[2].norm = norm;
	vertex_[3].norm = norm;
}

/*
 * 外部クラスが壁との当たり判定を計算できるようにするため
 * [入力] なし
 * [出力] 壁を構成する4つの頂点データ
 * [副作用] なし
 */
std::vector<VERTEX3D> Wall::GetVertex()
{
	std::vector<VERTEX3D> result;
	result.push_back(vertex_[0]);
	result.push_back(vertex_[1]);
	result.push_back(vertex_[2]);
	result.push_back(vertex_[3]);
	return result;
}

/*
 * オブジェクト破棄時の後処理を行うため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Wall::~Wall()
{

}

/*
 * 壁の動的な状態変化を毎フレーム反映させるため
 * [入力] なし
 * [出力] なし
 * [副作用] 状態変数を更新する
 */
void Wall::Update()
{
}

/*
 * プレイヤーに壁の存在を視覚的に伝えるため
 * [入力] なし
 * [出力] なし
 * [副作用] 画面にポリゴンを描画する
 */
void Wall::Draw()
{
	WORD index[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	// テクスチャ本来の色合いで描画するため
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex_, 4, index, 2, graph_handle_, true);
	SetUseLighting(true);
}

