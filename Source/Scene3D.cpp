#include "ServiceLocator.h"
#include"Scene3D.h"
#include"InputManager.h"
#include"ObjectManager.h"
#include"Player3D.h"
#include"GameTimer.h"
#include"GameManager.h"
#include"SkyBox.h"
#include"Thunder.h"
#include"CowMove.h"
#include"Floor.h"
#include"Wall.h"
#include"Object_Stage.h"
#include"Mountain.h"
#include"Tatumaki.h"
#include"Utility.h"
#include "GameConstants.h"

Thunder* thunder = nullptr;     // ???????o?p??A?N?e?B?u??I?u?W?F?N?g?|?C???^
Tatumaki* tatumaki = nullptr;   // ?X?e?[?W???????????I?u?W?F?N?g?|?C???^

Scene3D::Scene3D()
{
	mMassSpawnTimer = 0;
	mFontBackGraph = LoadGraph("Resource/2D/fontback.png");
}

Scene3D::~Scene3D()
{
	DeleteGraph(mFontBackGraph); // ?V?[????L??????\?[?X?????
	// ?? mpCowManager ?? thunder, tatumaki ???? Object3D ??p??????I?u?W?F?N?g??A
	// ???N???X Scene ??f?X?g???N?^?????? ObjectManager ??????????I?? delete ???????A
	// ???????蓮 delete ????d????i?N???b?V???????j??h??????s???????B
}

/*
 * @brief 3D?X?e?[?W?V?[??????????i?}?b?v?A?R???C?_?[?A?v???C???[?A?G?l?~?[?????j??s??
 * [????] ???
 * [?o??] ???
 * [????p] ?e?I?u?W?F?N?g??new?ABGM????J?n?A?R???C?_?[?o?^
 */
