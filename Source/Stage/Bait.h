#pragma once
#include "Object3D.h"
#include "Model.h"
#include "dxlib.h"
#include "string"
#include "GameTimer.h"

class Bait : public Object3D
{
public:
	Bait(std::string filename, VECTOR pos);
	virtual ~Bait();

	void Update() override;
	void Draw() override;

	virtual void OnEnter(Collider* collider, Collider* check) override;

public:
	Model* model_; ///< 3Dモデルの管理に使用する情報

private:
	GameTimer* game_timer_; ///< 時間経過や処理間隔を管理するカウンター
};
