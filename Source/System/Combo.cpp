#include "Combo.h"
#include "DxLib.h"
#include "Master.h"
#include "Utility.h"
#include <string>

Combo::Combo()
{
	combo_count_ = 0;
	combo_timer_ = 0.0f;
	combo_max_time_ = 3.0f; // 難易度調整のためコンボ継続時間は3秒に固定

	combo_image_ = Master::resource_manager_->LoadGraphics("Resource/2D/Combo/ComboText.png");

	combo_show_ = false;
	combo_show_timer_ = 0.0f;
}

Combo::~Combo()
{
}

void Combo::Draw()
{
	// 0コンボ時は表示を省略しUIの描画負荷を抑える
	if (combo_count_ >= 1)
	{
		int x = Utility::kUiBaseX;
		int y = Utility::kUiComboY;
		int width = Utility::kUiPanelW;
		int height = Utility::kUiPanelH;

		DrawExtendGraph(x, y, x + width, y + height, combo_image_, TRUE);

		if (Master::score_manager_)
		{
			Master::score_manager_->DrawNumber(
				Utility::kUiDigitX,
				y,
				combo_count_,
				1.25f,
				1
			);
		}
	}
}

void Combo::Update()
{
	if (combo_count_ > 0)
	{
		combo_timer_ -= 0.01f;

		auto currentScene = Master::scene_manager_->GetSceneType();
		// コンボ継続時間を使い切ったため状態をリセットする
		if (combo_timer_ <= 0.0f && currentScene != SceneManager::kSceneTutorial)
		{
			Reset();
		}
	}
}

void Combo::AddHit()
{
	combo_count_++;
	combo_timer_ = combo_max_time_;

	if (combo_count_ > 1000)
	{
		combo_count_ = 1000; // 描画桁数のオーバーフローを防ぐための上限値
	}
}

void Combo::Reset()
{
	combo_count_ = 0;
	combo_timer_ = 3.0f;
}

int Combo::GetCombo() const
{
	return combo_count_;
}

float Combo::GetMultiplier() const
{
	if (combo_count_ >= 1)
	{
		return (float)combo_count_;
	}
	return 1.0f;
}
