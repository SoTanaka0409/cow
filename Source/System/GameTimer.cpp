#include"GameTimer.h"
#include"Master.h"
#include"Utility.h"

GameTimer::GameTimer(VECTOR pos, int timer, Tag_Num num)
	: Time(timer)
	, position_(pos)
	, flag_(false)
	, stop_flag_(false)
	, tag_(num)
{
	mLastTime = GetNowCount();

	score_text_image_ = Master::resource_manager_->LoadGraphics("Resource/2D/ゲーム画面/制限時間文字.png"); // 描画遅延軽減のため先読み込み
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

	if (Master::score_manager_)
	{
		Master::score_manager_->DrawNumber(
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
	if (stop_flag_) return;

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
			flag_ = true;
			stop_flag_ = true;
		}
	}
}
