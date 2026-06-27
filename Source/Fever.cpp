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
	, feverPlayer(nullptr)
{
	gauge = 0;
	timer = 0;
	isFever = false;
}

Fever::~Fever()
{
}

void Fever::AddGauge(int value)
{
}

void Fever::StartFever(Player3D* player)
{
	if (player == nullptr) return;
	feverPlayer = player;
	playerStatus = player->GetStatusAttack();
	player->SetStatusAttack(playerStatus * 2.0f);
	isFever = true;
	timer = 600; // 難易度調整のためフィーバー継続時間を10秒(600フレーム)に固定する
	DropCount = 0;
	dropTime = 60;
	Master::feverFlag = true;
}

void Fever::EndFever()
{
	if (feverPlayer != nullptr)
	{
		feverPlayer->SetStatusAttack(playerStatus);
		feverPlayer = nullptr;
	}
	isFever = false;
	
	// ステージ上の獲物が枯渇するのを防ぐため終了時に基本構成で再配置する
	VECTOR spawnPos = Utility::StageSize;
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::COW_GOLD.modelPath, spawnPos, 50.0f, CowMove::Cow_gold, 1);
	ServiceLocator::GetCowManager()->SpawnCow(GameConstants::COW_DEFAULT.modelPath, spawnPos, 50.0f, CowMove::Cow_1, 10);
	ServiceLocator::GetAnimalManager()->SpawnAnimal(GameConstants::ANIMAL_SHEEP.modelPath, spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	Master::feverFlag = false;
}

void Fever::Update()
{
	if (!isFever) return;
	
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);

	// フィーバー状態であることを視覚的に伝えるため画面全体に加算ブレンドでエフェクトを描画する
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
	timer--;
	
	// ボーナススコア獲得機会を提供するため一定間隔で金の牛を生成する
	if (DropCount > dropTime)
	{
		DropCount = 0;
		VECTOR spawnPos = Utility::StageSize;
		ServiceLocator::GetCowManager()->SpawnCow(GameConstants::COW_GOLD.modelPath, spawnPos, 50.0f, CowMove::Cow_gold, 2, true);
	}

	// フィーバー継続時間を超過したため状態を通常に戻す
	if (timer <= 0)
	{
		EndFever();
	}
}

bool Fever::IsFever()
{
	return isFever;
}
