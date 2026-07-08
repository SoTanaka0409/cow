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
	comboMaxTime = 3.0f; // 難易度調整のためコンボ継続時間は3秒に固定

	comboImage = Master::mpResourceManager->LoadGraphics("Resource/2D/COMBO.png");

	comboShow = false;
	comboShowTimer = 0.0f;
}

Combo::~Combo()
{
}

void Combo::Draw()
{
	// 0コンボ時は表示を省略しUIの描画負荷を抑える
	if (comboCount >= 1)
	{
		int x = Utility::kUiBaseX;
		int y = Utility::kUiComboY;
		int width = Utility::kUiPanelW;
		int height = Utility::kUiPanelH;

		DrawExtendGraph(x, y, x + width, y + height, comboImage, TRUE);

		if (Master::mpScore)
		{
			Master::mpScore->DrawNumber(
				Utility::kUiDigitX,
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
	// TODO: リリース時に削除（デバッグ用追加ショートカット）
	if (InputManager::CheckDownKey(KEY_INPUT_R))
	{
		AddHit();
	}

	if (comboCount > 0)
	{
		comboTimer -= 0.01f;

		// コンボ継続時間を使い切ったため状態をリセットする
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
		comboCount = 1000; // 描画桁数のオーバーフローを防ぐための上限値
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
