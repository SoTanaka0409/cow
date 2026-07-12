#pragma once
#include"SceneManager.h"
#include"SoundManager.h"
#include"ResourceManager.h"
#include<string>
#include<vector>
#include<iostream>
#include"Camera.h"
#include"DebugCamera.h"
#include"Score.h"
#include"Scene.h"
#include"Level.h"
#include"EffectManager.h"
#include"Fever.h"

// シングルトン回避と吁E���Eへのグローバルアクセスを提供するためE��皁E��ラスとして定義
class Master
{
public:
	static SceneManager* mpSceneManager;      // シーン刁E��替えとライフサイクル管琁E��
	static SoundManager* mpSoundManager;      // BGM/SEの一括制御用
	static Score* mpScore;                    // UI表示とリザルト算�Eのためのスコア惁E��
	static ResourceManager* mpResourceManager;// 頻繁なロードによるカクつきを防ぐアセチE��キャチE��ュ
	static Camera* camera_;                  // プレイヤー追従用のメインカメラ
	static DebugCamera* mpDebugCamera;        // チE��チE��時�E自由視点確認用
	static bool mbIsDebugCamera;              // 入力�E断とカメラ操作を刁E��替えるチE��チE��フラグ
	static Level* mpLevel;                    // パラメータスケーリング用レベル惁E��
	static EffectManager* mpEffectManager;    // Effekseerエフェクト�E描画・寿命管琁E��
	
	static bool SelectSkill;                  // スキル選択中のゲーム進行停止用フラグ
	static int mnTutorialcount;               // チュートリアルの進行度判定用カウンター
	static bool GameFinishFlag;               // リザルト移行を制御する終亁E��態フラグ
	static bool FeverFlag;                    // 自動吸引と大量�E現モードを有効にするフラグ
	static int mnCaughtCowCount;              // 実績解除とイベント発生判定用の総回収数
	static bool tutorial_vacum_flag_;             // チュートリアルでの吸引操作達成フラグ

	static float mfDeltaTime;                 // フレームレート変動を吸収するため�E経過時間(私E
	static float GetDeltaTimeScaler() { return mfDeltaTime * 60.0f; } // 60FPS基準�E移動量補正スケール
};