#include "ServiceLocator.h"
#include "Cow_gold.h"
#include "CapsuleCollider.h"
#include "Player3D.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Bait.h"
#include "Wall.h"

Cow_gold::Cow_gold(std::string filename, VECTOR initPos, Tag_fever fever)
	: CowMove(filename, initPos)
	, mnFever(fever)
	, DeathCount(0)
	, DeathTimer(1200)
{
	SetTag_cow(CowMove::Cow_gold);
	mfXp = 20;
	mfScore = 30;
	// プレイヤーが捕獲しやすいように当たり判定を大きめに設定
	mColliderRadius = 150.0f;
}

Cow_gold::~Cow_gold()
{
}

void Cow_gold::Reset(VECTOR pos)
{
	CowMove::Reset(pos);
	DeathCount = 0;
}

void Cow_gold::Update()
{
	DeathCount++;
	CowMove::Update();

	// フィーバー終了時や寿命超過で画面内に残り続けるのを防ぐため消滅させる
	auto fv = ServiceLocator::GetFever();
	if (mnFever == fever && (fv == nullptr || fv->IsFever() == false || DeathCount >= DeathTimer))
	{
		Die(DEATH_LIMIT);
	}
}

void Cow_gold::Die(DeathReason reason)
{
	// 二重解放や不整合を防ぐため、既に消滅処理中なら弾く
	if (mDeleteFlag) return;
	CowMove::Die(reason);

	// プレイヤーの直接的アクション（吸引・エサ）で捕獲された場合のみ発動させる
	if (reason == DEATH_VACUUM || reason == DEATH_BAIT)
	{
		if (this->mnFever == Nofever)
		{
			if (auto fv = ServiceLocator::GetFever())
			{
				fv->StartFever(mpTargetPlayer);
			}
		}
	}
}
