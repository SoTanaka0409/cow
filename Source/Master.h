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

// シングルトン回避と各機能へのグローバルアクセスを提供するため静的クラスとして定義
class Master
{
public:
	static SceneManager* sceneManager;      // シーン切り替えとライフサイクル管理用
	static SoundManager* soundManager;      // BGM/SEの一括制御用
	static Score* score;                    // UI表示とリザルト算出のためのスコア情報
	static ResourceManager* resourceManager;// 頻繁なロードによるカクつきを防ぐアセットキャッシュ
	static Camera* camera;                  // プレイヤー追従用のメインカメラ
	static DebugCamera* debugCamera;        // デバッグ時の自由視点確認用
	static bool isDebugCamera;              // 入力遮断とカメラ操作を切り替えるデバッグフラグ
	static Level* level;                    // パラメータスケーリング用レベル情報
	static EffectManager* effectManager;    // Effekseerエフェクトの描画・寿命管理用
	
	static bool selectSkill;                  // スキル選択中のゲーム進行停止用フラグ
	static int tutorialCount;               // チュートリアルの進行度判定用カウンター
	static bool gameFinishFlag;               // リザルト移行を制御する終了状態フラグ
	static bool feverFlag;                    // 自動吸引と大量出現モードを有効にするフラグ
	static int caughtCowCount;              // 実績解除とイベント発生判定用の総回収数
	static bool tutorialVacuumFlag;             // チュートリアルでの吸引操作達成フラグ

	static float deltaTime;                 // フレームレート変動を吸収するための経過時間(秒)
	static float GetDeltaTimeScaler() { return deltaTime * 60.0f; } // 60FPS基準の移動量補正スケール
};
