#include "AttachmentModel.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex)
	: Object3D(VGet(0.0f, 0.0f, 0.0f)) // 親モデルの位置に同期するため初期値は原点にする
	, mnParentHandle(parentModelHandle)
	, mnParentFrameIndex(parentFrameIndex)
{
	mnHandle = MV1LoadModel(filename.c_str());
}

AttachmentModel::~AttachmentModel()
{
	MV1DeleteModel(mnHandle);
}

void AttachmentModel::Update()
{
	// アタッチ先（親モデル）の特定ボーンの現在位置・回転・スケールを合成したマトリクスを取得する
	MATRIX matrix = MV1GetFrameLocalWorldMatrix(mnParentHandle, mnParentFrameIndex);

	// 取得したマトリクスをこのアタッチオブジェクトに直接セットして完全に同期させる
	MV1SetMatrix(mnHandle, matrix);
}

void AttachmentModel::Draw()
{
	MV1DrawModel(mnHandle);
}
