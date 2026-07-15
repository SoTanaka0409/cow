#include"GameTimer.h"
#include"Master.h"
#include"Utility.h"

GameTimer::GameTimer(VECTOR pos, int timer, Tag_Num num)
	: Time(timer)
	, position_(pos)
	, mbFlag(false)
	, mbStopFlag(false)
	, tag_(num)
{
	mLastTime = GetNowCount();

	score_text_image_ = Master::mpResourceManager->LoadGraphics("Resource/2D/TimeLimitText.png"); // 描画遅延軽減のため先読み込み
}

GameTimer::~GameTimer()
{
}

void GameTimer::Draw()
{
	if (tag_ == Tag_NoGame) return;

	// 解像度変更を想定し、基準座標(position_)からの相対位置で描画
	DrawExtendGraph(
		static_cast<int>(position_.x + Utility::kUiBaseX),
		static_cast<int>(position_.y + Utility::kUiTimerY),
		static_cast<int>(position_.x + Utility::kUiBaseX + Utility::kUiPanelW),
		static_cast<int>(position_.y + Utility::kUiTimerY + Utility::kUiPanelH),
		score_text_image_,
		TRUE
	);

	if (Master::mpScore)
	{
		Master::mpScore->DrawNumber(
			static_cast<int>(position_.x + Utility::kUiDigitX),
			static_cast<int>(position_.y + Utility::kUiTimerY),
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

		// 0未満の表示やマイナス値によるバグを防ぐため下限をストップ
		if (Time <= 0)
		{
			Time = 0;
			mbFlag = true;
			mbStopFlag = true;
		}
	}
}