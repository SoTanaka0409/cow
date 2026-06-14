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

	// フィーバー用金の牛で、フィーバーが終了したか生存時間を超えた場合は自動消滅させる
	if (mnFever == fever && (ServiceLocator::GetFever()->IsFever() == false || DeathCount >= DeathTimer))
	{
		Die(DEATH_LIMIT);
	}
}

void Cow_gold::Die(DeathReason reason)
{
	if (mDeleteFlag || mCowtDelete) return;
	CowMove::Die(reason);

	if (reason == DEATH_VACUUM || reason == DEATH_BAIT)
	{
		if (this->mnFever == Nofever)
		{
			ServiceLocator::GetFever()->StartFever();
		}
	}
}

