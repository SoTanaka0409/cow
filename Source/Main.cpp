#include "ServiceLocator.h"
#include "DxLib.h"
#include"Master.h"
#include"Camera.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"ResourceManager.h"
#include"SoundManager.h"
#include"ColliderManager.h"
#include "Utility.h"
#include"Score.h"
#include"EffectManager.h"
#include"Fever.h"
#include"InputManager.h"
#include <EffekseerForDXLib.h>

SceneManager* Master::sceneManager = new SceneManager();
Camera* Master::camera = new Camera();
DebugCamera* Master::debugCamera = new DebugCamera();
bool Master::isDebugCamera = false;
ResourceManager* Master::resourceManager = new ResourceManager();
SoundManager* Master::soundManager = new SoundManager();

EffectManager* Master::effectManager = new EffectManager();

Score* Master::score = nullptr;
bool Master::selectSkill = false;
int Master::tutorialCount = 0;
bool Master::gameFinishFlag = false;
int Master::caughtCowCount = 0;
bool Master::tutorialVacuumFlag = false;
bool Master::feverFlag = false;
float Master::deltaTime = 0.01666f;

VECTOR Utility::StageSize= VGet(6000, 0, 6000); // 3D空間の境界制約としてステージサイズを定義

/*
 * @brief アプリケーションのエントリーポイント
 * [入力] hInstance, hPrevInstance, lpCmdLine, nCmdShow
 * [出力] 終了コード (正常終了時は0, エラー時は-1)
 * [副作用] ゲームウィンドウの起動、システム初期化、メインループ実行、全リソースの解放
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// デバッグや他作業の並行を容易にするためウィンドウモードで起動
	ChangeWindowMode(true);

	SetGraphMode(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, 32, 60);
	SetWindowSize(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT);
	
	// DxLibの初期化エラー時は実行継続不可のため即時終了
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	Master::score = new Score(); // DxLib初期化前だと画像読み込みが失敗する制約があるためここで生成

	SRand(GetNowCount());

	SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));
	
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	Master::soundManager->Initialize(); // 再生遅延を防ぐため全音源データをプリロードする

	// 初期シーンを構築する
	Master::sceneManager->Initialize();
	
	// 描画用の各カメラを初期化する
	Master::camera->Initialize();
	Master::debugCamera->Initialize();

	Master::effectManager->Initalize();

	int previousTime = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 前フレームの描画をクリアして新規描画の準備
		ClearDrawScreen();
		int time = GetNowCount();

		Master::deltaTime = (time - previousTime) / 1000.0f;
		if (Master::deltaTime > 0.1f) Master::deltaTime = 0.1f;
		previousTime = time;

		// 開発効率化のためF1キーでデバッグカメラをトグル
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::isDebugCamera = !Master::isDebugCamera;
			if (Master::isDebugCamera) {
				Master::debugCamera->Initialize();
			}
		}

		if (Master::isDebugCamera) {
			Master::debugCamera->Update();
		} else {
			Master::camera->Update();
		}

		Master::effectManager->Update();

		Master::sceneManager->Update();
		
		Master::sceneManager->Draw();
		
		Master::effectManager->Draw();

		// 描画のちらつきを防ぐためフリップする
		ScreenFlip();

		// 固定フレームレート(60FPS)維持のため待機
		while (GetNowCount() - time < 17)
		{
		}

		// メモリリークを防ぐため破棄要求のあるオブジェクトを解放
		ServiceLocator::GetCurrentScene()->GetCollisionManager()->DeleteAllColliderIfNeeded();
		ServiceLocator::GetObjectManager()->DeleteAll3DIfNeeded();

		// フレーム終了時に安全にシーン遷移を行う
		Master::sceneManager->ChangeSceneIfNeeded();
	}
	// アプリケーション終了に伴うリソース解放
	Master::sceneManager->Finalize();
	delete Master::sceneManager;
	Master::soundManager->Finalize();
	delete Master::soundManager;
	Master::camera->Finalize();
	delete Master::camera;
	delete Master::debugCamera;
	delete Master::resourceManager;

	ColliderManager::GetInstance()->Finalize();
	
	Effkseer_End();

	delete Master::score;

	DxLib_End(); // DxLibの内部リソースを解放

	return 0;
}

