#include "Floor.h"
#include "DxLib.h"
#include "Master.h"

/*
 * 入力: filename (テクスチャパス), centerPos (基準座標), topLeft (左上オフセット), bottomRight (右下オフセット)
 * 出力: なし
 * 副作用: テクスチャのロードおよびポリゴン描画用の頂点データの初期化
 */
Floor::Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	: Object3D(centerPos)
{
	graph_handle_ = Master::resource_manager_->LoadGraphics(filename.c_str());

	// テクスチャ本来の色を出力しつつ、光源による不自然なテカり(反射)を防ぐため、頂点色を白・スペキュラを無効化する
	vertex_[0].pos = VAdd(centerPos, topLeft);
	vertex_[0].dif = GetColorU8(255, 255, 255, 255);
	vertex_[0].spc = GetColorU8(0, 0, 0, 0);
	vertex_[0].u = 0.0f;
	vertex_[0].v = 0.0f;

	vertex_[1].pos = VAdd(centerPos, VGet(bottomRight.x, 0, topLeft.z));
	vertex_[1].dif = GetColorU8(255, 255, 255, 255);
	vertex_[1].spc = GetColorU8(0, 0, 0, 0);
	vertex_[1].u = 1.0f;
	vertex_[1].v = 0.0f;

	vertex_[2].pos = VAdd(centerPos, VGet(topLeft.x, 0, bottomRight.z));
	vertex_[2].dif = GetColorU8(255, 255, 255, 255);
	vertex_[2].spc = GetColorU8(0, 0, 0, 0);
	vertex_[2].u = 0.0f;
	vertex_[2].v = 1.0f;

	vertex_[3].pos = VAdd(centerPos, bottomRight);
	vertex_[3].dif = GetColorU8(255, 255, 255, 255);
	vertex_[3].spc = GetColorU8(0, 0, 0, 0);
	vertex_[3].u = 1.0f;
	vertex_[3].v = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		vertex_[i].norm = VGet(0.0f, 1.0f, 0.0f);
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
Floor::~Floor()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
void Floor::Update()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ライティング設定の一時変更と3Dポリゴンの描画
 */
void Floor::Draw()
{
	WORD index[6];

	// 外部仕様依存: DxLibの左手座標系において、時計回りの頂点順序が「表面」として認識されるためインデックス順序を固定する
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	DrawPolygonIndexed3D(vertex_, 4, index, 2, graph_handle_, TRUE);
}
