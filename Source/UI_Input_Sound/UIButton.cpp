#include "UIButton.h"
#include "Master.h"

/// @brief ボタンの種類、座標、テクスチャハンドル、ホバー状態、アニメーション位相の初期化
UIButton::UIButton()
	: type(SelectionManager::Title::title)
	, graph_handle(-1)
	, x(0), y(0)
	, w(0), h(0)
	, is_hover(false)
	, hover_scale_(0.0f)
	, hover_glow_alpha_(0.0f)
	, animation_offset(0.0f)
{
}

/// @param t ボタンの種類ID
/// @param handle DxLib画像ハンドル
/// @param anim_offset 波形アニメーションの初期位相
/// @brief px/py=描画基準座標
/// @details 画像アセット本来の解像度（w, h）の自動取得
void UIButton::Initialize(SelectionManager::Title t, int handle, int px, int py, float anim_offset)
{
	type = t;
	graph_handle = handle;
	x = px;
	y = py;
	animation_offset = anim_offset;
	is_hover = false;
	hover_scale_ = 0.0f;
	hover_glow_alpha_ = 0.0f;

	if (graph_handle != -1)
	{
		GetGraphSize(graph_handle, &w, &h);
	}
	else
	{
		w = 0;
		h = 0;
	}
}

/// @param mouse_y 現在の画面上のマウスポインタ座標
/// @brief mouse_x
/// @details マウスがボタンの矩形領域（AABB）と重なっているかどうかに応じたホバー状態（is_hover）の更新
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
	
	// のどかなSFの「ふわっと」した挙動を出すため、フレームレート非依存の補間で滑らかに変化させる
	float dt = Master::GetDeltaTimeScaler();
	if (is_hover)
	{
		hover_scale_ += (1.0f - hover_scale_) * 0.2f * dt;
		hover_glow_alpha_ += (255.0f - hover_glow_alpha_) * 0.15f * dt;
	}
	else
	{
		hover_scale_ += (0.0f - hover_scale_) * 0.2f * dt;
		hover_glow_alpha_ += (0.0f - hover_glow_alpha_) * 0.15f * dt;
	}
}

/// @param frame_count ゲーム全体の共通フレームカウンター
/// @brief ボタン画像の画面描画
void UIButton::Draw(int frame_count) const
{
	if (graph_handle == -1) return;

	// ボタン配置が単調になるのを防ぐため、個別の初期位相（animation_offset）を用いて互い違いに浮遊させる
	float wave = sin(frame_count * 0.05f + animation_offset) * 10.0f;
	int drawY = y + (int)wave;

	// 通常の拡大とADD（加算合成）によるSF風のふんわりした発光エフェクトを描画
	int expand = (int)(25.0f * hover_scale_);
	int center_x = x + w / 2;
	int center_y = drawY + h / 2;
	int target_w = (w / 2) + expand;
	int target_h = (h / 2) + expand;

	// メイン描画
	DrawExtendGraph(
		center_x - target_w,
		center_y - target_h,
		center_x + target_w,
		center_y + target_h,
		graph_handle, TRUE);

	// 発光層（ADD合成）
	if (hover_glow_alpha_ > 5.0f)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, (int)hover_glow_alpha_);
		// 発光層はメインよりさらに少しだけ大きくする
		int glow_w = target_w + 10;
		int glow_h = target_h + 10;
		DrawExtendGraph(
			center_x - glow_w,
			center_y - glow_h,
			center_x + glow_w,
			center_y + glow_h,
			graph_handle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
