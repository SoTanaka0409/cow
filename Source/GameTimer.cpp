#include"GameTimer.h"

GameTimer::GameTimer(VECTOR pos, int timer, Tag_Num num)
	: Time(timer)
	, mvPosition(pos)
	, flag(false)
	, stopFlag(false)
	, tag(num)
{
	lastTime = GetNowCount();

	scoreTextImage = LoadGraph("Resource/2D/rimit.png"); // 描画負荷軽減のため予め読み込む

	// アロケーション負荷を防ぐためコンストラクタ内で数字画像をキャッシュ
	numberImg[0] = LoadGraph("Resource/2D/コンボ数00.png");
	numberImg[1] = LoadGraph("Resource/2D/コンボ数01.png");
	numberImg[2] = LoadGraph("Resource/2D/コンボ数02.png");
	numberImg[3] = LoadGraph("Resource/2D/コンボ数03.png");
	numberImg[4] = LoadGraph("Resource/2D/コンボ数04.png");
	numberImg[5] = LoadGraph("Resource/2D/コンボ数05.png");
	numberImg[6] = LoadGraph("Resource/2D/コンボ数06.png");
	numberImg[7] = LoadGraph("Resource/2D/コンボ数07.png");
	numberImg[8] = LoadGraph("Resource/2D/コンボ数08.png");
	numberImg[9] = LoadGraph("Resource/2D/コンボ数09.png");
}

GameTimer::~GameTimer()
{
	// メモリリーク防止のため破棄時に全画像ハンドルを解放する
	DeleteGraph(scoreTextImage);
	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(numberImg[i]);
	}
}

void GameTimer::Draw()
{
	if (tag == Tag_NoGame) return;

	// 解像度変更を考慮し、基準座標(mvPosition)からの相対位置で描画
	DrawExtendGraph(
		static_cast<int>(mvPosition.x + 20),
		static_cast<int>(mvPosition.y),
		static_cast<int>(mvPosition.x + 220),
		static_cast<int>(mvPosition.y + 90),
		scoreTextImage,
		TRUE
	);

	int drawX = static_cast<int>(mvPosition.x + 220);
	int temp = Time;
	int digit[10];
	int digitCount = 0;

	// 1桁ずつ画像を描画するため、現在の時間を桁ごとの配列に分解
	if (temp == 0)
	{
		digit[digitCount++] = 0;
	}
	else
	{
		while (temp > 0)
		{
			digit[digitCount] = temp % 10;
			temp /= 10;
			digitCount++;
		}
	}

	// 分割した桁データを左側の桁から順に描画して数値を構成する
	for (int i = digitCount - 1; i >= 0; i--)
	{
		DrawExtendGraph(
			drawX,
			static_cast<int>(mvPosition.y),
			drawX + 80,
			static_cast<int>(mvPosition.y + 80),
			numberImg[digit[i]],
			TRUE
		);
		drawX += 80;
	}
}

void GameTimer::Update()
{
	if (stopFlag) return;

	int now = GetNowCount();

	// フレームレート非依存で時間を計測するためGetNowCountの差分を使用
	if (now - lastTime >= 1000)
	{
		lastTime = now;
		Time--;

		// 0未満の表示やマイナス値によるバグを防ぐため0で下限ストップ
		if (Time <= 0)
		{
			Time = 0;
			flag = true;
			stopFlag = true;
		}
	}
}

