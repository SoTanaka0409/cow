#include "Floor.h"
#include "DxLib.h"
#include "Master.h"

Floor::Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	graph_handle_ = Master::mpResourceManager->LoadGraphics(filename.c_str());

	// 床�E左上頂点の設宁E
	vertex_[0].pos = VAdd(centerPos, topLeft);
	vertex_[0].dif = GetColorU8(255, 255, 255, 255);
	vertex_[0].spc = GetColorU8(0, 0, 0, 0);
	vertex_[0].u = 0.0f;
	vertex_[0].v = 0.0f;

	// 床�E右上頂点の設宁E
	vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, 0, topLeft.z));
	vertex_[1].dif = GetColorU8(255, 255, 255, 255);
	vertex_[1].spc = GetColorU8(0, 0, 0, 0);
	vertex_[1].u = 1.0f;
	vertex_[1].v = 0.0f;

	// 床�E左下頂点の設宁E
	vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, 0, bottomRight.z));
	vertex_[2].dif = GetColorU8(255, 255, 255, 255);
	vertex_[2].spc = GetColorU8(0, 0, 0, 0);
	vertex_[2].u = 0.0f;
	vertex_[2].v = 1.0f;

	// 床�E右下頂点の設宁E
	vertex_[3].pos = VAdd(centerPos, bottomRight);
	vertex_[3].dif = GetColorU8(255, 255, 255, 255);
	vertex_[3].spc = GetColorU8(0, 0, 0, 0);
	vertex_[3].u = 1.0f;
	vertex_[3].v = 1.0f;

	// �ꍇ��点の法線�Eクトルを計算して面全体�E向きを統一する
	VECTOR norm = VCross(
		VSub(vertex_[0].pos, vertex_[1].pos),
		VSub(vertex_[0].pos, vertex_[2].pos)
	);
	norm = VNorm(norm);

	for (int i = 0; i < 4; i++)
	{
		vertex_[i].norm = norm;
	}
}

Floor::~Floor()
{

}

void Floor::Update()
{
}

void Floor::Draw()
{
	WORD index[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	// ライチE��ング計算を無効化し、テクスチャ本来の色で床を描画する
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex_, 4, index, 2, graph_handle_, TRUE);
	SetUseLighting(true);
}

