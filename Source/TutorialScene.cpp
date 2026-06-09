#include "TutorialScene.h"
#include "CowManager.h"
#include "Object_Stage.h"
#include "Player3D.h"
#include "Floor.h"
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
	// ※ mpCowManager = new CowManager() は基底クラス Scene::Scene() で
	// 既にインスタンス生成されているため、ここでの再生成（ポインタ上書きによるメモリリーク）は行いません。
	CowDelete = false;
	mfeverstate = FEVER_1; 
	timerCount = 0;        
}

TutorialScene::~TutorialScene()
{
	DeleteFontToHandle(fontHandle);

	if (mpTexture)  { delete mpTexture;  mpTexture = nullptr; }
	if (mpTexture2) { delete mpTexture2; mpTexture2 = nullptr; }
	if (mpTexture3) { delete mpTexture3; mpTexture3 = nullptr; }
	if (mpTexture4) { delete mpTexture4; mpTexture4 = nullptr; }
	if (mpTexture5) { delete mpTexture5; mpTexture5 = nullptr; }
	if (mpTexture6) { delete mpTexture6; mpTexture6 = nullptr; }
	
	// ※ mpCowManager は基底クラス Scene のデストラクタで自動解放されるため、
	// ここでの手動解放は行いません。
}

void TutorialScene::Initialize()
{
	mFadeState = SceneFade_In;
	SetFadeAlpha(255.0f);
	fontHandle = CreateFontToHandle("メイリオ", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);

	CreateFences();
	CreateWalls();

	new Floor(
		"Resource/2D/haikei.png",
		VGet(0, 0, 0),
		VGet(-11500, 0, -11500),
		VGet(11500, 0, 11500)
	);
	
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TUTORIAL);
	auto Player = new Player3D("Resource/3D/ufo2/uploads_files_2595751_UFO.mv1", VGet(0.0f, 1000.0f, 0.0f));
	Player->SetScale(0.6f);

	SetCamera(Master::mpCamera);

	VECTOR pos = VGet(1200, 150, 0);
	float GraphSize_x = 800, GraphSize_y = 300;
	mpTexture  = new Texture("Resource/2D/tutorial_1_move.png", pos, GraphSize_x, GraphSize_y, true);
	mpTexture2 = new Texture("Resource/2D/tutorial_2_beam.png", pos, GraphSize_x, GraphSize_y, true);
	mpTexture3 = new Texture("Resource/2D/tutorial_3_combo.png", pos, GraphSize_x, GraphSize_y, true);
	mpTexture4 = new Texture("Resource/2D/tutorial_4_phase.png", pos, GraphSize_x, GraphSize_y, true);
	mpTexture5 = new Texture("Resource/2D/tutorial_5_skill.png", pos, GraphSize_x, GraphSize_y, true);
	mpTexture6 = new Texture("Resource/2D/tutorial_6_fever.png", pos, GraphSize_x, GraphSize_y, true);
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
	mpCamera = camera;
}

void TutorialScene::Update()
{
	if (mpCamera != nullptr)
	{
		mpCamera->Update();
	}

	switch (mState)
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

	// チュートリアル中いつでもENTERキーを押せばタイトルへ強制的に戻れるショートカット
	if(InputManager::CheckDownKey(KEY_INPUT_RETURN) && mState != STATE_END)
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_TITLE;
	}

	if (mFadeState == SceneFade_Out)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)mNextScene);
		}
	}

	Scene::Update();
	mpCowManager->Update();
}

// 1. 移動操作説明：WASD入力のいずれかを検知したら次のステップへ進む
void TutorialScene::UpdateStateMove()
{
	if (mpTexture) mpTexture->Draw();

	if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_D))
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_TutorialChange);
		mState = STATE_BEAM;
		
		VECTOR spawnPos = VGet(0, 0.0f, 1000.0f);
		mpCowManager->SpawnCow("Resource/3D/Cow/uploads_files_3880923_Cow.mv1", spawnPos, 50.0f, CowMove::Cow_1, 1);
	}
}

