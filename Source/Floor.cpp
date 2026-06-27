#include "Floor.h"
#include "DxLib.h"

Floor::Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	graphHandle = LoadGraph(filename.c_str());

	// 床の左上頂点の設定
	vertex[0].pos = VAdd(centerPos, topLeft);
	vertex[0].dif = GetColorU8(255, 255, 255, 255);
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;

	// 床の右上頂点の設定
	vertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, 0, topLeft.z));
	vertex[1].dif = GetColorU8(255, 255, 255, 255);
	vertex[1].spc = GetColorU8(0, 0, 0, 0);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;

	// 床の左下頂点の設定
	vertex[2].pos = VAdd(centerPos, VGet(topLeft.x, 0, bottomRight.z));
	vertex[2].dif = GetColorU8(255, 255, 255, 255);
	vertex[2].spc = GetColorU8(0, 0, 0, 0);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;

	// 床の右下頂点の設定
	vertex[3].pos = VAdd(centerPos, bottomRight);
	vertex[3].dif = GetColorU8(255, 255, 255, 255);
	vertex[3].spc = GetColorU8(0, 0, 0, 0);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;

	// 各頂点の法線ベクトルを計算して面全体の向きを統一する
	VECTOR norm = VCross(
		VSub(vertex[0].pos, vertex[1].pos),
		VSub(vertex[0].pos, vertex[2].pos)
	);
	norm = VNorm(norm);

	for (int i = 0; i < 4; i++)
	{
		vertex[i].norm = norm;
	}
}

Floor::~Floor()
{
	DeleteGraph(graphHandle);
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

	// ライティング計算を無効化し、テクスチャ本来の色で床を描画する
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex, 4, index, 2, graphHandle, TRUE);
	SetUseLighting(true);
}

