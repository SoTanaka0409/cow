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
#include "ColliderManager.h"
#include "CowMove.h"
#include "Texture.h"
#include "GoldCow.h"

TutorialScene::TutorialScene()
	: Scene()
{
	// CowManager
	cow_delete_ = false;
	fever_state_ = kFever1;
	timer_count_ = 0;
}

TutorialScene::~TutorialScene()
{
	DeleteFontToHandle(font_handle_);

	if (texture_)
	{
		delete texture_;
		texture_ = nullptr;
	}
	if (texture2_)
	{
		delete texture2_;
		texture2_ = nullptr;
	}
	if (texture3_)
	{
		delete texture3_;
		texture3_ = nullptr;
	}
	if (texture4_)
	{
		delete texture4_;
		texture4_ = nullptr;
	}
	if (texture5_)
	{
		delete texture5_;
		texture5_ = nullptr;
	}
	if (texture6_)
	{
		delete texture6_;
		texture6_ = nullptr;
	}

	// CowManagerの生成と破棄は継承クラスSceneにて行うためここでは行わない
}

void TutorialScene::Initialize()
{
	Master::camera_->Initialize();

	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	font_handle_ = CreateFontToHandle("メイリオ", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);

	CreateFences();
	CreateWalls();

	new Floor(
		"Resource/2D/InGame/GameBackground.png",
		VGet(0, 0, 0),
		VGet(-11500, 0, -11500),
		VGet(11500, 0, 11500)
	);

	Master::sound_manager_->PlayBGM(SoundManager::kBgmTutorial);
	auto Player = new Player3D("Resource/3D/Character/Player/PlayerUFO.mv1", VGet(0.0f, 1000.0f, 0.0f));
	Player->SetScale(0.6f);

	SetCamera(Master::camera_);

	VECTOR pos = VGet(1200, 150, 0);
	int GraphSize_x = 800, GraphSize_y = 300;
	texture_  = new Texture("Resource/2D/Tutorial/Tutorial__d07a.png", pos, GraphSize_x, GraphSize_y, true);
	texture2_ = new Texture("Resource/2D/Tutorial/Tutorial_Vacuum.png", pos, GraphSize_x, GraphSize_y, true);
	texture3_ = new Texture("Resource/2D/Tutorial/Tutorial_Combo.png", pos, GraphSize_x, GraphSize_y, true);
	texture4_ = new Texture("Resource/2D/Tutorial/Tutorial_Phase.png", pos, GraphSize_x, GraphSize_y, true);
	texture5_ = new Texture("Resource/2D/Tutorial/Tutorial_Skill.png", pos, GraphSize_x, GraphSize_y, true);
	texture6_ = new Texture("Resource/2D/Tutorial/Tutorial__e727.png", pos, GraphSize_x, GraphSize_y, true);
}

void TutorialScene::CreateFences()
{
	for (int i = 0; i < 5; ++i)
	{
		float z = 5050.0f;
		float x = 500.0f + (i * 1000.0f);
		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(x, -25.0f, z), 2.5f, VGet(0.0f, 0.0f, 0.0f));
		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(-x, -25.0f, z), 2.5f, VGet(0.0f, 0.0f, 0.0f));

		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(x, -25.0f, -z), 2.5f, VGet(0.0f, DX_PI_F, 0.0f));
		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(-x, -25.0f, -z), 2.5f, VGet(0.0f, DX_PI_F, 0.0f));
	}

	for (int i = 0; i < 5; ++i)
	{
		float x = 5050.0f;
		float z = 500.0f + (i * 1000.0f);
		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(x, -25.0f, z), 2.5f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(x, -25.0f, -z), 2.5f, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));

		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(-x, -25.0f, z), 2.5f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
		new Object_Stage("Resource/3D/Stage/Fence/Fence.mv1", VGet(-x, -25.0f, -z), 2.5f, VGet(0.0f, -DX_PI_F / 2.0f, 0.0f));
	}
}

