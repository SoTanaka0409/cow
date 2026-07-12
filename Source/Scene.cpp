#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

Scene::Scene()
{

	object_manager_ = new ObjectManager();
	collider_manager_ = new ColliderManager();
	game_manager_ = new GameManager();
	cow_manager_ = new CowManager();
	animal_manager_ = new AnimalManager();
	fever_ = new Fever();


	fade_alpha_ = 0.0f;
	fade_speed_ = 5.0f;
}

Scene::~Scene()
{

	if (object_manager_ != nullptr)
	{
		object_manager_->DeleteAll3D();
		object_manager_->DeleteAll2D(); // シーン?E??替え時のメモリリーク防止
		delete object_manager_;
	}


	if (collider_manager_ != nullptr)
	{
		collider_manager_->DeleteAllCollider();
		delete collider_manager_;
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

void Scene::Draw()
{
	if (object_manager_ != nullptr)
	{
		object_manager_->Draw();
	}
	if (collider_manager_ != nullptr)
	{
		collider_manager_->Draw(); // チE??チE??用の当たり判定可視化
	}
	if (animal_manager_ != nullptr)
	{
		animal_manager_->Draw();
	}
}

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
}

void Scene::Fade(SceneFade fade)
{
	// フェードイン
	if (fade == SceneFade::kSceneFadeIn)
	{

		fade_alpha_ -= fade_speed_;
		if (fade_alpha_ < 0) fade_alpha_ = 0; 


		if (fade_alpha_ > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fade_alpha_);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); // 画面全体を暗転
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
		}
	}
	// フェードアウチE
	else if (fade == SceneFade::kSceneFadeOut)
	{

		fade_alpha_ += fade_speed_;
		if (fade_alpha_ > 255) fade_alpha_ = 255; // DxLibの仕様によりアルファ値上限は255


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fade_alpha_);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          
	}
	// ロード中
	else if (fade == SceneFade::kSceneFadeLoad)
	{
		// 封E??のロード画面拡張用スペ?Eス
	}
}
