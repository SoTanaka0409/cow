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

// 仕様制約：プレイヤーや牛がステージ外へ飛び出さないよう、侵入制限をかけるための境界値
VECTOR Utility::StageSize = VGet(6000, 0, 6000);

// 入力：Windowsアプリケーション起動用の基本インスタンス情報
// 出力：正常終了時は0、エラー時は-1
// 副作用：ウィンドウ生成、各種マネージャー初期化、メインループ起動、メモリ解放
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 複数端末でのテストやデバッグ効率化のため、フルスクリーンではなくウィンドウモードで起動する
	ChangeWindowMode(true);

	SetGraphMode(Utility::kScreenWidth, Utility::kScreenHeight, 32, 60);
	SetWindowSize(Utility::kScreenWidth, Utility::kScreenHeight);

	// 前回の不正終了時に内部ミューテックスが残り、プロセスが即落ちするバグを回避する設定
	SetDoubleStartValidFlag(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 仕様制約：内部でテクスチャ読み込みを行うため、DxLibの初期化が完了した後にインスタンス化する
	Master::mpScore = new Score();

	SRand(GetNowCount());

	SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	// パフォーマンス理由：ゲーム中のロード遅延によるスパイクを防ぐため、初期化時に全SEをプリロードする
	Master::mpSoundManager->Initialize();

	Master::mpSceneManager->Initialize();

	Master::camera_->Initialize();
	Master::mpDebugCamera->Initialize();

	Master::mpEffectManager->Initalize();

	int previousTime = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		int time = GetNowCount();

		// バグ回避：アプリの最小化や一時的なフリーズでデルタタイムが異常値になり、挙動が破綻するのを防ぐ上限設定
		Master::mfDeltaTime = (time - previousTime) / 1000.0f;
		if (Master::mfDeltaTime > 0.1f) Master::mfDeltaTime = 0.1f;
		previousTime = time;

		// 開発効率化のため、F1キーのトグル入力でプレイヤー視点と自由カメラ視点を切り替え可能にする
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::mbIsDebugCamera = !Master::mbIsDebugCamera;
			if (Master::mbIsDebugCamera) {
				Master::mpDebugCamera->Initialize();
			}
		}

		if (Master::mbIsDebugCamera) {
			Master::mpDebugCamera->Update();
		}
		else {
			Master::camera_->Update();
		}

		Master::mpEffectManager->Update();

		Master::mpSceneManager->Update();

		Master::mpSceneManager->Draw();

		Master::mpEffectManager->Draw();

		ScreenFlip();

		// パフォーマンス理由：CPUのハイパートレッディングによる過負荷を防ぎ、60FPS（約16.6ms）を維持する待機処理
		while (GetNowCount() - time < 17)
		{
		}

		// メモリリーク回避：削除フラグが立ったオブジェクトと当たり判定を、フレーム終了時に安全に一括解放する
		if (auto scene = ServiceLocator::GetCurrentScene())
		{
			scene->GetCollisionManager()->DeleteAllColliderIfNeeded();
		}
		if (auto objMgr = ServiceLocator::GetObjectManager())
		{
			objMgr->DeleteAll3DIfNeeded();
		}

		// バグ回避：UpdateやDrawの処理途中で解放が走るのを防ぐため、必ずフレームの最後でシーン切り替えを行う
		Master::mpSceneManager->ChangeSceneIfNeeded();
	}

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

	DxLib_End();

	return 0;
}