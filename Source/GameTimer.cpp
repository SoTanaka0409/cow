#include"GameTimer.h"

GameTimer::GameTimer(VECTOR pos, int timer, Tag_Num num)
	: Time(timer)
	, mvPosition(pos)
	, mbFlag(false)
	, mbStopFlag(false)
	, mnTag(num)
{
	mLastTime = GetNowCount();

	scoreTextImage = LoadGraph("Resource/2D/rimit.png"); // 「LIMIT」表示用文字画像

	// 残り秒数を表示するためのデジタル数字グラフィックのロード
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
	// 読み込み済み画像リソースのメモリ解放
	DeleteGraph(scoreTextImage);
	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(numberImg[i]);
	}
}

void GameTimer::Draw()
{
	if (mnTag == Tag_NoGame) return;

	// 「LIMIT」テキストの描画
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

	// 残り時間を各桁に分割する
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

	// 各桁のデジタル数字画像を順番に描画する
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
	if (mbStopFlag) return;

	int now = GetNowCount();

	// 1秒(1000ミリ秒)ごとに残り時間を1減算する
	if (now - mLastTime >= 1000)
	{
		mLastTime = now;
		Time--;

		// タイムアップに達した時点でタイマー更新処理を自ら停止する
		if (Time <= 0)
		{
			Time = 0;
			mbFlag = true;
			mbStopFlag = true;
		}
	}
}