void TutorialScene::CreateWalls()
{
	new Wall("", VGet(0.0f, 0.0f, 5000.0f), VGet(-5000.0f, 5000.0f, 0.0f), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("", VGet(0.0f, 0.0f, -5000.0f), VGet(-5000.0f, 5000.0f, 0.0), VGet(5000.0f, 0.0f, 0.0f));
	new Wall("", VGet(5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
	new Wall("", VGet(-5000.0f, 0.0f, 0.0f), VGet(0.0f, 5000.0f, 5000.0f), VGet(0.0f, 0.0f, -5000.0f));
}

void TutorialScene::SetCamera(Camera* camera)
{
	camera_ = camera;
}

void TutorialScene::Update()
{
	Scene::Update();

	if (camera_ != nullptr)
	{
		camera_->Update();
	}
	if (cow_manager_ != nullptr)
	{
		cow_manager_->Update();
	}

	switch (state_)
	{
	case kStateMove:
		UpdateStateMove();
		break;
	case kStateBeam:
		UpdateStateBeam();
		break;
	case kStateComboScore:
		UpdateStateComboScore();
		break;
	case kStatePhase:
		UpdateStatePhase();
		break;
	case kStateSkill:
		UpdateStateSkill();
		break;
	case kStateFever:
		UpdateStateFever();
		break;
	case kStateEnd:
		UpdateStateEnd();
		break;
	}

	// チュートリアル終了、ENTERキー入力でタイトル画面へ遷移
	if(InputManager::CheckDownKey(KEY_INPUT_RETURN) && state_ != kStateEnd)
	{
		fade_state_ = kSceneFadeOut;
		next_scene_ = SceneManager::kSceneTitle;
	}

	Scene::Update();

}

/// @brief 移動操作（WASD）を確認し、次のステップへ進行する
void TutorialScene::UpdateStateMove()
{
	if (texture_) texture_->Draw();

	if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_D))
	{
		Master::sound_manager_->PlaySE(SoundManager::kSeTutorialChange);
		state_ = kStateBeam;

		VECTOR spawnPos = VGet(0, 0.0f, 150.0f);
		cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, spawnPos, 50.0f, CowMove::kCowT, 1);
	}
}

/// @brief マウス左クリックでビームを照射することを確認
void TutorialScene::UpdateStateBeam()
{
	if (texture2_) texture2_->Draw();

	if (GetMouseInput() & MOUSE_INPUT_LEFT)
	{
		Master::sound_manager_->PlaySE(SoundManager::kSeTutorialChange);
		state_ = kStateComboScore;

		// コンボ稼ぎ用として牛の群れをスポーン
		cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, VGet(150.0f, 0.0f, 150.0f), 50.0f, CowMove::kCowT, 2);
		cow_manager_->SpawnCow(GameConstants::kCowDefault.model_path, VGet(-150.0f, 0.0f, 150.0f), 50.0f, CowMove::kCowT, 2);
	}
}

/// @brief プレイヤーのコンボが2以上になったことを確認して次に進行
void TutorialScene::UpdateStateComboScore()
{
	if (texture3_) texture3_->Draw();

	Player3D* player = ServiceLocator::GetPlayer();

	if (player && player->combo_->GetCombo() >= 2)
	{
		Master::sound_manager_->PlaySE(SoundManager::kSeTutorialChange);
		state_ = kStatePhase;
		timer_count_ = 0;
	}
}

/// @brief フェーズ移行のため、待機（3秒）時間を設けて進行する
void TutorialScene::UpdateStatePhase()
{
	if (texture4_) texture4_->Draw();

	timer_count_++;
	if (timer_count_ > 180)
	{
		Master::sound_manager_->PlaySE(SoundManager::kSeTutorialChange);
		state_ = kStateSkill;
	}
}

/// @brief スキルを使用することを確認し、フィーバーの導線へ移行する
void TutorialScene::UpdateStateSkill()
{
	Player3D* player = ServiceLocator::GetPlayer();

	if (!player) return;

	if (player->skill_->add_skill_flag_)
	{
		if(texture5_) texture5_->Draw();
		skill_flag_ = true;
	}

	if (player->skill_->add_skill_flag_ == false && skill_flag_)
	{
		skill_flag_ = false;
		state_ = kStateFever;
		Master::sound_manager_->PlaySE(SoundManager::kSeTutorialChange);

		// フィーバーモード用でゴールド牛を召喚
		auto g = new GoldCow("Resource/3D/Character/Cow/Cow_f6cf.mv1", VGet(1000, 0, 1000), GoldCow::kNoFever);
		g->SetScale(100);
	}
}

/// @brief フィーバーモードの開始と終了を確認する
void TutorialScene::UpdateStateFever()
{
	if (fever_state_ == kFever1)
	{
		if(texture6_) texture6_->Draw();

		if (fever_ && fever_->IsFever())
		{
			fever_state_ = kFever2;
		}
	}
	else if (fever_state_ == kFever2)
	{
		if (fever_ && fever_->IsFever() == false)
		{
			state_ = kStateEnd;
		}
	}
}

/// @brief チュートリアル終了テキストを表示して待機状態にする
void TutorialScene::UpdateStateEnd()
{
	DrawTutorialText("Enterでチュートリアルを終了します");

	if(CheckHitKey(KEY_INPUT_RETURN))
	{
		fade_state_ = kSceneFadeOut;
		next_scene_ = SceneManager::kSceneTitle;
	}
}

void TutorialScene::Draw()
{
	Scene::Draw();
	ColliderManager::GetInstance()->Draw();
}

void TutorialScene::DrawTutorialText(const char* text, int yOffset)
{
	unsigned int color = GetColor(255, 255, 0);
	DrawFormatStringToHandle(400, 50 + yOffset, color, font_handle_, text);
}

void TutorialScene::Finalize()
{
	Master::sound_manager_->StopBGM();
}



