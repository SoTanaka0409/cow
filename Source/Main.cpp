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
Camera* Master::mpCamera = new Camera();
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
bool Master::TutrialVacumFlag = false;
bool Master::FeverFlag = false;
float Master::mfDeltaTime = 0.01666f;

VECTOR Utility::StageSize= VGet(6000, 0, 6000); // 3D空間におけるステージ全体の広さ境界設定

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

	Master::mpScore = new Score(); // DxLib初期化後にインスタンス化して画像読み込みを正常に行う

	SRand(GetNowCount());

	SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));
	
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	Master::mpSoundManager->Initialize(); // 起動時のBGMやSEなどすべての音源データをプリロード

	// 最初のシーン（通常はタイトル画面）の準備処理を実行
	Master::mpSceneManager->Initialize();
	
	// メインカメラおよびデバッグ用カメラの初期状態を設定
	Master::mpCamera->Initialize();
	Master::mpDebugCamera->Initialize();

	Master::mpEffectManager->Initalize();

	int previousTime = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 描画バッファをリセットし、今フレームの新規描画に備える
		ClearDrawScreen();
		int time = GetNowCount();

		Master::mfDeltaTime = (time - previousTime) / 1000.0f;
		if (Master::mfDeltaTime > 0.1f) Master::mfDeltaTime = 0.1f;
		previousTime = time;

		// F1キー押下で開発用のデバッグカメラモードへ切り替え
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
			Master::mpCamera->Update();
		}

		Master::mpEffectManager->Update();

		Master::mpSceneManager->Update();
		
		Master::mpSceneManager->Draw();
		
		Master::mpEffectManager->Draw();

		// ダブルバッファリング：バックバッファの描画内容をフロント画面に反映
		ScreenFlip();

		// 60FPSを維持するため、前フレームからの経過時間が17ms未満の場合は待機
		while (GetNowCount() - time < 17)
		{
		}

		// 削除フラグが立っているコライダーと3Dオブジェクトをメモリから解放
		ServiceLocator::GetCurrentScene()->GetCollisionManager()->DeleteAllColliderIfNeeded();
		ServiceLocator::GetObjectManager()->DeleteAll3DIfNeeded();

		// 処理の区切り目として、次フレーム開始前のシーン遷移要求を処理
		Master::mpSceneManager->ChangeSceneIfNeeded();
	}
	
	// ゲームループ終了に伴う各グローバルマネージャーの破棄処理
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;
	Master::mpCamera->Finalize();
	delete Master::mpCamera;
	delete Master::mpDebugCamera;
	delete Master::mpResourceManager;

	ColliderManager::GetInstance()->Finalize();
	
	Effkseer_End();

	delete Master::mpScore;

	DxLib_End(); // DxLibシステム全体のクローズ処理

	return 0;
}

