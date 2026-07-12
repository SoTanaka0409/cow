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

SceneManager* Master::mpSceneManager = new SceneManager();
Camera* Master::camera_ = new Camera();
DebugCamera* Master::mpDebugCamera = new DebugCamera();
bool Master::mbIsDebugCamera = false;
ResourceManager* Master::mpResourceManager = new ResourceManager();
SoundManager* Master::mpSoundManager = new SoundManager();

EffectManager* Master::mpEffectManager = new EffectManager();

Score* Master::mpScore = nullptr;
bool Master::SelectSkill = false;
int Master::mnTutorialcount = 0;
bool Master::GameFinishFlag = false;
int Master::mnCaughtCowCount = 0;
bool Master::tutorial_vacum_flag_ = false;
bool Master::FeverFlag = false;
float Master::mfDeltaTime = 0.01666f;

VECTOR Utility::StageSize= VGet(6000, 0, 6000); // 3D空間?E?E??制?E??してスチE?Eジサイズを定義

/*
 * @brief アプリケーションのエントリーポインチE
 * [入力] hInstance, hPrevInstance, lpCmdLine, nCmdShow
 * [出力] 終?E??ーチE(正常終?E??は0, エラー時?E-1)
 * [副作用] ゲームウィンドウの起動、シスチE??初期化、メインループ実行、?Eリソースの解放
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// チEチEめE作業の並行を容易にするためウィンドウモードで起勁E
	ChangeWindowMode(true);

	SetGraphMode(Utility::kScreenWidth, Utility::kScreenHeight, 32, 60);
	SetWindowSize(Utility::kScreenWidth, Utility::kScreenHeight);
	
	SetDoubleStartValidFlag(TRUE); // 二重起動を許可する（不正なミューテックス残存による即落ちを防ぐため）

	// DxLibの初期化エラー時E実行継続不可のため即時終?E
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	Master::mpScore = new Score(); // DxLib初期化前だと画像読み込みが失敗する制?Eあるためここで生E

	SRand(GetNowCount());

	SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));
	
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	Master::mpSoundManager->Initialize(); // 再生遁E??を防ぐため?E音源データを?EリロードすめE

	// 初期シーンを構築すめE
	Master::mpSceneManager->Initialize();
	
	// 描画用の吁E??メラを?E期化する
	Master::camera_->Initialize();
	Master::mpDebugCamera->Initialize();

	Master::mpEffectManager->Initalize();

	int previousTime = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 前フレームの描画をクリアして新規描画の準備
		ClearDrawScreen();
		int time = GetNowCount();

		Master::mfDeltaTime = (time - previousTime) / 1000.0f;
		if (Master::mfDeltaTime > 0.1f) Master::mfDeltaTime = 0.1f;
		previousTime = time;

		// 開発効?E??のためF1キーでチE??チE??カメラをトグル
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::mbIsDebugCamera = !Master::mbIsDebugCamera;
			if (Master::mbIsDebugCamera) {
				Master::mpDebugCamera->Initialize();
			}
		}

		if (Master::mbIsDebugCamera) {
			Master::mpDebugCamera->Update();
		} else {
			Master::camera_->Update();
		}

		Master::mpEffectManager->Update();

		Master::mpSceneManager->Update();
		
		Master::mpSceneManager->Draw();
		
		Master::mpEffectManager->Draw();

		// 描画のちらつきを防ぐためフリチE?Eする
		ScreenFlip();

		// 固定フレームレーチE60FPS)維持?Eため征E??E
		while (GetNowCount() - time < 17)
		{
		}

		// メモリリークを防ぐため破?E??求?Eあるオブジェクトを解放
		if (auto scene = ServiceLocator::GetCurrentScene())
		{
			scene->GetCollisionManager()->DeleteAllColliderIfNeeded();
		}
		if (auto objMgr = ServiceLocator::GetObjectManager())
		{
			objMgr->DeleteAll3DIfNeeded();
		}

		// フレーム終?E??に安?Eにシーン遷移を行う
		Master::mpSceneManager->ChangeSceneIfNeeded();
	}
	// アプリケーション終?E??伴ぁE??ソース解放
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;
	Master::camera_->Finalize();
	delete Master::camera_;
	delete Master::mpDebugCamera;
	delete Master::mpResourceManager;

	ColliderManager::GetInstance()->Finalize();
	
	Effkseer_End();

	delete Master::mpScore;

	DxLib_End(); // DxLibの?E??リソースを解放

	return 0;
}