void Scene3D::Initialize()
{
	Master::mnCaughtCowCount = 0;
	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME);
	Master::mpSoundManager->SetBGMVolume(120);
	
	// ?}?b?v???E??????????????????????A?Z?b?g???????
	for (int i = 0; i < 6; i++)
	{
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/SmallTree1.mv1", VGet(6000, 0, -5000 + 2000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/SmallTree1.mv1", VGet(-6000, 0, -5000 + 2000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/Flower1.mv1", VGet(5500, 0, -5000 + 2000 * i), 3.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/SmallTree1.mv1", VGet(-6000, 0, -5000 + 2000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/SmallTree1.mv1", VGet(-5000 + 2000 * i, 0, -6000), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/SmallTree1.mv1", VGet(-5000 + 2000 * i, 0, 6000), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}

	for (int i = 0; i < 3; i++)
	{
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/BigTree1.mv1", VGet(5500, 0, -4000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/BigTree4.mv1", VGet(-5500, 0, -4000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/BigTree4.mv1", VGet(-4000 + 4000 * i, 0, 5500), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/BigTree3.mv1", VGet(-4000 + 4000 * i, 0, -5500), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}

	for (int i = 0; i < 2; i++)
	{
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/BigTree3.mv1", VGet(5500, 0, -2000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse (2)/Sousyoku/BigTree2.mv1", VGet(-5500, 0, -2000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}
	 
	new Object_Stage("Resource/3D/fanse (2)/Sousyoku/Grass2.mv1", VGet(0, 0, 0), 2.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));

	// ?X?e?[?W??L?????\????????A???E????l????R?iMountain?j??z?u
	VECTOR mountainScale = VGet(30.0f, 50.0f, 30.0f);
	float mountainDist = 13000.0f;
	auto m1 = new Mountain("Resource/3D/Mountain/uploads_files_2708212_terrain.mv1", VGet(0, 0, mountainDist), mountainScale, VGet(0.0f, DX_PI_F, 0.0f));
	m1->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m2 = new Mountain("Resource/3D/Mountain/uploads_files_2708212_terrain.mv1", VGet(0, 0, -mountainDist), mountainScale, VGet(0.0f, 0.0f, 0.0f));
	m2->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m3 = new Mountain("Resource/3D/Mountain/uploads_files_2708212_terrain.mv1", VGet(mountainDist, 0, 0), mountainScale, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	m3->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m4 = new Mountain("Resource/3D/Mountain/uploads_files_2708212_terrain.mv1", VGet(-mountainDist, 0, 0), mountainScale, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
	m4->SetColor(0.2f, 0.3f, 0.2f, 1.0f);

	// プレイエリア外に出られないよう岩(Rock)を配置
	for (int i = 0; i < 40; i++)
	{
		float rockX = (float)(GetRand(15000) - 7500);
		float rockZ = (float)(GetRand(15000) - 7500);
		
		if (rockX > -4500 && rockX < 4500 && rockZ > -4500 && rockZ < 4500) {
			continue;
		}

		float rockScale = 1.0f + (float)(GetRand(30)) / 10.0f;
		float rockRot = (float)(GetRand(360)) * DX_PI_F / 180.0f;
		
		auto rock = new Object_Stage("Resource/3D/Storn/rock.mv1", VGet(rockX, 0, rockZ), rockScale, VGet(0.0f, rockRot, 0.0f));
		rock->SetColor(0.4f, 0.7f, 0.3f, 1.0f);
	}

	// プレイエリア境界にフェンス(Fence)を配置
	for (int i = 0; i < 5; i++)
	{
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(490.0f + 1000 * i, 0.0f, 5050.0f), 12.80f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(-490.0f + -1000 * i, 0.0f, 5050.0f), 12.80f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(490.0f + 1000 * i, 0.0f, -5050.0f), 12.80f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(-490.0f + -1000 * i, 0.0f, -5050.0f), 12.80f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(5050.0f , 0.0f, 490.0f + 1000* i), 12.80f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(5050.0f, 0.0f, -490.0f + -1000 * i), 12.80f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(-5050.0f, 0.0f, 490.0f + 1000 * i), 12.80f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/NewFence/fence1.mv1", VGet(-5050.0f, 0.0f, -490.0f + -1000 * i), 12.80f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	}

	thunder = new Thunder(VGet(0.0f, 0.0f, 0.0f));
	tatumaki = new Tatumaki(VGet(3000.0f, 0.0f, 3000.0f));

	auto Player = new Player3D("Resource/3D/ufo2/uploads_files_2595751_UFO.mv1", VGet(1000.0f, 2000.0f, 0.0f));
	Player->SetScale(0.6f);

	VECTOR spawnPos = Utility::StageSize; // スポーン初期位置

	// 初期ステージの生物を配置
	mpCowManager->SpawnCow(GameConstants::COW_DEFAULT.modelPath, spawnPos, 50.0f, CowMove::Cow_1, 10);
	mpCowManager->SpawnCow(GameConstants::COW_GOLD.modelPath, spawnPos, 50.0f, CowMove::Cow_gold, 2);
	mpAnimalManager->SpawnAnimal(GameConstants::ANIMAL_CHICKEN.modelPath, spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	mpAnimalManager->SpawnAnimal(GameConstants::ANIMAL_BEAR.modelPath, spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	
	mpPhase = Normal;

	auto skybox = new SkyBox("Resource/3D/SkyBox/SkyBox.mv1", VGet(0, 0, 0));
	skybox->SetScale(30.0f);
	skybox->SetModelTexture("Resource/3D/SkyBox/sky000.jpg", 0);

	new Floor(
		"Resource/2D/haikei.png",
		VGet(0, 0, 0),
		VGet(-11500, 0, -11500),
		VGet(11500, 0, 11500)
	);
	
	// マップ外壁の当たり判定用コライダー設定
	new Wall("Resource", VGet(0.0f, 0.0f, 5000.0f), VGet(-5000.0f, 5000.0f, 0.0f), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("Resource", VGet(0.0f, 0.0f, -5000.0f), VGet(-5000.0f, 5000.0f, 0.0), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("Resource", VGet(5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
	new Wall("Resource", VGet(-5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
}

void Scene3D::Update()
{
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	Scene::Update();

	mpCowManager->Update();
	mpGameManager->Update();
	PhaseUpdate();
	tatumaki->Update();

	// 制限時間終了でリザルト画面へのフェード開始
	if (ServiceLocator::GetGameManager()->GetGameTimer()->GetTime() <= 0)
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_RESULT;
	}

	if (mFadeState == SceneFade_Out)
	{
		// フェードアウトに合わせてBGMをフェードアウト
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)mNextScene);
		}
	}
}

void Scene3D::Draw()
{
	Scene::Draw();
	const int count = 51;
	const float distance = -500.0f;
	
	// デバッグ用のグリッド描画
	for (int i = 0; i < count; i++)
	{
		float base = (count / 2 - i) * -distance;

		DrawLine3D(
			VGet(-distance * (count / 2), 0.0f, base),
			VGet(distance * (count / 2), 0.0f, base),
			GetColor(255, 255, 255)
		);

		DrawLine3D(
			VGet(base, 0.0f, -distance * (count / 2)),
			VGet(base, 0.0f, distance * (count / 2)),
			GetColor(255, 255, 255)
		);
	}
	
	mpCowManager->Draw();
  
	if (mpGameManager->GetGameTimer() && !(mpGameManager->GetGameTimer()->OutTimerFlag()))
	{
		mpGameManager->GetGameTimer()->Draw();
	}

	// イベントフェーズに応じた警告テキスト表示
	if (Master::mpCamera->GetIsPhaseCameraActive())
	{
		int currentPhase = (int)mpGameManager->GetCurrentPhase();
		
		if (currentPhase == (int)GameManager::GamePhase::MassSpawn || currentPhase == (int)GameManager::GamePhase::TornadoCrisis)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawExtendGraph(0, 0, 1920, 1080, mFontBackGraph, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		if (currentPhase == (int)GameManager::GamePhase::MassSpawn)
		{
			SetFontSize(64);
			DrawFormatString(600, 200, GetColor(255, 100, 100), "???????????I?I");
			SetFontSize(16);
		}
		else if (currentPhase == (int)GameManager::GamePhase::TornadoCrisis)
		{
			SetFontSize(64);
			DrawFormatString(600, 200, GetColor(255, 100, 100), "???????????I?I");
			SetFontSize(16);
		}
	}

	if (mFadeState != SceneFade_None)
	{
		Scene::Fade(mFadeState);
	}
}

/*
 * @brief フェーズ進行制御
 * [入力] なし [出力] なし [副作用] カメラ・竜巻状態変更、大量スポーン処理
 */
void Scene3D::PhaseUpdate()
{
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	if (player != nullptr && tatumaki != nullptr) {
		int currentPhase = (int)mpGameManager->GetCurrentPhase();
		Master::mpCamera->UpdateCameraByPhase(currentPhase, player->GetPosition(), tatumaki->GetPosition());

		tatumaki->SetCrisisMode(currentPhase == (int)GameManager::GamePhase::TornadoCrisis);

		// 大量出現フェーズ時、60フレーム間隔で牛をスポーン
		if (currentPhase == (int)GameManager::GamePhase::MassSpawn)
		{
			mMassSpawnTimer++;
			if (mMassSpawnTimer >= 60)
			{
				mMassSpawnTimer = 0;

				VECTOR spawnCenter = player->GetPosition();
				// スポーン位置をプレイヤー上空(y+2000)に設定
				VECTOR spawnArgs = VGet(4000.0f, spawnCenter.y + 2000.0f, 4000.0f);

				mpCowManager->SpawnCow(GameConstants::COW_DEFAULT.modelPath, spawnArgs, 50.0f, CowMove::Cow_1, 2);
			}
		}
	}
}

void Scene3D::Finalize()
{
	Master::mpSoundManager->StopBGM(); // シーン終了時にBGMを停止
}
