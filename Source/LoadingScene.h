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
	bool mbLoadStarted; // 髱槫酔譛溘Ο繝ｼ繝峨′譌｢縺ｫ髢句ｧ区ｸ医∩縺九・繝輔Λ繧ｰ
};
