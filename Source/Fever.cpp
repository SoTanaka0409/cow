#include "ServiceLocator.h"
#include "Fever.h"
#include "Cow_gold.h"
#include "CowManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "Cow.h"
#include "Utility.h"
#include "GameConstants.h"

Fever::Fever()
	: dropTime(0)
	, DropCount(0)
{
	mGauge = 0;
	mTimer = 0;
	mIsFever = false;
}

Fever::~Fever()
{
}

void Fever::AddGauge(int value)
{
}

void Fever::StartFever()
{
	// ?v???C???[??z???\???2?{?????t?B?[?o?[????b??^????
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	playerStatus = player->GetStatusAttack();
	player->SetStatusAttack(playerStatus * 2.0f);
	mIsFever = true;
	mTimer = 600; // ?t?B?[?o?[?????10?b?i600?t???[???j????
	DropCount = 0;
	dropTime = 60;
	Master::FeverFlag = true;
}

void Fever::EndFever()
{
	// ?v???C???[??z???\??????l????
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	player->SetStatusAttack(playerStatus);
	mIsFever = false;
	
	// ?t?B?[?o?[?I??????~?ƒÏE????ƒO?????X?e?[?W?S??????r???z??????
	VECTOR spawnPos = Utility::StageSize;
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::COW_GOLD.modelPath, spawnPos, 50.0f, CowMove::Cow_gold, 1);
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::COW_DEFAULT.modelPath, spawnPos, 50.0f, CowMove::Cow_1, 10);
	ServiceLocator::GetAnimalManager()->SpawnAnimal(GameConstants::ANIMAL_SHEEP.modelPath, spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	Master::FeverFlag = false;
}

void Fever::Update()
{
	if (!mIsFever) return;
	
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);

	// ?t?B?[?o?[??????ç·?????o?I???????????A??????E????F??g????`????
	DrawBox(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(255, 200, 50), FALSE);
	DrawBox(1, 1, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(255, 220, 100), FALSE);
	DrawBox(2, 2, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, GetColor(255, 255, 180), FALSE);

	for (int i = 0; i < 30; i++)
	{
		DrawBox(
			i,
			i,
			Utility::SCREEN_WIDTH - i,
			Utility::SCREEN_HEIGHT - i,
			GetColor(255, 200, 50),
			FALSE
		);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DropCount++;
	mTimer--;
	
	// ?t?B?[?o?[??????A???????i??1?b????j?????????????v???C???[??l???`?????X????
	if (DropCount > dropTime)
	{
		DropCount = 0;
		VECTOR spawnPos = Utility::StageSize;
		ServiceLocator::GetCowManager()->SpawnCow(GameConstants::COW_GOLD.modelPath, spawnPos, 50.0f, CowMove::Cow_gold, 2, true);
	}

	// ?K??t???[???????o???????t?B?[?o?[????I??????
	if (mTimer <= 0)
	{
		EndFever();
	}
}

bool Fever::IsFever()
{
	return mIsFever;
}
