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
	int mLoadingTimer;
	bool mbLoadStarted; // 非同期ロードが既に開始済みかのフラグ
};
