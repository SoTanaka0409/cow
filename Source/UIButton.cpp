#include "UIButton.h"

UIButton::UIButton()
	: type(SelectionManager::Title::title)
	, graph_handle(-1)
	, x(0), y(0)
	, w(0), h(0)
	, is_hover(false)
	, animation_offset(0.0f)
{
}

void UIButton::Initialize(SelectionManager::Title t, int handle, int px, int py, float anim_offset)
{
	type = t;
	graph_handle = handle;
	x = px;
	y = py;
	animation_offset = anim_offset;
	is_hover = false;

	if (graph_handle != -1) {
		GetGraphSize(graph_handle, &w, &h);
	} else {
		w = 0;
		h = 0;
	}
}

void UIButton::Update(int mouse_x, int mouse_y)
{
	if (mouse_x >= x && mouse_x <= x + w &&
		mouse_y >= y && mouse_y <= y + h)
	{
		is_hover = true;
	}
	else
	{
		is_hover = false;
	}
}

void UIButton::Draw(int frame_count) const
{
	if (graph_handle == -1) return;

	float wave = sin(frame_count * 0.05f + animation_offset) * 10.0f;
	int drawY = y + (int)wave;

	if (is_hover)
	{
		int expand = 15;
		DrawExtendGraph(
			x - expand,
			drawY - expand,
			x + w + expand,
			drawY + h + expand,
			graph_handle, TRUE);
	}
	else
	{
		DrawGraph(x, drawY, graph_handle, TRUE);
	}
}