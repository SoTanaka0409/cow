#include "Floor.h"
#include "DxLib.h"

Floor::Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	mnGraphHandle = LoadGraph(filename.c_str());

	// 床の左上頂点の設定
	mVertex[0].pos = VAdd(centerPos, topLeft);
	mVertex[0].dif = GetColorU8(255, 255, 255, 255);
	mVertex[0].spc = GetColorU8(0, 0, 0, 0);
	mVertex[0].u = 0.0f;
	mVertex[0].v = 0.0f;

	// 床の右上頂点の設定
	mVertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, 0, topLeft.z));
	mVertex[1].dif = GetColorU8(255, 255, 255, 255);
	mVertex[1].spc = GetColorU8(0, 0, 0, 0);
	mVertex[1].u = 1.0f;
	mVertex[1].v = 0.0f;

	// 床の左下頂点の設定
	mVertex[2].pos = VAdd(centerPos, VGet(topLeft.x, 0, bottomRight.z));
	mVertex[2].dif = GetColorU8(255, 255, 255, 255);
	mVertex[2].spc = GetColorU8(0, 0, 0, 0);
	mVertex[2].u = 0.0f;
	mVertex[2].v = 1.0f;

	// 床の右下頂点の設定
	mVertex[3].pos = VAdd(centerPos, bottomRight);
	mVertex[3].dif = GetColorU8(255, 255, 255, 255);
	mVertex[3].spc = GetColorU8(0, 0, 0, 0);
	mVertex[3].u = 1.0f;
	mVertex[3].v = 1.0f;

	// 各頂点の法線ベクトルを計算して面全体の向きを統一する
	VECTOR norm = VCross(
		VSub(mVertex[0].pos, mVertex[1].pos),
		VSub(mVertex[0].pos, mVertex[2].pos)
	);
	norm = VNorm(norm);

	for (int i = 0; i < 4; i++)
	{
		mVertex[i].norm = norm;
	}
}

Floor::~Floor()
{
	DeleteGraph(mnGraphHandle);
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
	DrawPolygonIndexed3D(mVertex, 4, index, 2, mnGraphHandle, TRUE);
	SetUseLighting(true);
}
