#include "Wall.h"
#include "DxLib.h"
#include "Master.h"

Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	SetTag(Object3D::kTag3dWall);
	graph_handle_ = Master::mpResourceManager->LoadGraphics(filename.c_str());

	// 蟾ｦ荳企らせ縺ｮ險ｭ螳・
	vertex_[0].pos = VAdd(centerPos, topLeft);
	vertex_[0].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[0].dif = GetColorU8(255, 255, 255, 255);
	vertex_[0].spc = GetColorU8(0, 0, 0, 0);
	vertex_[0].u = 0.0f;
	vertex_[0].v = 0.0f;
	vertex_[0].su = 0.0f;
	vertex_[0].sv = 0.0f;

	// 蜿ｳ荳企らせ縺ｮ險ｭ螳・
	vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	vertex_[1].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[1].dif = GetColorU8(255, 255, 255, 255);
	vertex_[1].spc = GetColorU8(0, 0, 0, 0);
	vertex_[1].u = 1.0f;
	vertex_[1].v = 0.0f;
	vertex_[1].su = 1.0f;
	vertex_[1].sv = 0.0f;

	// 蟾ｦ荳矩らせ縺ｮ險ｭ螳・
	vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	vertex_[2].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[2].dif = GetColorU8(255, 255, 255, 255);
	vertex_[2].spc = GetColorU8(0, 0, 0, 0);
	vertex_[2].u = 0.0f;
	vertex_[2].v = 1.0f;
	vertex_[2].su = 0.0f;
	vertex_[2].sv = 1.0f;

	// 蜿ｳ荳矩らせ縺ｮ險ｭ螳・
	vertex_[3].pos = VAdd(centerPos, bottomRight);
	vertex_[3].norm = VGet(1.0f, 0.0f, 0.0f);
	vertex_[3].dif = GetColorU8(255, 255, 255, 255);
	vertex_[3].spc = GetColorU8(0, 0, 0, 0);
	vertex_[3].u = 1.0f;
	vertex_[3].v = 1.0f;
	vertex_[3].su = 1.0f;
	vertex_[3].sv = 1.0f;

	// 繝昴Μ繧ｴ繝ｳ縺ｮ鬆らせ蠎ｧ讓吶°繧画ｳ慕ｷ壹・繧ｯ繝医Ν繧定ｨ育ｮ励＠縺ｦ蜷・らせ縺ｫ險ｭ螳壹☆繧・
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

	// 繝ｩ繧､繝・ぅ繝ｳ繧ｰ繧堤┌蜉ｹ蛹悶＠縺ｦ縲√ユ繧ｯ繧ｹ繝√Ε譛ｬ譚･縺ｮ濶ｲ蜷医＞縺ｧ螢√ｒ謠冗判縺吶ｋ
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex_, 4, index, 2, graph_handle_, true);
	SetUseLighting(true);
}
