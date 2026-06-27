#include "Wall.h"

Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	SetTag(Object3D::Tag3D_Wall);
	graphHandle = LoadGraph(filename.c_str());

	// 左上頂点の設定
	vertex[0].pos = VAdd(centerPos, topLeft);
	vertex[0].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex[0].dif = GetColorU8(255, 255, 255, 255);
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	// 右上頂点の設定
	vertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	vertex[1].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex[1].dif = GetColorU8(255, 255, 255, 255);
	vertex[1].spc = GetColorU8(0, 0, 0, 0);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;
	vertex[1].su = 1.0f;
	vertex[1].sv = 0.0f;

	// 左下頂点の設定
	vertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	vertex[2].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex[2].dif = GetColorU8(255, 255, 255, 255);
	vertex[2].spc = GetColorU8(0, 0, 0, 0);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;
	vertex[2].su = 0.0f;
	vertex[2].sv = 1.0f;

	// 右下頂点の設定
	vertex[3].pos = VAdd(centerPos, bottomRight);
	vertex[3].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex[3].dif = GetColorU8(255, 255, 255, 255);
	vertex[3].spc = GetColorU8(0, 0, 0, 0);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;
	vertex[3].su = 1.0f;
	vertex[3].sv = 1.0f;

	// ポリゴンの頂点座標から法線ベクトルを計算して各頂点に設定する
	VECTOR norm = VCross(
		VSub(vertex[0].pos, vertex[1].pos),
		VSub(vertex[0].pos, vertex[2].pos)
	);
	norm = VNorm(norm);
	vertex[0].norm = norm;
	vertex[1].norm = norm;
	vertex[2].norm = norm;
	vertex[3].norm = norm;
}

Wall::~Wall()
{
	DeleteGraph(graphHandle);
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

	// ライティングを無効化して、テクスチャ本来の色合いで壁を描画する
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex, 4, index, 2, graphHandle, true);
	SetUseLighting(true);
}

