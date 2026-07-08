#include "Combo.h"
#include "DxLib.h"
#include "InputManager.h"
#include "Master.h"
#include "Utility.h"
#include <string>

Combo::Combo()
{
	comboCount = 0;
	comboTimer = 0.0f;
	comboMaxTime = 3.0f; // 難易度調整のためコンボ猶予は3秒に固定

	comboImage = Master::mpResourceManager->LoadGraphics("Resource/2D/COMBO.png");

	comboShow = false;
	comboShowTimer = 0.0f;
}

Combo::~Combo()
{
}

void Combo::Draw()
{
	// 0コンボ時は表示を省略しUIの煩雑化を防ぐ
	if (comboCount >= 1)
	{
		int x = Utility::UI_BASE_X;
		int y = Utility::UI_COMBO_Y;
		int width = Utility::UI_PANEL_W;
		int height = Utility::UI_PANEL_H;

		DrawExtendGraph(x, y, x + width, y + height, comboImage, TRUE);

		if (Master::mpScore)
		{
			Master::mpScore->DrawNumber(
				Utility::UI_DIGIT_X,
				y,
				comboCount,
				1.25f,
				1
			);
		}
	}
}

void Combo::Update()
{
	// TODO: リリース時に削除（デバッグ用加算ショートカット）
	if (InputManager::CheckDownKey(KEY_INPUT_R))
	{
		AddHit();
	}

	if (comboCount > 0)
	{
		comboTimer -= 0.01f;

		// コンボ猶予時間を超過したため状態を破棄する
		if (comboTimer <= 0.0f)
		{
			Reset();
		}
	}
}

void Combo::AddHit()
{
	comboCount++;
	comboTimer = comboMaxTime;

	if (comboCount > 1000)
	{
		comboCount = 1000; // 描画領域のオーバーフローを防ぐための上限値
	}
}

void Combo::Reset()
{
	comboCount = 0;
	comboTimer = 3.0f;
}

int Combo::GetCombo() const
{
	return comboCount;
}

float Combo::GetMultiplier() const
{
	if (comboCount >= 1)
	{
		return (float)comboCount;
	}
	return 1.0f;
}

