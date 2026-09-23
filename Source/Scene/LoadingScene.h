#pragma once
#include "Scene.h"

/// @brief Scene to preload heavy 3D models asynchronously to prevent stuttering
class LoadingScene : public Scene
{
public:
	LoadingScene();
	~LoadingScene();

	/// @brief 初期化処理
	void Initialize() override;
	/// @brief 毎フレームの更新処理
	void Update() override;
	/// @brief 描画処理
	void Draw() override;
	/// @brief 終了処理
	void Finalize() override;

private:
	int loading_timer_; ///< 時間経過や処理間隔を管理するカウンター
	bool load_started_; ///< 内部状態を管理する値
	int font_handle_;   ///< NOW LOADING...表x8eｦ用フォントハンドル
};
