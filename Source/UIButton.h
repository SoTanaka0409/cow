#pragma once
#include "SelectionManager.h"
#include "DxLib.h"
#include <cmath>

struct UIButton
{
	SelectionManager::Title type;
	int graph_handle;
	int x, y;
	int w, h;
	bool is_hover;
	float animation_offset;

	UIButton();

	void Initialize(SelectionManager::Title t, int handle, int px, int py, float anim_offset);
	void Update(int mouse_x, int mouse_y);
	void Draw(int frame_count) const;
};