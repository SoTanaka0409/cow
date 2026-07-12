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

	// 驥榊鴨蜃ｦ逅・ｼ夂ｩｺ荳ｭ豬ｮ驕翫ｒ髦ｲ縺舌◆繧∝ｸｸ縺ｫ蝨ｰ髱｢譁ｹ蜷代∈蠑輔″蟇・○繧・
	VECTOR pos = character->GetPosition();
	pos.y -= 4.0f;
	if (pos.y <= 0)
	{
		pos.y = 0;
	}
	character->SetPosition(pos);

	if (character->GetActionTimer() <= 0)
	{
		// 蠕・ｩ滓凾髢鍋ｵゆｺ・ｾ後・0%縺ｮ遒ｺ邇・〒豁ｩ陦檎憾諷九∈遘ｻ陦後＠陦悟虚繝代ち繝ｼ繝ｳ繧貞・謨｣縺輔○繧・
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

	// 蜻ｨ蝗ｲ繧貞ｾ伜ｾ翫＆縺帙ｋ縺溘ａ縲√Λ繝ｳ繝繝縺ｪ譁ｹ蜷代∈蜷代°縺・ｧｻ蜍輔・繧ｯ繝医Ν繧堤ｮ怜・縺吶ｋ
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

	// 驥榊鴨蜃ｦ逅・↓繧医ｋ謗･蝨ｰ邯ｭ謖・
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
