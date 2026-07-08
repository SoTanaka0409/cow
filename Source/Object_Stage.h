#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

// ステージ内に配置される各種静的3Dオブジェクト（障害物や装飾オブジェクトなど）を制御するクラス
class Object_Stage : public Object3D
{
public:
	/*
	 * @brief ステージ装飾オブジェクトを読み込み、位置、拡大率、回転角度を設定して初期化する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度
	 * [出力] なし
	 * [副作用] Modelインスタンスが動的確保される
	 */
	Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle);
	virtual ~Object_Stage();

	void Update() override;
	void Draw() override;

	void SetScale(float scale);
	void SetRotation(VECTOR angle);
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	Model* mpModel;          // 制御対象の3Dモデルへのポインタ
};

