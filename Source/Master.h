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

// ゲーム全体で共有されるマネージャー群やグローバルなゲーム状態フラグを保持するクラス
class Master
{
public:
	static SceneManager* mpSceneManager;      // 画面遷移や現在アクティブなシーンのライフサイクルを管理
	static SoundManager* mpSoundManager;      // BGMやSEの再生・音量制御などを一括管理
	static Score* mpScore;                    // ゲームプレイ中の獲得スコアやランキング情報を保持
	static ResourceManager* mpResourceManager;// 3Dモデルや2D画像などのアセットデータのキャッシュ管理
	static Camera* mpCamera;                  // プレイヤーを追従するメイン3Dカメラ
	static DebugCamera* mpDebugCamera;        // 開発検証用の自由移動カメラ
	static bool mbIsDebugCamera;              // デバッグカメラ有効化フラグ (有効時はメイン入力を遮断)
	static Level* mpLevel;                    // プレイヤーのレベルや経験値管理システムへのポインタ
	static EffectManager* mpEffectManager;    // Effekseerエフェクトの生成と寿命管理を行うマネージャー
	
	static bool SelectSkill;                  // スキル選択画面が表示中かどうかのフラグ
	static int mnTutorialcount;               // チュートリアルにおける対象オブジェクトの回収数カウンター
	static bool GameFinishFlag;               // ゲーム本編が終了（タイムアップ等）したことを示す状態フラグ
	static bool FeverFlag;                    // 大量出現かつ自動吸引が行われるフィーバー状態の有効化フラグ
	static int mnCaughtCowCount;              // 回収された牛の総数 (実績やゲーム状態移行に使用)
	static bool TutrialVacumFlag;             // チュートリアルにおいて吸引操作が完了したかどうかの判定フラグ
};
