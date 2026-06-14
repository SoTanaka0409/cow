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

	VECTOR pos = character->GetPosition();
	pos.y -= 4.0f;
	if (pos.y <= 0)
	{
		pos.y = 0;
	}
	character->SetPosition(pos);

	if (character->GetActionTimer() <= 0)
	{
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
