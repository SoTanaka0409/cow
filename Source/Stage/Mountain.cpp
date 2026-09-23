#include "Mountain.h"

/// @brief 背景演出用の静的モデルを構築するため
/// @param filename モデルのファイルパス
/// @param init_pos 初期座標
/// @param scale 拡大率
/// @param angle 回転角度
/// @details model_に新しくModelインスタンスが確保される
Mountain::Mountain(const std::string& filename, VECTOR init_pos, VECTOR scale, VECTOR angle)
	: Object3D(init_pos)
{
	model_ = new Model(filename, init_pos);
	model_->SetScale(scale);
	model_->SetRotation(angle);
}

/// @brief メモリリークを防ぐため
/// @details model_のメモリが解放される
Mountain::~Mountain()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
}

/// @brief 状態を最新に保つため
void Mountain::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

/// @brief 画面にモデルを可視化するため
/// @details 画面に描画される
void Mountain::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

/// @brief モデルの大きさを柔軟に変更するため
/// @param scale 新しい拡大率
/// @details スケール状態が更新される
void Mountain::SetScale(VECTOR scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

/// @brief モデルの向きを柔軟に変更するため
/// @param angle 新しい回転角度
/// @details 回転状態が更新される
void Mountain::SetRotation(VECTOR angle)
{
	if (model_ != nullptr)
	{
		model_->SetRotation(angle);
	}
}

/// @brief 時間帯などの環境変化に合わせて色を変えるため
/// @param r 赤
/// @param g 緑
/// @param b 青
/// @param a アルファ値
/// @details 色状態が更新される
void Mountain::SetColor(float r, float g, float b, float a)
{
	if (model_ != nullptr)
	{
		model_->SetColor(r, g, b, a);
	}
}
