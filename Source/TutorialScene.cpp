#include "ServiceLocator.h"
#include "TutorialScene.h"
#include "CowManager.h"
#include "Object_Stage.h"
#include "Player3D.h"
#include "Floor.h"
#include "Skill.h"
#include "GameConstants.h"
#include "Wall.h"
#include "Scene.h"
#include "Camera.h"
#include "Master.h"
#include "Combo.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "CowMove.h"
#include "Texture.h"
#include "Cow_gold.h"

TutorialScene::TutorialScene()
	: Scene() 
{
	// CowManagerの生成と破棄は基底クラスSceneにて行われるためここでは処理しない
	CowDelete = false;
	mfeverstate = FEVER_1; 
	timerCount = 0;        
}

TutorialScene::~TutorialScene()
{
	DeleteFontToHandle(fontHandle);

	if (texture)  { delete texture;  texture = nullptr; }
	if (texture2) { delete texture2; texture2 = nullptr; }
	if (texture3) { delete texture3; texture3 = nullptr; }
	if (texture4) { delete texture4; texture4 = nullptr; }
	if (texture5) { delete texture5; texture5 = nullptr; }
	if (texture6) { delete texture6; texture6 = nullptr; }
	
	// CowManagerの破棄はSceneのデストラクタに委譲する
}

void TutorialScene::Initialize()
{
	fadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	fontHandle = CreateFontToHandle("???C???I", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);

	CreateFences();
	CreateWalls();

	new Floor(
		"Resource/2D/haikei.png",
		VGet(0, 0, 0),
		VGet(-11500, 0, -11500),
		VGet(11500, 0, 11500)
	);
	
	Master::soundManager->PlayBGM(SoundManager::BGM_TUTORIAL);
	auto Player = new Player3D("Resource/3D/ufo2/uploads_files_2595751_UFO.mv1", VGet(0.0f, 1000.0f, 0.0f));
	Player->SetScale(0.6f);

	SetCamera(Master::camera);

	VECTOR pos = VGet(1200, 150, 0);
	float GraphSize_x = 800, GraphSize_y = 300;
	texture  = new Texture("Resource/2D/tutorial_1_move.png", pos, GraphSize_x, GraphSize_y, true);
	texture2 = new Texture("Resource/2D/tutorial_2_beam.png", pos, GraphSize_x, GraphSize_y, true);
	texture3 = new Texture("Resource/2D/tutorial_3_combo.png", pos, GraphSize_x, GraphSize_y, true);
	texture4 = new Texture("Resource/2D/tutorial_4_phase.png", pos, GraphSize_x, GraphSize_y, true);
	texture5 = new Texture("Resource/2D/tutorial_5_skill.png", pos, GraphSize_x, GraphSize_y, true);
	texture6 = new Texture("Resource/2D/tutorial_6_fever.png", pos, GraphSize_x, GraphSize_y, true);
}

void TutorialScene::CreateFences()
{
	for (int i = 0; i < 5; ++i) {
		float z = 5050.0f;
		float x = 500.0f + (i * 1000.0f);
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(x, -25.0f, z), 2.5f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(-x, -25.0f, z), 2.5f, VGet(0.0f, 0.0f, 0.0f));
		
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(x, -25.0f, -z), 2.5f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(-x, -25.0f, -z), 2.5f, VGet(0.0f, DX_PI_F, 0.0f));
	}

	for (int i = 0; i < 5; ++i) {
		float x = 5050.0f;
		float z = 500.0f + (i * 1000.0f);
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(x, -25.0f, z), 2.5f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(x, -25.0f, -z), 2.5f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));

		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(-x, -25.0f, z), 2.5f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/fanse/fanse/Fence.mv1", VGet(-x, -25.0f, -z), 2.5f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	}
}

