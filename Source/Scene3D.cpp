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

Thunder* thunder = nullptr;     // 落雷演出用のアクティブなオブジェクトポインタ
Tatumaki* tatumaki = nullptr;   // ステージ上を巡回する竜巻オブジェクトポインタ

Scene3D::Scene3D()
{
	mMassSpawnTimer = 0;
	mFontBackGraph = LoadGraph("Resource/2D/fontback.png");
}

Scene3D::~Scene3D()
{
	DeleteGraph(mFontBackGraph); // シーン個有の画像リソースのみ解放
	// ※ mpCowManager や thunder, tatumaki などの Object3D を継承するオブジェクトは、
	// 基底クラス Scene のデストラクタ内にある ObjectManager によって自動的に delete されるため、
	// ここでの手動 delete は多重解放（クラッシュ原因）を防ぐため行いません。
}

/*
 * @brief 3Dステージシーンの初期設定（マップ、コライダー、プレイヤー、エネミー生成）を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 各オブジェクトのnew、BGM再生開始、コライダー登録
 */
void Scene3D::Initialize()
{
	Master::mnCaughtCowCount = 0;
	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);

	Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME);
	Master::mpSoundManager->SetBGMVolume(120);
	
	// マップ境界沿いに並べる樹木や花などの装飾アセットを一元生成
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

	// ステージの広がりを表現するため、視界を遮る四方の山（Mountain）を配置
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

	// プレイエリアの外郭境界部にランダムな遮蔽岩（Rock）を配置
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

	// プレイエリア境界（ステージ端）を視覚的に分かりやすくするための柵（Fence）の生成
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

	VECTOR spawnPos = Utility::StageSize; // スポーン判定域（ステージサイズ範囲内）

	// 初期ステージ配置として牛と野生動物をランダム座標へ配置
	mpCowManager->SpawnCow("Resource/3D/Cow/uploads_files_3880923_Cow.mv1", spawnPos, 50.0f, CowMove::Cow_1, 10);
	mpCowManager->SpawnCow("Resource/3D/GOLDCow/GoldCow.mv1", spawnPos, 50.0f, CowMove::Cow_gold, 2);
	mpAnimalManager->SpawnAnimal("Resource/3D/Animal/uploads_files_3881253_Chicken_Low.mv1", spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	mpAnimalManager->SpawnAnimal("Resource/3D/Animal/uploads_files_3887296_Black_Bear.mv1", spawnPos, 50.0f, AnimalMove::Animal_1, 5);
	
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
	
	// 見えないマップ境界壁のコリジョンを四方に生成
	new Wall("Resource", VGet(0.0f, 0.0f, 5000.0f), VGet(-5000.0f, 5000.0f, 0.0f), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("Resource", VGet(0.0f, 0.0f, -5000.0f), VGet(-5000.0f, 5000.0f, 0.0), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("Resource", VGet(5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
	new Wall("Resource", VGet(-5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
}

void Scene3D::Update()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	Scene::Update();

	mpCowManager->Update();
	mpGameManager->Update();
	PhaseUpdate();
	tatumaki->Update();

	// 制限時間タイマーが終了した時点でリザルト遷移のフェードを開始
	if (Master::mpSceneManager->GetCurrentScene()->mpGameManager->GetGameTimer()->GetTime() <= 0)
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_RESULT;
	}

	if (mFadeState == SceneFade_Out)
	{
		// フェードアウトに合わせてBGMの音量を滑らかに下げる
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
	
	// デバッグ用およびプレイヤー位置把握のためのグリッド線を床に描画
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

	// イベントカメラ起動中、プレイヤーに注意を促すイベントテキスト表示
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
			DrawFormatString(600, 200, GetColor(255, 100, 100), "牛が大量発生！！");
			SetFontSize(16);
		}
		else if (currentPhase == (int)GameManager::GamePhase::TornadoCrisis)
		{
			SetFontSize(64);
			DrawFormatString(600, 200, GetColor(255, 100, 100), "竜巻が巨大化！！");
			SetFontSize(16);
		}
	}

	if (mFadeState != SceneFade_None)
	{
		Scene::Fade(mFadeState);
	}
}

/*
 * @brief ゲームのフェーズ遷移状況に追従して、カメラワーク、竜巻サイズ、牛の定期降雨イベントを処理する
 * [入力] なし
 * [出力] なし
 * [副作用] カメラ状態更新、竜巻の拡大、牛の新規追加生成(new)
 */
void Scene3D::PhaseUpdate()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	
	if (player != nullptr && tatumaki != nullptr) {
		int currentPhase = (int)mpGameManager->GetCurrentPhase();
		Master::mpCamera->UpdateCameraByPhase(currentPhase, player->GetPosition(), tatumaki->GetPosition());

		tatumaki->SetCrisisMode(currentPhase == (int)GameManager::GamePhase::TornadoCrisis);

		// 大量発生フェーズ中は、一定間隔（60フレーム）でプレイヤー上空から牛を降らせる
		if (currentPhase == (int)GameManager::GamePhase::MassSpawn)
		{
			mMassSpawnTimer++;
			if (mMassSpawnTimer >= 60)
			{
				mMassSpawnTimer = 0;

				VECTOR spawnCenter = player->GetPosition();
				// スポーン時に中心座標(spawnCenter)と落下開始位置(y+2000)、および拡散範囲幅を合成して渡す
				VECTOR spawnArgs = VGet(4000.0f, spawnCenter.y + 2000.0f, 4000.0f);

				mpCowManager->SpawnCow("Resource/3D/Cow/uploads_files_3880923_Cow.mv1", spawnArgs, 50.0f, CowMove::Cow_1, 2);
			}
		}
	}
}

void Scene3D::Finalize()
{
	Master::mpSoundManager->StopBGM(); // 次画面に遷移するにあたりゲーム中BGMを止める
}
