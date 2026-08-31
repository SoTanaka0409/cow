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

SceneManager* Master::scene_manager_ = new SceneManager();
Camera* Master::camera_ = new Camera();
DebugCamera* Master::debug_camera_ = new DebugCamera();
bool Master::is_debug_camera_ = false;
ResourceManager* Master::resource_manager_ = new ResourceManager();
SoundManager* Master::sound_manager_ = new SoundManager();

EffectManager* Master::effect_manager_ = new EffectManager();

Score* Master::score_manager_ = nullptr;
bool Master::SelectSkill = false;
int Master::tutorial_count_ = 0;
bool Master::GameFinishFlag = false;
int Master::caught_cow_count_ = 0;
bool Master::tutorial_vacum_flag_ = false;
bool Master::FeverFlag = false;
float Master::delta_time_ = 0.01666f;
bool Master::is_shadow_enabled_ = false; // デフォルトは影OFF
bool Master::is_debug_mode_ = false;    // デフォルトはデバッグモードOFF

/// @brief 仕様制約：プレイヤーや牛がステージ外へ飛び出さないよう、侵入制限をかけるための境界値
VECTOR Utility::StageSize = VGet(4000, 0, 4000);


/// @return 正常終了時は0、エラー時は-1
/// @brief Windowsアプリケーション起動用の基本インスタンス情報
/// @details ウィンドウ生成、各種マネージャー初期化、メインループ起動、メモリ解放
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 複数端末でのテストやデバッグ効率化のため、フルスクリーンではなくウィンドウモードで起動する
	ChangeWindowMode(TRUE);
	SetOutApplicationLogValidFlag(FALSE); // Disable Log.txt generation

	SetGraphMode(Utility::kScreenWidth, Utility::kScreenHeight, 32, 60);
	SetWindowSize(Utility::kScreenWidth, Utility::kScreenHeight);

	// 前回の不正終了時に内部ミューテックスが残り、プロセスが即落ちするバグを回避する設定
	SetDoubleStartValidFlag(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 仕様制約：内部でテクスチャ読み込みを行うため、DxLibの初期化が完了した後にインスタンス化する
	Master::score_manager_ = new Score();

	SRand(GetNowCount());

	SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	// ゲーム中のロード遅延によるスパイクを防ぐため、初期化時に全SEをプリロードする
	Master::sound_manager_->Initialize();

	Master::scene_manager_->Initialize();

	Master::camera_->Initialize();
	Master::debug_camera_->Initialize();

	Master::effect_manager_->Initalize();

	int previousTime = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		int time = GetNowCount();

		// アプリの最小化や一時的なフリーズでデルタタイムが異常値になり、挙動が破綻するのを防ぐ上限設定
		Master::delta_time_ = (time - previousTime) / 1000.0f;
		if (Master::delta_time_ > 0.1f) Master::delta_time_ = 0.1f;
		previousTime = time;

		// 開発効率化のため、F1キーのトグル入力でプレイヤー視点と自由カメラ視点を切り替え可能にする
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::is_debug_mode_ = !Master::is_debug_mode_;
			if (!Master::is_debug_mode_)
			{
				Master::is_debug_camera_ = false;
			}
		}

		if (Master::is_debug_mode_ && InputManager::CheckDownKey(KEY_INPUT_F2))
		{
			Master::is_debug_camera_ = !Master::is_debug_camera_;
			if (Master::is_debug_camera_)
			{
				Master::debug_camera_->Initialize();
			}
		}

		if (Master::is_debug_camera_)
		{
			Master::debug_camera_->Update();
		}
		else
		{
			Master::camera_->Update();
		}

		Master::effect_manager_->Update();

		Master::scene_manager_->Update();

		Master::scene_manager_->Draw();

		Master::effect_manager_->Draw();

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

		// UpdateやDrawの処理途中で解放が走るのを防ぐため、必ずフレームの最後でシーン切り替えを行う
		Master::scene_manager_->ChangeSceneIfNeeded();
	}

	Master::scene_manager_->Finalize();
	delete Master::scene_manager_;
	Master::sound_manager_->Finalize();
	delete Master::sound_manager_;
	Master::camera_->Finalize();
	delete Master::camera_;
	delete Master::debug_camera_;
	delete Master::resource_manager_;

	ColliderManager::GetInstance()->Finalize();

	Effkseer_End();

	delete Master::score_manager_;

	DxLib_End();

	return 0;
}
