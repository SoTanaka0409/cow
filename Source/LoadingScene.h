#pragma once
#include "Scene.h"

// Scene to preload heavy 3D models asynchronously to prevent stuttering
class LoadingScene : public Scene
{
public:
	LoadingScene();
	~LoadingScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	int loading_timer_;
	bool load_started_; // 非同期ロードが既に開始済みか�Eフラグ
};
