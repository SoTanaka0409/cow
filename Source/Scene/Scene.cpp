#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

/// @brief シーンの基底クラスの初期化
/// @details 各種マネージャーの生成とフェード用変数の初期化を行う
Scene::Scene()
{

	object_manager_ = new ObjectManager();
	collider_manager_ = ColliderManager::GetInstance();
	game_manager_ = new GameManager();
	cow_manager_ = new CowManager();
	animal_manager_ = new AnimalManager();
	fever_ = new Fever();

	fade_alpha_ = 0.0f;
	fade_speed_ = 5.0f;
}

/// @brief リソースの解放
/// @details 動的確保した各マネージャーのメモリを解放する
Scene::~Scene()
{

	if (object_manager_ != nullptr)
	{
		object_manager_->DeleteAll3D();
		object_manager_->DeleteAll2D();
		delete object_manager_;
	}

	if (collider_manager_ != nullptr)
	{
		collider_manager_->DeleteAllCollider();
	}

	if (game_manager_ != nullptr)
	{
		delete game_manager_;
	}
	if (cow_manager_ != nullptr)
	{
		delete cow_manager_;
	}
	if (fever_ != nullptr)
	{
		delete fever_;
	}
	if (animal_manager_ != nullptr)
	{
		delete animal_manager_;
	}
}

/// @brief 画面への描画処理
/// @details 管理中のオブジェクトや当たり判定を描画バッファに登録する
void Scene::Draw()
{
	if (object_manager_ != nullptr)
	{
		object_manager_->Draw();
	}
	if (collider_manager_ != nullptr)
	{
		collider_manager_->Draw();
	}
	if (animal_manager_ != nullptr)
	{
		animal_manager_->Draw();
	}
}

/// @brief シーンの状態更新
/// @details 管理中オブジェクトの更新と、フェードアウト完了時のシーン遷移を行う
void Scene::Update()
{
	if (object_manager_ != nullptr)
	{
		object_manager_->Update();
	}
	if (collider_manager_ != nullptr)
	{
		collider_manager_->Update();
	}
	if (fever_ != nullptr)
	{
		fever_->Update();
	}
	if (animal_manager_ != nullptr)
	{
		animal_manager_->Update();
	}

	if (fade_state_ == kSceneFadeOut)
	{
		Master::sound_manager_->SetBGMVolume((Master::sound_manager_->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::scene_manager_->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
		}
	}
}

/// @brief 画面の暗転表現
/// @param fade 進行するフェード状態
/// @details ブレンドモードを変更し黒い矩形を描画する
void Scene::Fade(SceneFade fade)
{
	if (fade == SceneFade::kSceneFadeIn)
	{
		fade_alpha_ -= fade_speed_;
		if (fade_alpha_ < 0) fade_alpha_ = 0;

		if (fade_alpha_ > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fade_alpha_);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else if (fade == SceneFade::kSceneFadeOut)
	{
		fade_alpha_ += fade_speed_;
		if (fade_alpha_ > 255) fade_alpha_ = 255;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fade_alpha_);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else if (fade == SceneFade::kSceneFadeLoad)
	{
	}
}
