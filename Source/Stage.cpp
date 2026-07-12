#include "Stage.h"
#include "Master.h"

Stage::Stage(VECTOR initPos, std::string stageModelName, std::string stageCollisionModelName)
	: Object3D(initPos)
{
	SetTag(Object3D::kTag3dStage);

	model_handle_ = Master::mpResourceManager->LoadModel(stageModelName);
	collision_handle_ = Master::mpResourceManager->LoadModel(stageCollisionModelName);

	// 実行時の負荷を軽減するため、�E期化時にポリゴン惁E��を事前構築しておく
	float StageSize = 5.0f;
	MV1SetScale(model_handle_, VGet(StageSize, 0.3f, StageSize));
	MV1SetScale(collision_handle_, VGet(StageSize, 0.3f, StageSize));
	MV1SetPosition(collision_handle_, initPos);
	MV1SetPosition(model_handle_, initPos);
	MV1SetupCollInfo(collision_handle_);
}

Stage::~Stage()
{
	MV1DeleteModel(model_handle_);
	MV1DeleteModel(collision_handle_);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	MV1DrawModel(model_handle_);
}

bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(collision_handle_, -1, pos1, pos2, r);

	if (result.HitNum >= 1)
	{
		for (int i = 0; i < result.HitNum; i++)
		{
			DrawTriangle3D(
				result.Dim[i].Position[0],
				result.Dim[i].Position[1],
				result.Dim[i].Position[2],
				GetColor(255, 0, 0),
				0
			);
		}
	}
	
	// メモリリークを防ぐため、動皁E��保された判定結果は�K�v��解放する
	MV1CollResultPolyDimTerminate(result);

	return (result.HitNum >= 1);
}

VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	auto result = MV1CollCheck_Line(collision_handle_, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
	}

	return ret;
}

VECTOR Stage::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	auto result = MV1CollCheck_Line(collision_handle_, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
		DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit: x:%f, y:%f. z:%f", ret.x, ret.y, ret.z);
	}
	else
	{
		DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit None");
	}

	return ret;
}