#include "UIButton.h"

// 副作用：ボタンの種類、座標、テクスチャハンドル、ホバー状態、アニメーション位相の初期化
UIButton::UIButton()
	: type(SelectionManager::Title::title)
	, graph_handle(-1)
	, x(0), y(0)
	, w(0), h(0)
	, is_hover(false)
	, animation_offset(0.0f)
{
}

// 入力：t=ボタンの種類ID, handle=DxLib画像ハンドル, px/py=描画基準座標, anim_offset=波形アニメーションの初期位相
// 副作用：画像アセット本来の解像度（w, h）の自動取得
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
	}
	else {
		w = 0;
		h = 0;
	}
}

// 入力：mouse_x, mouse_y = 現在の画面上のマウスポインタ座標
// 副作用：マウスがボタンの矩形領域（AABB）と重なっているかどうかに応じたホバー状態（is_hover）の更新
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

// 入力：frame_count = ゲーム全体の共通フレームカウンター
// 副作用：ボタン画像の画面描画
void UIButton::Draw(int frame_count) const
{
	if (graph_handle == -1) return;

	// ボタン配置が単調になるのを防ぐため、個別の初期位相（animation_offset）を用いて互い違いに浮遊させる
	float wave = sin(frame_count * 0.05f + animation_offset) * 10.0f;
	int drawY = y + (int)wave;

	// 選択中の視覚的フィードバックを強調するため、ホバー時は中心から外側へ15pxずつ拡大して描画する
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