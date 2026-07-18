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
#include "StageLoader.h"
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
	font_back_graph_ = Master::resource_manager_->LoadGraphics("Resource/2D/FontPanelBackground.png");
	shadow_map_handle_ = -1;
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
	// 設定画面でのON/OFF選択に従い、シャドウマップの生成を制御する
	if (Master::is_shadow_enabled_)
	{
		shadow_map_handle_ = MakeShadowMap(2048, 2048);
	}
	else
	{
		shadow_map_handle_ = -1;
	}
	if (shadow_map_handle_ != -1)
	{
		SetShadowMapDrawArea(shadow_map_handle_, VGet(-7000.0f, -100.0f, -7000.0f), VGet(7000.0f, 3500.0f, 7000.0f));
		SetShadowMapLightDirection(shadow_map_handle_, VNorm(VGet(-1.0f, -2.0f, -1.0f)));
		SetShadowMapAdjustDepth(shadow_map_handle_, 0.0005f);
	}

	Master::camera_->Initialize();

	Master::caught_cow_count_ = 0;
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);

	Master::sound_manager_->PlayBGM(SoundManager::kBgmGame);
	Master::sound_manager_->SetBGMVolume(120);

	StageLoader::LoadFromCSV("Resource/Data/stage_objects.csv");

	thunder_ = new Thunder(VGet(0.0f, 0.0f, 0.0f));
	tatumaki = new Tornado(VGet(3000.0f, 0.0f, 3000.0f));

	auto Player = new Player3D("Resource/3D/Player/UFO.mv1", VGet(1000.0f, 2000.0f, 0.0f));
	Player->SetScale(0.6f);

	VECTOR spawnPos = VGet(0.0f, 0.0f, 0.0f); // スポーン中心位置

	// 牛と動物のスポーン
	cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCow1, 10, false, Utility::StageSize.x);
	cow_manager_->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2, false, Utility::StageSize.x);
	animal_manager_->SpawnAnimal(GameConstants::kAnimalChicken.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5, Utility::StageSize.x);
	animal_manager_->SpawnAnimal(GameConstants::kAnimalBear.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5, Utility::StageSize.x);

	phase_ = kNormal;

	auto skybox = new SkyBox("Resource/3D/SkyBox/SkyBox.mv1", VGet(0, 0, 0));
	skybox->SetScale(30.0f);
	skybox->SetModelTexture("Resource/3D/SkyBox/sky000.jpg", 0);

	new Floor(
		"Resource/2D/GameBackground.png",
		VGet(0, 0, 0),
		VGet(-11500, 0, -11500),
		VGet(11500, 0, 11500)
	);

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
	DrawShadowMap();

	if (shadow_map_handle_ != -1)
	{
		SetUseShadowMap(0, shadow_map_handle_);
		SetLightUseShadowMap(0, TRUE);
	}

	Scene::Draw();

	DrawGrid();

	cow_manager_->Draw();

	if (game_manager_->GetGameTimer() && !(game_manager_->GetGameTimer()->OutTimerFlag()))
	{
		game_manager_->GetGameTimer()->Draw();
	}

	DrawPhaseUI();

	if (shadow_map_handle_ != -1)
	{
		SetUseShadowMap(0, -1);
	}
}

void Scene3D::DrawShadowMap()
{
	if (shadow_map_handle_ == -1) return;

	ShadowMap_DrawSetup(shadow_map_handle_);

	auto objMgr = ServiceLocator::GetObjectManager();
	if (objMgr != nullptr)
	{
		for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dPlayer))
		{
			obj->DrawShadowCaster();
		}
		for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dCow))
		{
			obj->DrawShadowCaster();
		}
		for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dAnimal))
		{
			obj->DrawShadowCaster();
		}
	}

	ShadowMap_DrawEnd();
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
	if (shadow_map_handle_ != -1)
	{
		DeleteShadowMap(shadow_map_handle_);
		shadow_map_handle_ = -1;
	}
	Master::sound_manager_->StopBGM();
}
