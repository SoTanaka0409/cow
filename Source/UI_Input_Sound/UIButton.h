#pragma once
#include "SelectionManager.h"
#include "DxLib.h"
#include <cmath>

struct UIButton
{
	SelectionManager::Title type; ///< 現在の状態や種別を管理する値
	int graph_handle;             ///< 描画やリソース管理に使用するハンドル
	int x, y;                     ///< 座標や位置情報を管理する値
	int w, h;                     ///< 座標や位置情報を管理する値
	bool is_hover;
	float hover_scale_;
	float hover_glow_alpha_;                ///< 状態の有効・無効を管理するフラグ
	float animation_offset;       ///< 内部状態を管理する値

	UIButton();

	/// @brief 初期化処理
	void Initialize(SelectionManager::Title t, int handle, int px, int py, float anim_offset);
	/// @brief 毎フレームの更新処理
	void Update(int mouse_x, int mouse_y);
	/// @brief 描画処理
	void Draw(int frame_count) const;
};
