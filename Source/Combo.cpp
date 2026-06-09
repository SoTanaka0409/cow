#include "Combo.h"
#include "DxLib.h"
#include "InputManager.h"
#include <string>

Combo::Combo()
{
	comboCount = 0;
	comboTimer = 0.0f;
	comboMaxTime = 3.0f; // 3秒以内に次の牛を獲得すればコンボ継続

	comboImage = LoadGraph("Resource/2D/COMBO.png");

	comboShow = false;
	comboShowTimer = 0.0f;

	numberImage[0] = LoadGraph("Resource/2D/コンボ数00.png");
	numberImage[1] = LoadGraph("Resource/2D/コンボ数01.png");
	numberImage[2] = LoadGraph("Resource/2D/コンボ数02.png");
	numberImage[3] = LoadGraph("Resource/2D/コンボ数03.png");
	numberImage[4] = LoadGraph("Resource/2D/コンボ数04.png");
	numberImage[5] = LoadGraph("Resource/2D/コンボ数05.png");
	numberImage[6] = LoadGraph("Resource/2D/コンボ数06.png");
	numberImage[7] = LoadGraph("Resource/2D/コンボ数07.png");
	numberImage[8] = LoadGraph("Resource/2D/コンボ数08.png");
	numberImage[9] = LoadGraph("Resource/2D/コンボ数09.png");
}

Combo::~Combo()
{
	DeleteGraph(comboImage);

	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(numberImage[i]);
	}
}

void Combo::Draw()
{
	// 1コンボ以上からコンボ表示を開始する
	if (comboCount >= 1)
	{
		int x = 20;
		int y = 200;
		int width = 200;
		int height = 100;

		DrawExtendGraph(x, y, x + width, y + height, comboImage, TRUE);

		std::string comboStr = std::to_string(comboCount);
		int nmX = 220;
		int nmY = 200;
		int nmWidth = 100;
		int nmHeight = 100;

		for (int i = 0; i < comboStr.size(); i++)
		{
			int digit = comboStr[i] - '0';
			DrawExtendGraph(
				nmX + i * 80,
				nmY,
				nmX + i * 80 + nmWidth,
				nmY + nmHeight,
				numberImage[digit],
				TRUE
			);
		}
	}
}

void Combo::Update()
{
	// デバッグ用キー操作
	if (InputManager::CheckDownKey(KEY_INPUT_R))
	{
		AddHit();
	}

	if (comboCount > 0)
	{
		comboTimer -= 0.01f;

		// 制限時間切れによるリセット
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
		comboCount = 1000; // カウンターの最大値制限
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
