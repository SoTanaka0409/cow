#include "Stage.h"
#include "Master.h"

/*
 * 入力: initPos (初期座標), stageModelName (描画用モデル), stageCollisionModelName (判定用モデル)
 * 出力: なし
 * 副作用: 描画用および判定用モデルの読み込みと、判定情報(コリジョン)のセットアップ
 */
Stage::Stage(VECTOR initPos, std::string stageModelName, std::string stageCollisionModelName)
	: Object3D(initPos)
{
	SetTag(Object3D::kTag3dStage);

	// ハイポリゴンの描画用モデルで直接当たり判定を行うと処理落ち(パフォーマンス低下)を招くため、
	// 軽量な判定専用モデルを別途読み込んで使用する
	model_handle_ = Master::mpResourceManager->LoadModel(stageModelName);
	collision_handle_ = Master::mpResourceManager->LoadModel(stageCollisionModelName);

	float StageSize = 5.0f;
	MV1SetScale(model_handle_, VGet(StageSize, 0.3f, StageSize));
	MV1SetScale(collision_handle_, VGet(StageSize, 0.3f, StageSize));
	MV1SetPosition(collision_handle_, initPos);
	MV1SetPosition(model_handle_, initPos);

	// パフォーマンス制約: 毎フレームの衝突判定負荷を軽減するため、ロード時に空間分割などの判定メタデータを事前構築する
	MV1SetupCollInfo(collision_handle_);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ロードしたモデルリソースの破棄
 */
Stage::~Stage()
{
	MV1DeleteModel(model_handle_);
	MV1DeleteModel(collision_handle_);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
void Stage::Update()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ステージの3Dモデル描画
 */
void Stage::Draw()
{
	MV1DrawModel(model_handle_);
}

/*
 * 入力: pos1, pos2 (カプセルの両端座標), r (カプセルの半径)
 * 出力: 衝突していればtrue
 * 副作用: なし (一時対応としてデバッグ用のポリゴン描画を含む)
 */
bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(collision_handle_, -1, pos1, pos2, r);

	if (result.HitNum >= 1)
	{
		// 暫定対応: 地形抜けバグの検証用として、ヒットしたポリゴンを赤く可視化している（期限: リリースビルド作成時まで）
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

	// 外部仕様依存: DxLib内部で動的確保された判定結果(配列)を破棄しないと深刻なメモリリークを引き起こすため必ず解放する
	MV1CollResultPolyDimTerminate(result);

	return (result.HitNum >= 1);
}

/*
 * 入力: pos1, pos2 (線分の始点と終点)
 * 出力: 衝突点の座標 (未衝突時はゼロベクトル)
 * 副作用: なし
 */
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

/*
 * 入力: pos1, pos2 (線分の始点と終点)
 * 出力: 衝突点の座標
 * 副作用: 画面左上へのヒット座標のテキスト描画
 */
VECTOR Stage::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	auto result = MV1CollCheck_Line(collision_handle_, -1, pos1, pos2);

	// デバッグ表示専用関数。キャラクターの接地判定や、カメラが壁にめり込んだ際の座標を視覚的に検証するために用いる
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