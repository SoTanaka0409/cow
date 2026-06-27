#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

// 背景に配置される山などの静的背景3Dオブジェクトを制御するクラス
class Mountain : public Object3D
{
public:
	/*
	 * @brief 背景モデルを読み込み、位置、拡大率、回転角度を設定して初期化する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度
	 * [出力] なし
	 * [副作用] Modelインスタンスが動的確保される
	 */
	Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle);
	virtual ~Mountain();

	void Update() override;
	void Draw() override;

	void SetScale(VECTOR scale);
	void SetRotation(VECTOR angle);
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	Model* model;          // 制御対象の3Dモデルへのポインタ
};

