#include "LoadingScene.h"
#include "DxLib.h"
#include "Master.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameConstants.h"

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 内部タイマーとフラグの初期化
 */
LoadingScene::LoadingScene()
	: loading_timer_(0)
	, load_started_(false)
{
}

LoadingScene::~LoadingScene()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 内部状態のリセット
 */
void LoadingScene::Initialize()
{
	loading_timer_ = 0;
	load_started_ = false;
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 非同期ロードのキックおよびシーン遷移指示
 */
void LoadingScene::Update()
{
	Scene::Update();

	loading_timer_++;

	// ロード画面を描画する前に重い処理が走りフリーズするのを防ぐことと、DxLibの非同期ロード関数が
	// 開始直後に誤って未完了数0を返すバグを回避するため、2フレーム目以降に処理を遅延させる
	if (!load_started_ && loading_timer_ >= 2)
	{
		SetUseASyncLoadFlag(TRUE);

		Master::mpResourceManager->PreloadModel(GameConstants::kCowDefault.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kCowGold.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kAnimalChicken.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kAnimalBear.model_path);
		Master::mpResourceManager->PreloadModel(GameConstants::kAnimalSheep.model_path);
		Master::mpResourceManager->PreloadModel("Resource/3D/Player/UFO.mv1");
		Master::mpResourceManager->PreloadModel("Resource/3D/Fence1/Fence1/Fence.mv1");
		Master::mpResourceManager->PreloadModel("Resource/3D/SkyBox/SkyBox.mv1");

		SetUseASyncLoadFlag(FALSE);

		load_started_ = true;
	}

	// ロードが即座に終了した際に画面が一瞬だけ暗転してチラつく(3D酔いや不快感に繋がる)のを防ぐため、
	// 最低でも60フレーム(1秒間)は強制的にローディング画面を維持する
	if (load_started_ && GetASyncLoadNum() == 0 && loading_timer_ > 60)
	{
		// 外部仕様依存: このクラス単体で遷移処理を完結させず、SceneManagerにライフサイクルを委譲する
		Master::mpSceneManager->SetNextScene(SceneManager::kScene3D);
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ローディング画面のUI描画
 */
void LoadingScene::Draw()
{
	DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);

	// 非同期ロードの裏でアプリケーションがフリーズ(ハングアップ)していないことをユーザーに視覚的に保証するため、
	// 固定周期で点滅するアニメーションを描画する
	if ((loading_timer_ / 20) % 2 == 0)
	{
		DrawString(1600 / 2 - 60, 900 / 2, "NOW LOADING...", GetColor(255, 255, 255));
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
void LoadingScene::Finalize()
{
}
