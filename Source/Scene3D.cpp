#include "ServiceLocator.h"
#include "Scene3D.h"
#include "Master.h"
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
#include"Tornado.h"
#include"Utility.h"
#include "GameConstants.h"

Thunder* thunder_ = nullptr;     // ???????o?p??A?N?e?B?u??I?u?W?F?N?g?|?C???^
Tornado* tatumaki = nullptr;   // ?X?e?[?W???????????I?u?W?F?N?g?|?C???^

Scene3D::Scene3D()
{
	mass_spawn_timer_ = 0;
	font_back_graph_ = Master::mpResourceManager->LoadGraphics("Resource/2D/fontback.png");
}

Scene3D::~Scene3D()
{

	// ?? cow_manager_ ?? thunder_, tatumaki ???? Object3D ??p??????I?u?W?F?N?g??A
	// ???N???X Scene ??f?X?g???N?^?????? ObjectManager ??????????I?? delete ???????A
	// ???????闢?E? delete ????d????i?N???b?V???????j??h??????s???????B
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
	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);
	Master::mpSoundManager->SetBGMVolume(120);
	
	// ?}?b?v???E??????????????????????A?Z?b?g???????
	for (int i = 0; i < 6; i++)
	{
		new Object_Stage("Resource/3D/陬・｣ｾ/SmallTree1.mv1", VGet(6000, 0, -5000 + 2000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/SmallTree1.mv1", VGet(-6000, 0, -5000 + 2000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/Flower1.mv1", VGet(5500, 0, -5000 + 2000 * i), 3.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/SmallTree1.mv1", VGet(-6000, 0, -5000 + 2000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/SmallTree1.mv1", VGet(-5000 + 2000 * i, 0, -6000), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/SmallTree1.mv1", VGet(-5000 + 2000 * i, 0, 6000), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}

	for (int i = 0; i < 3; i++)
	{
		new Object_Stage("Resource/3D/陬・｣ｾ/BigTree1.mv1", VGet(5500, 0, -4000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/BigTree4.mv1", VGet(-5500, 0, -4000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/BigTree4.mv1", VGet(-4000 + 4000 * i, 0, 5500), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/BigTree3.mv1", VGet(-4000 + 4000 * i, 0, -5500), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}

	for (int i = 0; i < 2; i++)
	{
		new Object_Stage("Resource/3D/陬・｣ｾ/BigTree3.mv1", VGet(5500, 0, -2000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
		new Object_Stage("Resource/3D/陬・｣ｾ/BigTree2.mv1", VGet(-5500, 0, -2000 + 4000 * i), 1.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));
	}
	 
	new Object_Stage("Resource/3D/陬・｣ｾ/Grass2.mv1", VGet(0, 0, 0), 2.0f, VGet(0.0f, DX_PI_F / 1.0f, 0.0f));

	// ?X?e?[?W??L?????\????????A???E????l????R?iMountain?j??z?u
	VECTOR mountainScale = VGet(30.0f, 50.0f, 30.0f);
	float mountainDist = 13000.0f;
	auto m1 = new Mountain("Resource/3D/螻ｱ/蝨ｰ蠖｢.mv1", VGet(0, 0, mountainDist), mountainScale, VGet(0.0f, DX_PI_F, 0.0f));
	m1->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m2 = new Mountain("Resource/3D/螻ｱ/蝨ｰ蠖｢.mv1", VGet(0, 0, -mountainDist), mountainScale, VGet(0.0f, 0.0f, 0.0f));
	m2->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m3 = new Mountain("Resource/3D/螻ｱ/蝨ｰ蠖｢.mv1", VGet(mountainDist, 0, 0), mountainScale, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	m3->SetColor(0.2f, 0.3f, 0.2f, 1.0f);
	auto m4 = new Mountain("Resource/3D/螻ｱ/蝨ｰ蠖｢.mv1", VGet(-mountainDist, 0, 0), mountainScale, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
	m4->SetColor(0.2f, 0.3f, 0.2f, 1.0f);

	// 繝励Ξ繧?E?繧?E?繝ｪ繧?E?螟悶↓蜃?E?繧峨?E?縺?E?縺・?E?縺・?E??E?(Rock)繧帝?E鄂ｮ
	for (int i = 0; i < 40; i++)
	{
		float rockX = (float)(GetRand(15000) - 7500);
		float rockZ = (float)(GetRand(15000) - 7500);
		
		if (rockX > -4500 && rockX < 4500 && rockZ > -4500 && rockZ < 4500) {
			continue;
		}

		float rockScale = 1.0f + (float)(GetRand(30)) / 10.0f;
		float rockRot = (float)(GetRand(360)) * DX_PI_F / 180.0f;
		
		auto rock = new Object_Stage("Resource/3D/遏ｳ/rock.mv1", VGet(rockX, 0, rockZ), rockScale, VGet(0.0f, rockRot, 0.0f));
		rock->SetColor(0.4f, 0.7f, 0.3f, 1.0f);
	}

	// 繝励Ξ繧?E?繧?E?繝ｪ繧?E??E?E阜縺?E?繝輔ぉ繝ｳ繧?E?(Fence)繧帝?E鄂ｮ
	for (int i = 0; i < 5; i++)
	{
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(490.0f + 1000 * i, 0.0f, 5050.0f), 12.80f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(-490.0f + -1000 * i, 0.0f, 5050.0f), 12.80f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(490.0f + 1000 * i, 0.0f, -5050.0f), 12.80f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(-490.0f + -1000 * i, 0.0f, -5050.0f), 12.80f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(5050.0f , 0.0f, 490.0f + 1000* i), 12.80f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(5050.0f, 0.0f, -490.0f + -1000 * i), 12.80f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(-5050.0f, 0.0f, 490.0f + 1000 * i), 12.80f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/譁ｰ縺励＞譟ｵ/fence1.mv1", VGet(-5050.0f, 0.0f, -490.0f + -1000 * i), 12.80f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	}

	thunder_ = new Thunder(VGet(0.0f, 0.0f, 0.0f));
	tatumaki = new Tornado(VGet(3000.0f, 0.0f, 3000.0f));

	auto Player = new Player3D("Resource/3D/ufo2/Ufo.mv1", VGet(1000.0f, 2000.0f, 0.0f));
	Player->SetScale(0.6f);

	VECTOR spawnPos = Utility::StageSize; // 繧?E?繝昴・繝ｳ蛻晁E??菴咲?E??E?

	// 蛻晁E??繧?E?繝?E・繧?E?縺?E?逕溽?E??帝?E鄂ｮ
	cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCow1, 10);
	cow_manager_->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2);
	animal_manager_->SpawnAnimal(GameConstants::kAnimalChicken.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5);
	animal_manager_->SpawnAnimal(GameConstants::kAnimalBear.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5);
	
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
	
	// 繝槭ャ繝怜､門?E?√?E蠖薙?E??雁?E螳夂畑繧?E?繝ｩ繧?E?繝繝ｼ險?E?螳・
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

	cow_manager_->Update();
	game_manager_->Update();
	PhaseUpdate();
	tatumaki->Update();

	// 蛻?E?髯先?E髢鍋ｵめE??・〒繝ｪ繧?E?繝ｫ繝育判髱?E?縺?E?縺?E?繝輔ぉ繝ｼ繝蛾幕蟋・
	if (ServiceLocator::GetGameManager()->GetGameTimer()->GetTime() <= 0)
	{
		fade_state_ = kSceneFadeOut;
		next_scene_ = SceneManager::kSceneResult;
	}

	if (fade_state_ == kSceneFadeOut)
	{
		// 繝輔ぉ繝ｼ繝峨ぁE???E?繝医↓蜷医?E?縺帙※BGM繧偵ヵ繧?E?繝ｼ繝峨ぁE???E?繝?E
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
		}
	}
}

void Scene3D::Draw()
{
	Scene::Draw();
	const int count = 51;
	const float distance = -500.0f;
	
	// 繝?Eヰ繝?Eげ逕ｨ縺?E?繧?E?繝ｪ繝?Eラ謠冗判
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
	
	cow_manager_->Draw();
  
	if (game_manager_->GetGameTimer() && !(game_manager_->GetGameTimer()->OutTimerFlag()))
	{
		game_manager_->GetGameTimer()->Draw();
	}

	// 繧?E?繝吶Φ繝医ヵ繧?E?繝ｼ繧?E?縺?E?蠢懊§縺溯?E??E?蜻翫ユ繧?E?繧?E?繝郁?E??E?遉ｺ
	if (Master::camera_->GetIsPhaseCameraActive())
	{
		int currentPhase = (int)game_manager_->GetCurrentPhase();
		
		if (currentPhase == (int)GameManager::GamePhase::kMassSpawn || currentPhase == (int)GameManager::GamePhase::kTornadoCrisis)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawExtendGraph(0, 0, 1920, 1080, font_back_graph_, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		if (currentPhase == (int)GameManager::GamePhase::kMassSpawn)
		{
			SetFontSize(64);
			DrawFormatString(600, 200, GetColor(255, 100, 100), "迚帙′螟ｧ驥冗匱逕?E!");
			SetFontSize(16);
		}
		else if (currentPhase == (int)GameManager::GamePhase::kTornadoCrisis)
		{
			SetFontSize(64);
			DrawFormatString(600, 200, GetColor(255, 100, 100), "蜿?E?鬚ｨ縺悟ｷ?E?螟ｧ蛹・!");
			SetFontSize(16);
		}
	}

	if (fade_state_ != kSceneFadeNone)
	{
		Scene::Fade(fade_state_);
	}
}

/*
 * @brief 繝輔ぉ繝ｼ繧?E?騾?E?陦悟宛蠕｡
 * [蜈･蜉嫁E縺?E?縺・[?E??蜉嫁E縺?E?縺・[蜑ｯ菴?E??] 繧?E?繝｡繝ｩ・遶懷?E??E?迥?E?諷句?E?画峩縲∝､?E?驥上せ繝昴・繝ｳ?E??送E?E
 */
void Scene3D::PhaseUpdate()
{
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	if (player != nullptr && tatumaki != nullptr) {
		int currentPhase = (int)game_manager_->GetCurrentPhase();
		Master::camera_->UpdateCameraByPhase(currentPhase, player->GetPosition(), tatumaki->GetPosition());

		tatumaki->SetCrisisMode(currentPhase == (int)GameManager::GamePhase::kTornadoCrisis);

		// 螟ｧ驥丞?E迴?E?繝輔ぉ繝ｼ繧?E?譎ゅ・0繝輔Ξ繝ｼ繝髢馴囈縺?E?迚帙ｒ繧?E?繝昴・繝ｳ
		if (currentPhase == (int)GameManager::GamePhase::kMassSpawn)
		{
			mass_spawn_timer_++;
			if (mass_spawn_timer_ >= 60)
			{
				mass_spawn_timer_ = 0;

				VECTOR spawnCenter = player->GetPosition();
				// 繧?E?繝昴・繝ｳ菴咲?E??E?繧偵・繝ｬ繧?E?繝､繝ｼ荳顔ｩ?E?(y+2000)縺?E?險?E?螳・
				VECTOR spawnArgs = VGet(4000.0f, spawnCenter.y + 2000.0f, 4000.0f);

				cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, spawnArgs, 50.0f, CowMove::kCow1, 2);
			}
		}
	}
}

void Scene3D::Finalize()
{
	Master::mpSoundManager->StopBGM(); // 繧?E?繝ｼ繝ｳ邨めE??・凾縺?E?BGM繧貞●豁E??
}