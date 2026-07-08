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
#include"Tatumaki.h"
#include"Utility.h"
#include "GameConstants.h"

Thunder* thunder = nullptr;     // ???????o?p??A?N?e?B?u??I?u?W?F?N?g?|?C???^
Tatumaki* tatumaki = nullptr;   // ?X?e?[?W???????????I?u?W?F?N?g?|?C???^

Scene3D::Scene3D()
{
	mMassSpawnTimer = 0;
	mFontBackGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/fontback.png");
}

Scene3D::~Scene3D()
{

	// ?? mpCowManager ?? thunder, tatumaki ???? Object3D ??p??????I?u?W?F?N?g??A
	// ???N???X Scene ??f?X?g???N?^?????? ObjectManager ??????????I?? delete ???????A
	// ???????髣｢・ｮ delete ????d????i?N???b?V???????j??h??????s???????B
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

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);
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

	// 郢晏干ﾎ樒ｹｧ・､郢ｧ・ｨ郢晢ｽｪ郢ｧ・｢陞滓じ竊楢怎・ｺ郢ｧ蟲ｨ・檎ｸｺ・ｪ邵ｺ繝ｻ・育ｸｺ繝ｻ・ｲ・ｩ(Rock)郢ｧ蟶昴・驗ゑｽｮ
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

	// 郢晏干ﾎ樒ｹｧ・､郢ｧ・ｨ郢晢ｽｪ郢ｧ・｢陟・・髦懃ｸｺ・ｫ郢晁ｼ斐♂郢晢ｽｳ郢ｧ・ｹ(Fence)郢ｧ蟶昴・驗ゑｽｮ
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

	VECTOR spawnPos = Utility::StageSize; // 郢ｧ・ｹ郢晄亢繝ｻ郢晢ｽｳ陋ｻ譎・ｄ闖ｴ蜥ｲ・ｽ・ｮ

	// 陋ｻ譎・ｄ郢ｧ・ｹ郢昴・繝ｻ郢ｧ・ｸ邵ｺ・ｮ騾墓ｺｽ鮟・ｹｧ蟶昴・驗ゑｽｮ
	mpCowManager->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCow1, 10);
	mpCowManager->SpawnCow(GameConstants::kCowGold.model_path, spawnPos, 50.0f, CowMove::kCowGold, 2);
	mpAnimalManager->SpawnAnimal(GameConstants::kAnimalChicken.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5);
	mpAnimalManager->SpawnAnimal(GameConstants::kAnimalBear.model_path, spawnPos, 50.0f, AnimalMove::kAnimal1, 5);
	
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
	
	// 郢晄ｧｭ繝｣郢晄懶ｽ､髢・｣竏壹・陟冶侭笳・ｹｧ髮∵・陞ｳ螟ら舞郢ｧ・ｳ郢晢ｽｩ郢ｧ・､郢敖郢晢ｽｼ髫ｪ・ｭ陞ｳ繝ｻ
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

	// 陋ｻ・ｶ鬮ｯ蜈亥・鬮｢骰具ｽｵ繧・ｽｺ繝ｻ縲堤ｹ晢ｽｪ郢ｧ・ｶ郢晢ｽｫ郢晁ご蛻､鬮ｱ・｢邵ｺ・ｸ邵ｺ・ｮ郢晁ｼ斐♂郢晢ｽｼ郢晁崟蟷戊沂繝ｻ
	if (ServiceLocator::GetGameManager()->GetGameTimer()->GetTime() <= 0)
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_RESULT;
	}

	if (mFadeState == SceneFade_Out)
	{
		// 郢晁ｼ斐♂郢晢ｽｼ郢晏ｳｨ縺・ｹｧ・ｦ郢晏現竊楢惺蛹ｻ・冗ｸｺ蟶吮ｻBGM郢ｧ蛛ｵ繝ｵ郢ｧ・ｧ郢晢ｽｼ郢晏ｳｨ縺・ｹｧ・ｦ郢昴・
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
	
	// 郢昴・繝ｰ郢昴・縺帝包ｽｨ邵ｺ・ｮ郢ｧ・ｰ郢晢ｽｪ郢昴・繝ｩ隰蜀怜愛
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

	// 郢ｧ・､郢晏生ﾎｦ郢晏現繝ｵ郢ｧ・ｧ郢晢ｽｼ郢ｧ・ｺ邵ｺ・ｫ陟｢諛環ｧ邵ｺ貅ｯ・ｭ・ｦ陷ｻ鄙ｫ繝ｦ郢ｧ・ｭ郢ｧ・ｹ郢晞メ・｡・ｨ驕会ｽｺ
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
			DrawFormatString(600, 200, GetColor(255, 100, 100), "霑壼ｸ吮ｲ陞滂ｽｧ鬩･蜀怜験騾輔・!");
			SetFontSize(16);
		}
		else if (currentPhase == (int)GameManager::GamePhase::TornadoCrisis)
		{
			SetFontSize(64);
			DrawFormatString(600, 200, GetColor(255, 100, 100), "陷ｿ・ｰ鬯夲ｽｨ邵ｺ謔滂ｽｷ・ｨ陞滂ｽｧ陋ｹ繝ｻ!");
			SetFontSize(16);
		}
	}

	if (mFadeState != SceneFade_None)
	{
		Scene::Fade(mFadeState);
	}
}

