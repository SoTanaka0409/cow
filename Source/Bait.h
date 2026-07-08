#pragma once
#include "Object3D.h"
#include "Model.h"
#include "dxlib.h"
#include "string"
#include "GameTimer.h"

// 牛を引き寄せるための餌（Bait）オブジェクトを制御するクラス
class Bait : public Object3D
{
public:
	/*
	 * @brief 餌オブジェクトを配置し、有効時間用のタイマーと吸引コライダーを設定する
	 * [入力] filename: モデルのファイルパス, pos: 配置座標
	 * [出力] なし
	 * [副作用] GameTimerインスタンスの生成、コライダーサイズ設定
	 */
	Bait(std::string filename, VECTOR pos);
	virtual ~Bait();

	void Update() override;
	void Draw() override;

	virtual void OnEnter(Collider* collider, Collider* check) override;

public:
	Model* mpModel;          // 餌の3Dモデルへのポインタ

private:
	GameTimer* mpGameTimer;  // 有効時間をカウントするタイマーオブジェクトへのポインタ
};

