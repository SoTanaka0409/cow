#include"GameTimer.h"
#include"Master.h"
#include"Utility.h"

GameTimer::GameTimer(VECTOR pos, int timer, Tag_Num num)
	: Time(timer)
	, mvPosition(pos)
	, mbFlag(false)
	, mbStopFlag(false)
	, mnTag(num)
{
	mLastTime = GetNowCount();

	scoreTextImage = Master::mpResourceManager->LoadGraphics("Resource/2D/rimit.png"); // 描画負荷軽減のため予め読み込む
}

GameTimer::~GameTimer()
{
}

void GameTimer::Draw()
{
	if (mnTag == Tag_NoGame) return;

	// 解像度変更を考慮し、基準座標(mvPosition)からの相対位置で描画
	DrawExtendGraph(
		static_cast<int>(mvPosition.x + Utility::UI_BASE_X),
		static_cast<int>(mvPosition.y),
		static_cast<int>(mvPosition.x + Utility::UI_BASE_X + 200),
		static_cast<int>(mvPosition.y + 90),
		scoreTextImage,
		TRUE
	);

	if (Master::mpScore)
	{
		Master::mpScore->DrawNumber(
			static_cast<int>(mvPosition.x + Utility::UI_DIGIT_X),
			static_cast<int>(mvPosition.y),
			Time,
			1.0f,
			1
		);
	}
}

void GameTimer::Update()
{
	if (mbStopFlag) return;

	int now = GetNowCount();

	// フレームレート非依存で時間を計測するためGetNowCountの差分を使用
	if (now - mLastTime >= 1000)
	{
		mLastTime = now;
		Time--;

		// 0未満の表示やマイナス値によるバグを防ぐため0で下限ストップ
		if (Time <= 0)
		{
			Time = 0;
			mbFlag = true;
			mbStopFlag = true;
		}
	}
}