/*
 * @brief 郢晁ｼ斐♂郢晢ｽｼ郢ｧ・ｺ鬨ｾ・ｲ髯ｦ謔溷ｮ幄包ｽ｡
 * [陷茨ｽ･陷牙ｫ・邵ｺ・ｪ邵ｺ繝ｻ[陷・ｽｺ陷牙ｫ・邵ｺ・ｪ邵ｺ繝ｻ[陷托ｽｯ闖ｴ諛・舞] 郢ｧ・ｫ郢晢ｽ｡郢晢ｽｩ郢晢ｽｻ驕ｶ諛ｷ・ｷ・ｻ霑･・ｶ隲ｷ蜿･・､逕ｻ蟲ｩ邵ｲ竏晢ｽ､・ｧ鬩･荳翫○郢晄亢繝ｻ郢晢ｽｳ陷・ｽｦ騾・・
 */
void Scene3D::PhaseUpdate()
{
	auto p = ServiceLocator::GetPlayer();
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	if (player != nullptr && tatumaki != nullptr) {
		int currentPhase = (int)mpGameManager->GetCurrentPhase();
		Master::mpCamera->UpdateCameraByPhase(currentPhase, player->GetPosition(), tatumaki->GetPosition());

		tatumaki->SetCrisisMode(currentPhase == (int)GameManager::GamePhase::TornadoCrisis);

		// 陞滂ｽｧ鬩･荳槭・霑ｴ・ｾ郢晁ｼ斐♂郢晢ｽｼ郢ｧ・ｺ隴弱ｅﾂ繝ｻ0郢晁ｼ釆樒ｹ晢ｽｼ郢晢｣ｰ鬮｢鬥ｴ蝗育ｸｺ・ｧ霑壼ｸ呻ｽ堤ｹｧ・ｹ郢晄亢繝ｻ郢晢ｽｳ
		if (currentPhase == (int)GameManager::GamePhase::MassSpawn)
		{
			mMassSpawnTimer++;
			if (mMassSpawnTimer >= 60)
			{
				mMassSpawnTimer = 0;

				VECTOR spawnCenter = player->GetPosition();
				// 郢ｧ・ｹ郢晄亢繝ｻ郢晢ｽｳ闖ｴ蜥ｲ・ｽ・ｮ郢ｧ蛛ｵ繝ｻ郢晢ｽｬ郢ｧ・､郢晢ｽ､郢晢ｽｼ闕ｳ鬘費ｽｩ・ｺ(y+2000)邵ｺ・ｫ髫ｪ・ｭ陞ｳ繝ｻ
				VECTOR spawnArgs = VGet(4000.0f, spawnCenter.y + 2000.0f, 4000.0f);

				mpCowManager->SpawnCow(GameConstants::kCowDefault.model_path, spawnArgs, 50.0f, CowMove::kCow1, 2);
			}
		}
	}
}

void Scene3D::Finalize()
{
	Master::mpSoundManager->StopBGM(); // 郢ｧ・ｷ郢晢ｽｼ郢晢ｽｳ驍ｨ繧・ｽｺ繝ｻ蜃ｾ邵ｺ・ｫBGM郢ｧ雋樞酪雎・ｽ｢
}