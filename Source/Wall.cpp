#include "Wall.h"
#include "DxLib.h"
#include "Master.h"

Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	SetTag(Object3D::Tag3D_Wall);
	mnGraphHandle = Master::mpResourceManager->LoadGraphics(filename.c_str());

	// 左上頂点の設定
	mVertex[0].pos = VAdd(centerPos, topLeft);
	mVertex[0].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[0].dif = GetColorU8(255, 255, 255, 255);
	mVertex[0].spc = GetColorU8(0, 0, 0, 0);
	mVertex[0].u = 0.0f;
	mVertex[0].v = 0.0f;
	mVertex[0].su = 0.0f;
	mVertex[0].sv = 0.0f;

	// 右上頂点の設定
	mVertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	mVertex[1].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[1].dif = GetColorU8(255, 255, 255, 255);
	mVertex[1].spc = GetColorU8(0, 0, 0, 0);
	mVertex[1].u = 1.0f;
	mVertex[1].v = 0.0f;
	mVertex[1].su = 1.0f;
	mVertex[1].sv = 0.0f;

	// 左下頂点の設定
	mVertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	mVertex[2].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[2].dif = GetColorU8(255, 255, 255, 255);
	mVertex[2].spc = GetColorU8(0, 0, 0, 0);
	mVertex[2].u = 0.0f;
	mVertex[2].v = 1.0f;
	mVertex[2].su = 0.0f;
	mVertex[2].sv = 1.0f;

	// 右下頂点の設定
	mVertex[3].pos = VAdd(centerPos, bottomRight);
	mVertex[3].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[3].dif = GetColorU8(255, 255, 255, 255);
	mVertex[3].spc = GetColorU8(0, 0, 0, 0);
	mVertex[3].u = 1.0f;
	mVertex[3].v = 1.0f;
	mVertex[3].su = 1.0f;
	mVertex[3].sv = 1.0f;

	// ポリゴンの頂点座標から法線ベクトルを計算して各頂点に設定する
	VECTOR norm = VCross(
		VSub(mVertex[0].pos, mVertex[1].pos),
		VSub(mVertex[0].pos, mVertex[2].pos)
	);
	norm = VNorm(norm);
	mVertex[0].norm = norm;
	mVertex[1].norm = norm;
	mVertex[2].norm = norm;
	mVertex[3].norm = norm;
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

	// ライティングを無効化して、テクスチャ本来の色合いで壁を描画する
	SetUseLighting(false);
	DrawPolygonIndexed3D(mVertex, 4, index, 2, mnGraphHandle, true);
	SetUseLighting(true);
}

