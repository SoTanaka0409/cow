#pragma once
#include "Scene.h"

/// @brief Scene to preload heavy 3D models asynchronously to prevent stuttering
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
	int loading_timer_; ///< 時間経過や処理間隔を管理するカウンター
	bool load_started_; ///< 内部状態を管理する値
	int font_handle_;   ///< NOW LOADING...表x8eｦ用フォントハンドル
};
