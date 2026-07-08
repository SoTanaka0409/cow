#include "Wall.h"
#include "DxLib.h"
#include "Master.h"

Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	SetTag(Object3D::kTag3dWall);
	mnGraphHandle = Master::mpResourceManager->LoadGraphics(filename.c_str());

	// 蟾ｦ荳企らせ縺ｮ險ｭ螳・
	mVertex[0].pos = VAdd(centerPos, topLeft);
	mVertex[0].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[0].dif = GetColorU8(255, 255, 255, 255);
	mVertex[0].spc = GetColorU8(0, 0, 0, 0);
	mVertex[0].u = 0.0f;
	mVertex[0].v = 0.0f;
	mVertex[0].su = 0.0f;
	mVertex[0].sv = 0.0f;

	// 蜿ｳ荳企らせ縺ｮ險ｭ螳・
	mVertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	mVertex[1].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[1].dif = GetColorU8(255, 255, 255, 255);
	mVertex[1].spc = GetColorU8(0, 0, 0, 0);
	mVertex[1].u = 1.0f;
	mVertex[1].v = 0.0f;
	mVertex[1].su = 1.0f;
	mVertex[1].sv = 0.0f;

	// 蟾ｦ荳矩らせ縺ｮ險ｭ螳・
	mVertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	mVertex[2].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[2].dif = GetColorU8(255, 255, 255, 255);
	mVertex[2].spc = GetColorU8(0, 0, 0, 0);
	mVertex[2].u = 0.0f;
	mVertex[2].v = 1.0f;
	mVertex[2].su = 0.0f;
	mVertex[2].sv = 1.0f;

	// 蜿ｳ荳矩らせ縺ｮ險ｭ螳・
	mVertex[3].pos = VAdd(centerPos, bottomRight);
	mVertex[3].norm = VGet(1.0f, 0.0f, 0.0f);
	mVertex[3].dif = GetColorU8(255, 255, 255, 255);
	mVertex[3].spc = GetColorU8(0, 0, 0, 0);
	mVertex[3].u = 1.0f;
	mVertex[3].v = 1.0f;
	mVertex[3].su = 1.0f;
	mVertex[3].sv = 1.0f;

	// 繝昴Μ繧ｴ繝ｳ縺ｮ鬆らせ蠎ｧ讓吶°繧画ｳ慕ｷ壹・繧ｯ繝医Ν繧定ｨ育ｮ励＠縺ｦ蜷・らせ縺ｫ險ｭ螳壹☆繧・
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

std::vector<VERTEX3D> Wall::GetVertex()
{
	std::vector<VERTEX3D> result;
	result.push_back(mVertex[0]);
	result.push_back(mVertex[1]);
	result.push_back(mVertex[2]);
	result.push_back(mVertex[3]);
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

	// 繝ｩ繧､繝・ぅ繝ｳ繧ｰ繧堤┌蜉ｹ蛹悶＠縺ｦ縲√ユ繧ｯ繧ｹ繝√Ε譛ｬ譚･縺ｮ濶ｲ蜷医＞縺ｧ螢√ｒ謠冗判縺吶ｋ
	SetUseLighting(false);
	DrawPolygonIndexed3D(mVertex, 4, index, 2, mnGraphHandle, true);
	SetUseLighting(true);
}
