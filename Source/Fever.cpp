#include "Fever.h"
#include "Cow_gold.h"
#include "CowManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "Cow.h"
#include "Utility.h"

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
	// プレイヤーの吸引能力を2倍にしてフィーバーの恩恵を与える
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	playerStatus = player->GetStatusAttack();
	player->SetStatusAttack(playerStatus * 2.0f);
	mIsFever = true;
	mTimer = 600; // フィーバー時間を10秒（600フレーム）に設定
	DropCount = 0;
	dropTime = 60;
	Master::FeverFlag = true;
}

void Fever::EndFever()
{
	// プレイヤーの吸引能力を元の値に戻す
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	player->SetStatusAttack(playerStatus);
	mIsFever = false;
	
	// フィーバー終了時の救済・盛り上げとしてステージ全体に牛と羊を再配備する
	VECTOR spawnPos = Utility::StageSize;
	Master::mpSceneManager->GetCurrentScene()->mpCowManager->SpawnCow("Resource/3D/GOLDCow/GoldCow.mv1", spawnPos, 50.0f, CowMove::Cow_gold, 1);
	Master::mpSceneManager->GetCurrentScene()->mpCowManager->SpawnCow("Resource/3D/Cow/uploads_files_3880923_Cow.mv1", spawnPos, 50.0f, CowMove::Cow_1, 10);
	Master::mpSceneManager->GetCurrentScene()->mpAnimalManager->SpawnAnimal("Resource/3D/Animal/uploads_files_3880923_Sheep.mv1", spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	Master::FeverFlag = false;
}

void Fever::Update()
{
	if (!mIsFever) return;
	
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);

	// フィーバー中であることを視覚的に強調するため、画面の境界に黄色の枠線を描画する
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
	
	// フィーバー期間中、一定周期（約1秒ごと）で金の牛を生成してプレイヤーの獲得チャンスを増やす
	if (DropCount > dropTime)
	{
		DropCount = 0;
		VECTOR spawnPos = Utility::StageSize;
		Master::mpSceneManager->GetCurrentScene()->mpCowManager->SpawnCow("Resource/3D/GOLDCow/GoldCow.mv1", spawnPos, 50.0f, CowMove::Cow_gold, 2, true);
	}

	// 規定フレーム数が経過したらフィーバー状態を終了する
	if (mTimer <= 0)
	{
		EndFever();
	}
}

bool Fever::IsFever()
{
	return mIsFever;
}