void TutorialScene::CreateWalls()
{
	new Wall("Resource", VGet(0.0f, 0.0f, 5000.0f), VGet(-5000.0f, 5000.0f, 0.0f), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("Resource", VGet(0.0f, 0.0f, -5000.0f), VGet(-5000.0f, 5000.0f, 0.0), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("Resource", VGet(5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
	new Wall("Resource", VGet(-5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
}

void TutorialScene::SetCamera(Camera* camera)
{
	camera = camera;
}

void TutorialScene::Update()
{
	if (camera != nullptr)
	{
		camera->Update();
	}
	if (cowManager != nullptr)
	{
		cowManager->Update();
	}

	switch (state)
	{
	case STATE_MOVE:
		UpdateStateMove();         
		break;
	case STATE_BEAM:
		UpdateStateBeam();         
		break;
	case STATE_COMBO_SCORE:
		UpdateStateComboScore();   
		break;
	case STATE_PHASE:
		UpdateStatePhase();        
		break;
	case STATE_SKILL:
		UpdateStateSkill();        
		break;
	case STATE_FEVER:
		UpdateStateFever();        
		break;
	case STATE_END:
		UpdateStateEnd();          
		break;
	}

	// チュートリアル終了時、ENTERキー入力でタイトル画面へ遷移する
	if(InputManager::CheckDownKey(KEY_INPUT_RETURN) && state != STATE_END)
	{
		fadeState = SceneFade_Out;
		nextScene = SceneManager::SCENE_TITLE;
	}

	if (fadeState == SceneFade_Out)
	{
		Master::soundManager->SetBGMVolume((Master::soundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::sceneManager->SetNextScene((SceneManager::SCENE_TYPE)nextScene);
		}
	}
	
	Scene::Update();
	
}

// 移動操作（WASD）を確認し、次のステップへ進行する
void TutorialScene::UpdateStateMove()
{
	if (texture) texture->Draw();

	if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_D))
	{
		Master::soundManager->PlaySE(SoundManager::SE_TutorialChange);
		state = STATE_BEAM;
		
		VECTOR spawnPos = VGet(0, 0.0f, 1000.0f);
		cowManager->SpawnCow(GameConstants::COW_DEFAULT.modelPath, spawnPos, 50.0f, CowMove::Cow_1, 1);
	}
}

// マウス左クリックでビーム吸引操作を確認する
void TutorialScene::UpdateStateBeam()
{
	if (texture2) texture2->Draw();

	if (GetMouseInput() & MOUSE_INPUT_LEFT)
	{
		Master::soundManager->PlaySE(SoundManager::SE_TutorialChange);
		state = STATE_COMBO_SCORE;

		// コンボ説明用として複数の牛をスポーンさせる
		cowManager->SpawnCow(GameConstants::COW_DEFAULT.modelPath, VGet(500, 0.0f, 1000.0f), 50.0f, CowMove::Cow_1, 2);
		cowManager->SpawnCow(GameConstants::COW_DEFAULT.modelPath, VGet(-500, 0.0f, 1000.0f), 50.0f, CowMove::Cow_1, 2);
	}
}

// プレイヤーのコンボが2以上になったことを確認して次に進む
void TutorialScene::UpdateStateComboScore()
{
	if (texture3) texture3->Draw();

	Player3D* player = ServiceLocator::GetPlayer();

	if (player && player->combo->GetCombo() >= 2)
	{
		Master::soundManager->PlaySE(SoundManager::SE_TutorialChange);
		state = STATE_PHASE;
		timerCount = 0; 
	}
}

// フェーズ説明のため、一定時間（約3秒）待機した後に進行する
void TutorialScene::UpdateStatePhase()
{
	if (texture4) texture4->Draw();

	timerCount++;
	if (timerCount > 180)
	{
		Master::soundManager->PlaySE(SoundManager::SE_TutorialChange);
		state = STATE_SKILL;
	}
}

// スキル発動操作を確認し、フィーバー体験へ移行する
void TutorialScene::UpdateStateSkill()
{
	Player3D* player = ServiceLocator::GetPlayer();

	if (!player) return;

	if (player->skill->AddSkillFlag)
	{
		if(texture5) texture5->Draw();
		skillFlag = true;
	}

	if (player->skill->AddSkillFlag == false && skillFlag)
	{
		skillFlag = false;
		state = STATE_FEVER;
		Master::soundManager->PlaySE(SoundManager::SE_TutorialChange);
		
		// フィーバーモード用にゴールド牛を配置する
		auto g = new Cow_gold("Resource/3D/GOLDCow/GoldCow.mv1", VGet(1000, 0, 1000), Cow_gold::NoFever);
		g->SetScale(100);
	}
}

// フィーバーモードの開始と終了を監視する
void TutorialScene::UpdateStateFever()
{
	if (mfeverstate == FEVER_1)
	{
		if(texture6) texture6->Draw(); 
		
		if (fever && fever->IsFever()) { 
			mfeverstate = FEVER_2; 
		}
	}
	else if (mfeverstate == FEVER_2)
	{
		if (fever && fever->IsFever() == false) { 
			state = STATE_END; 
		}
	}
}

// チュートリアル終了テキストを表示し入力待機状態にする
void TutorialScene::UpdateStateEnd()
{
	DrawTutorialText("Enterでチュートリアルを終わります ");

	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		fadeState = SceneFade_Out;
		nextScene = SceneManager::SCENE_TITLE;
	}
}

void TutorialScene::Draw()
{
	Scene::Draw();
}

void TutorialScene::DrawTutorialText(const char* text, int yOffset)
{
	unsigned int color = GetColor(255, 255, 0);
	DrawFormatStringToHandle(400, 50 + yOffset, color, fontHandle, text);
}

void TutorialScene::Finalize()
{
	Master::soundManager->StopBGM();  
}