// 2. 吸引説明：マウス左クリック（ビーム照射）が行われたら次のステップへ進む
void TutorialScene::UpdateStateBeam()
{
	if (mpTexture2) mpTexture2->Draw();

	if (GetMouseInput() & MOUSE_INPUT_LEFT)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_TutorialChange);
		mState = STATE_COMBO_SCORE;

		// コンボ説明用に、吸引対象の牛を追加スポーン
		mpCowManager->SpawnCow("Resource/3D/Cow/uploads_files_3880923_Cow.mv1", VGet(500, 0.0f, 1000.0f), 50.0f, CowMove::Cow_1, 2);
		mpCowManager->SpawnCow("Resource/3D/Cow/uploads_files_3880923_Cow.mv1", VGet(-500, 0.0f, 1000.0f), 50.0f, CowMove::Cow_1, 2);
	}
}

// 3. コンボスコア説明：プレイヤーが2連続以上で牛を回収し、コンボを成立させたら次のステップへ
void TutorialScene::UpdateStateComboScore()
{
	if (mpTexture3) mpTexture3->Draw();

	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);

	if (player && player->mpCombo->GetCombo() >= 2)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_TutorialChange);
		mState = STATE_PHASE;
		timerCount = 0; 
	}
}

// 4. フェーズ演出説明：一定時間(3秒)その場で案内を読ませたら自動的に次ステップへ
void TutorialScene::UpdateStatePhase()
{
	if (mpTexture4) mpTexture4->Draw();

	timerCount++;
	if (timerCount > 180)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_TutorialChange);
		mState = STATE_SKILL;
	}
}

// 5. スキル説明：プレイヤーが何らかのランダムスキルを獲得し、発動させた後に次ステップへ
void TutorialScene::UpdateStateSkill()
{
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);

	if (!player) return;

	if (player->mpSkill->AddSkillFlag)
	{
		if(mpTexture5) mpTexture5->Draw();
		mbSkillFlag = true;
	}

	if (player->mpSkill->AddSkillFlag == false && mbSkillFlag)
	{
		mbSkillFlag = false;
		mState = STATE_FEVER;
		Master::mpSoundManager->PlaySE(SoundManager::SE_TutorialChange);
		
		// フィーバー用ゴールド牛の配置
		auto g = new Cow_gold("Resource/3D/GOLDCow/GoldCow.mv1", VGet(1000, 0, 1000), Cow_gold::Nofever);
		g->SetScale(100);
	}
}

// 6. フィーバー説明：ゴールド牛を吸い込み、フィーバーが発動・終了するまで待機
void TutorialScene::UpdateStateFever()
{
	if (mfeverstate == FEVER_1)
	{
		if(mpTexture6) mpTexture6->Draw(); 
		
		if (mpFever && mpFever->IsFever()) { 
			mfeverstate = FEVER_2; 
		}
	}
	else if (mfeverstate == FEVER_2)
	{
		if (mpFever && mpFever->IsFever() == false) { 
			mState = STATE_END; 
		}
	}
}

// 終了ステップ：チュートリアル完了の案内テキストを表示し、ENTERキー押下でタイトルへ戻る
void TutorialScene::UpdateStateEnd()
{
	DrawTutorialText("チュートリアル終了！ ENTERキーを押してタイトルに戻ります。");

	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		mFadeState = SceneFade_Out;
		mNextScene = SceneManager::SCENE_TITLE;
	}
}

void TutorialScene::Draw()
{
	Scene::Draw();
}

void TutorialScene::DrawTutorialText(const char* text, int yOffset)
{
	unsigned int color = GetColor(255, 255, 0);
	DrawFormatStringToHandle(600, 100 + yOffset, color, fontHandle, text);
}

void TutorialScene::Finalize()
{
	Master::mpSoundManager->StopBGM();  
}
