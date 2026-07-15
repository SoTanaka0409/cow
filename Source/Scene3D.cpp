#include "ServiceLocator.h"
#include "Scene3D.h"
#include "Master.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "GameTimer.h"
#include "GameManager.h"
#include "SkyBox.h"
#include "Thunder.h"
#include "CowMove.h"
#include "Floor.h"
#include "Wall.h"
#include "Object_Stage.h"
#include "Mountain.h"
#include "Tornado.h"
#include "Utility.h"
#include "GameConstants.h"

Thunder* thunder_ = nullptr;
Tornado* tatumaki = nullptr;

/*
 * 3Dシーンの初期化
 * [入力] なし
 * [出力] なし
 * [副作用] メンバ変数の初期化
 */
Scene3D::Scene3D()
{
	mass_spawn_timer_ = 0;
	font_back_graph_ = Master::mpResourceManager->LoadGraphics("Resource/2D/fontback.png");
}

/*
 * 3Dシーンの破棄
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Scene3D::~Scene3D()
{
}

/*
 * 3Dシーンのセットアップ
 * [入力] なし
 * [出力] なし
 * [副作用] 3Dオブジェクトの配置、BGM再生の開始
 */
void Scene3D::Initialize()
{
	Master::camera_->Initialize();

	Master::mnCaughtCowCount = 0;
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);
	Master::mpSoundManager->SetBGMVolume(120);
	
	for (int i = 0; i < 6; i++)
	{
		new Object_Stage("Resource/3D/装飾/SmallTree1.mv1", VGet(6000.0f, 0.0f, -5000.0f + 2000.0f * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/SmallTree1.mv1", VGet(-6000.0f, 0.0f, -5000.0f + 2000.0f * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/Flower1.mv1", VGet(5500.0f, 0.0f, -5000.0f + 2000.0f * i), 3.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/SmallTree1.mv1", VGet(-6000.0f, 0.0f, -5000.0f + 2000.0f * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/SmallTree1.mv1", VGet(-5000.0f + 2000.0f * i, 0, -6000), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/SmallTree1.mv1", VGet(-5000.0f + 2000.0f * i, 0, 6000), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}

	for (int i = 0; i < 3; i++)
	{
		new Object_Stage("Resource/3D/装飾/BigTree1.mv1", VGet(5500.0f, 0.0f, -4000.0f + 4000.0f * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/BigTree4.mv1", VGet(-5500.0f, 0.0f, -4000.0f + 4000.0f * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/BigTree4.mv1", VGet(-4000.0f + 4000.0f * i, 0, 5500), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/BigTree3.mv1", VGet(-4000.0f + 4000.0f * i, 0, -5500), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}

	for (int i = 0; i < 2; i++)
	{
		new Object_Stage("Resource/3D/装飾/BigTree3.mv1", VGet(5500.0f, 0.0f, -2000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/装飾/BigTree2.mv1", VGet(-5500.0f, 0.0f, -2000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}
	 
	new Object_Stage("Resource/3D/装飾/Grass2.mv1", VGet(0, 0, 0), 2.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));

	VECTOR mountainScale = VGet(30.0f, 50.0f, 30.0f);
	float mountainDist = 13000.0f;
	auto m1 = new Mountain("Resource/3D/山/地形.mv1", VGet(0, 0, mountainDist), mountainScale, VGet(0.0f, DX_PI_F, 0.0f));
	m1->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m2 = new Mountain("Resource/3D/山/地形.mv1", VGet(0, 0, -mountainDist), mountainScale, VGet(0.0f, 0.0f, 0.0f));
	m2->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m3 = new Mountain("Resource/3D/山/地形.mv1", VGet(mountainDist, 0, 0), mountainScale, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	m3->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m4 = new Mountain("Resource/3D/山/地形.mv1", VGet(-mountainDist, 0, 0), mountainScale, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
	m4->SetColor(0.2f, 0.3f, 0.2f, 1.0f);

	// プレイヤーがマップ外へ出られないよう、境界に岩を配置
	for (int i = 0; i < 40; i++)
	{
		float rockX = (float)(GetRand(15000) - 7500);
		float rockZ = (float)(GetRand(15000) - 7500);
		
		if (rockX > -4500 && rockX < 4500 && rockZ > -4500 && rockZ < 4500) {
			continue;
		}

		float rockScale = 1.0f + (float)(GetRand(30)) / 10.0f;
		float rockRot = (float)(GetRand(360)) * DX_PI_F / 180.0f;
		
		auto rock = new Object_Stage("Resource/3D/石/rock.mv1", VGet(rockX, 0, rockZ), rockScale, VGet(0.0f, rockRot, 0.0f));
		rock->SetColor(0.4f, 0.7f, 0.3f, 1.0f);
	}

	for (int i = 0; i < 5; i++)
	{
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(490.0f + 1000 * i, 0.0f, 5050.0f), 12.80f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(-490.0f + -1000 * i, 0.0f, 5050.0f), 12.80f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(490.0f + 1000 * i, 0.0f, -5050.0f), 12.80f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(-490.0f + -1000 * i, 0.0f, -5050.0f), 12.80f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(5050.0f , 0.0f, 490.0f + 1000* i), 12.80f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(5050.0f, 0.0f, -490.0f + -1000 * i), 12.80f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(-5050.0f, 0.0f, 490.0f + 1000 * i), 12.80f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/新しい柵/fence1.mv1", VGet(-5050.0f, 0.0f, -490.0f + -1000 * i), 12.80f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	}

	thunder_ = new Thunder(VGet(0.0f, 0.0f, 0.0f));
	tatumaki = new Tornado(VGet(3000.0f, 0.0f, 3000.0f));

	auto Player = new Player3D("Resource/3D/ufo2/Ufo.mv1", VGet(1000.0f, 2000.0f, 0.0f));
	Player->SetScale(0.6f);

	VECTOR spawnPos = VGet(0.0f, 0.0f, 0.0f);
	float scatterArea = Utility::StageSize.x;

	cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCow1, 20, false, scatterArea);
	cow_manager_->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2, false, 500.0f);
	animal_manager_->SpawnAnimal(GameConstants::kAnimalChicken.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5, scatterArea);
	animal_manager_->SpawnAnimal(GameConstants::kAnimalBear.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5, scatterArea);
	
	phase_ = kNormal;

	auto skybox = new SkyBox("Resource/3D/SkyBox/SkyBox.mv1", VGet(0, 0, 0));
	skybox->SetScale(30.0f);
	skybox->SetModelTexture("Resource/3D/SkyBox/sky000.jpg", 0);

	new Floor(
		"Resource/2D/haikei.png",
		VGet(0, 0, 0),
		VGet(-11500, 0, -11500),
		VGet(11500, 0, 11500)
	);
	
	new Wall("", VGet(0.0f, 0.0f, 5000.0f), VGet(-5000.0f, 5000.0f, 0.0f), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("", VGet(0.0f, 0.0f, -5000.0f), VGet(-5000.0f, 5000.0f, 0.0), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("", VGet(5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
	new Wall("", VGet(-5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));

	// Wall is registered with kNone3d tag in Object3D ctor before SetTag(kTag3dWall) is called.
	// Rebuild the tag cache after all Walls are created so collision detection works correctly.
	ServiceLocator::GetObjectManager()->RebuildTagCache3D();
}

/*
 * 毎フレームの更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] マネージャー群の更新、制限時間終了時のシーン移行
 */
void Scene3D::Update()
{
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	Scene::Update();

	cow_manager_->Update();
	game_manager_->Update();
	PhaseUpdate();
	tatumaki->Update();

	// プレイ時間が終了した場合、進行を停止するためリザルト画面に移行
	if (ServiceLocator::GetGameManager()->GetGameTimer()->GetTime() <= 0)
	{
		fade_state_ = kSceneFadeOut;
		next_scene_ = SceneManager::kSceneResult;
	}
}

/*
 * 3Dシーンの描画処理
 * [入力] なし
 * [出力] なし
 * [副作用] オブジェクトの描画、UIの表示
 */
void Scene3D::Draw()
{
	Scene::Draw();
	
	DrawGrid();
	
	cow_manager_->Draw();
  
	if (game_manager_->GetGameTimer() && !(game_manager_->GetGameTimer()->OutTimerFlag()))
	{
		game_manager_->GetGameTimer()->Draw();
	}

	DrawPhaseUI();
}

void Scene3D::DrawGrid()
{
	const int count = 51;
	const float distance = -500.0f;
	
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
}

void Scene3D::DrawPhaseUI()
{
	int fontSize = GetFontSize();
	// プレイヤーに何のイベントが起きているか知らせるため、全画面の透過テキストを描画
	if (Master::camera_->GetIsPhaseCameraActive())
	{
		int currentPhase = (int)game_manager_->GetCurrentPhase();
		
		if (currentPhase == (int)GameManager::GamePhase::kMassSpawn || currentPhase == (int)GameManager::GamePhase::kTornadoCrisis)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawExtendGraph(0, 0, 1920, 1080, font_back_graph_, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		SetFontSize(40);
		if (currentPhase == (int)GameManager::GamePhase::kMassSpawn)
		{
			DrawString(200, 300, "MASS SPAWN!", GetColor(255, 50, 50), true);
			DrawString(200, 350, "牛が大量発生！", GetColor(255, 255, 255), true);
		}
		else if (currentPhase == (int)GameManager::GamePhase::kTornadoCrisis)
		{
			DrawString(200, 300, "TORNADO CRISIS!", GetColor(255, 100, 0), true);
			DrawString(200, 350, "巨大竜巻が接近中！", GetColor(255, 255, 255), true);
		}
		SetFontSize(fontSize);
	}
}

/*
 * 現在のフェーズに合わせた処理
 * [入力] なし
 * [出力] なし
 * [副作用] カメラの更新、大量発生時の定期スポーン
 */
void Scene3D::PhaseUpdate()
{
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	if (player != nullptr && tatumaki != nullptr) {
		int currentPhase = (int)game_manager_->GetCurrentPhase();
		Master::camera_->UpdateCameraByPhase(currentPhase, player->GetPosition(), tatumaki->GetPosition());

		tatumaki->SetCrisisMode(currentPhase == (int)GameManager::GamePhase::kTornadoCrisis);

		// 牛を連続して降らせるイベントのため、一定間隔で上空から追加スポーンする
		if (currentPhase == (int)GameManager::GamePhase::kMassSpawn)
		{
			mass_spawn_timer_++;
			if (mass_spawn_timer_ >= 60)
			{
				mass_spawn_timer_ = 0;

				VECTOR spawnCenter = VGet(0.0f, 0.0f, 0.0f);
				VECTOR spawnArgs = VGet(spawnCenter.x, spawnCenter.y + 2000.0f, spawnCenter.z);
				float scatterRadius = 4000.0f;

				cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, spawnArgs, 50.0f, CowMove::kCow1, 2, false, scatterRadius);
			}
		}
	}
}

/*
 * 3Dシーンの終了処理
 * [入力] なし
 * [出力] なし
 * [副作用] BGMの停止
 */
void Scene3D::Finalize()
{
	Master::mpSoundManager->StopBGM();
}
