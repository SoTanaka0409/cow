#include "CharacterState.h"
#include "CharacterMove.h"
#include "DxLib.h"
#include "Master.h"
#include <cmath>

void StateIdle::Enter(CharacterMove* character)
{
	character->SetActionTimer(60 + GetRand(60));
}

void StateIdle::Update(CharacterMove* character)
{
	character->DecreaseActionTimer();

	// 重力処理：空中浮遊を防ぐため常に地面方向へ引き寄せる
	VECTOR pos = character->GetPosition();
	pos.y -= 4.0f;
	if (pos.y <= 0)
	{
		pos.y = 0;
	}
	character->SetPosition(pos);

	if (character->GetActionTimer() <= 0)
	{
		// 待機時間終了後、50%の確率で歩行状態へ移行し行動パターンを分散させる
		if (GetRand(100) < 50)
		{
			character->ChangeState(new StateWalk());
		}
		else
		{
			character->ChangeState(new StateIdle());
		}
	}
}

void StateWalk::Enter(CharacterMove* character)
{
	character->SetActionTimer(60 + GetRand(120));

	// 周囲を徘徊させるため、ランダムな方向へ向かう移動ベクトルを算出する
	float angle = GetRand(359) * DX_PI_F / 180.0f;
	VECTOR moveVec;
	moveVec.x = sinf(angle);
	moveVec.y = 0.0f;
	moveVec.z = cosf(angle);
	character->SetMoveVec(moveVec);
}

void StateWalk::Update(CharacterMove* character)
{
	character->DecreaseActionTimer();

	VECTOR pos = character->GetPosition();
	VECTOR moveVec = character->GetMoveVec();
	float speed = character->GetSpeed();

	pos = VAdd(pos, VScale(moveVec, speed * Master::GetDeltaTimeScaler()));

	// 重力処理による接地維持
	pos.y -= 4.0f;
	if (pos.y <= 0)
	{
		pos.y = 0;
	}
	character->SetPosition(pos);

	if (character->GetActionTimer() <= 0)
	{
		character->ChangeState(new StateIdle());
	}
}

void StateVacuum::Enter(CharacterMove* character)
{
}

void StateVacuum::Update(CharacterMove* character)
{
}
