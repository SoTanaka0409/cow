#include "Wall.h"
#include "DxLib.h"
#include "Master.h"

Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	SetTag(Object3D::kTag3dWall);
	graph_handle_ = Master::mpResourceManager->LoadGraphics(filename.c_str());

	// 左上頂点の設宁E
	vertex_[0].pos = VAdd(centerPos, topLeft);
	vertex_[0].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[0].dif = GetColorU8(255, 255, 255, 255);
	vertex_[0].spc = GetColorU8(0, 0, 0, 0);
	vertex_[0].u = 0.0f;
	vertex_[0].v = 0.0f;
	vertex_[0].su = 0.0f;
	vertex_[0].sv = 0.0f;

	// 右上頂点の設宁E
	vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	vertex_[1].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[1].dif = GetColorU8(255, 255, 255, 255);
	vertex_[1].spc = GetColorU8(0, 0, 0, 0);
	vertex_[1].u = 1.0f;
	vertex_[1].v = 0.0f;
	vertex_[1].su = 1.0f;
	vertex_[1].sv = 0.0f;

	// 左下頂点の設宁E
	vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	vertex_[2].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[2].dif = GetColorU8(255, 255, 255, 255);
	vertex_[2].spc = GetColorU8(0, 0, 0, 0);
	vertex_[2].u = 0.0f;
	vertex_[2].v = 1.0f;
	vertex_[2].su = 0.0f;
	vertex_[2].sv = 1.0f;

	// 右下頂点の設宁E
	vertex_[3].pos = VAdd(centerPos, bottomRight);
	vertex_[3].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[3].dif = GetColorU8(255, 255, 255, 255);
	vertex_[3].spc = GetColorU8(0, 0, 0, 0);
	vertex_[3].u = 1.0f;
	vertex_[3].v = 1.0f;
	vertex_[3].su = 1.0f;
	vertex_[3].sv = 1.0f;

	// ポリゴンの頂点座標から法線�Eクトルを計算して�ꍇ��点に設定すめE
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

std::vector<VERTEX3D> Wall::GetVertex()
{
	std::vector<VERTEX3D> result;
	result.push_back(vertex_[0]);
	result.push_back(vertex_[1]);
	result.push_back(vertex_[2]);
	result.push_back(vertex_[3]);
	return result;
}

Wall::~Wall()
{

}

void Wall::Update()
{
}

void Wall::Draw()
{
	WORD index[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	// ライチE��ングを無効化して、テクスチャ本来の色合いで壁を描画する
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex_, 4, index, 2, graph_handle_, true);
	SetUseLighting(true);
}
