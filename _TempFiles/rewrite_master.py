import codecs

content = \"\"\"#pragma once
#include\"SceneManager.h\"
#include\"SoundManager.h\"
#include\"ResourceManager.h\"
#include<string>
#include<vector>
#include<iostream>
#include\"Camera.h\"
#include\"DebugCamera.h\"
#include\"Score.h\"
#include\"Scene.h\"
#include\"Level.h\"
#include\"EffectManager.h\"
#include\"Fever.h\"

// シングルトン回避と各機能へのグローバルアクセスを提供するため静的クラスとして定義
class Master
{
public:
	static SceneManager* mpSceneManager;      // シーン切り替えとライフサイクル管理用
	static SoundManager* mpSoundManager;      // BGM/SEの一括制御用
	static Score* mpScore;                    // UI表示とリザルト算出のためのスコア情報
	static ResourceManager* mpResourceManager;// 頻繁なロードによるカクつきを防ぐアセットキャッシュ
	static Camera* mpCamera;                  // プレイヤー追従用のメインカメラ
	static DebugCamera* mpDebugCamera;        // デバッグ時の自由視点確認用
	static bool mbIsDebugCamera;              // 入力遮断とカメラ操作を切り替えるデバッグフラグ
	static Level* mpLevel;                    // パラメータスケーリング用レベル情報
	static EffectManager* mpEffectManager;    // Effekseerエフェクトの描画・寿命管理用
	
	static bool SelectSkill;                  // スキル選択中のゲーム進行停止用フラグ
	static int mnTutorialcount;               // チュートリアルの進行度判定用カウンター
	static bool GameFinishFlag;               // リザルト移行を制御する終了状態フラグ
	static bool FeverFlag;                    // 自動吸引と大量出現モードを有効にするフラグ
	static int mnCaughtCowCount;              // 実績解除とイベント発生判定用の総回収数
	static bool TutrialVacumFlag;             // チュートリアルでの吸引操作達成フラグ

	static float mfDeltaTime;                 // フレームレート変動を吸収するための経過時間(秒)
	static float GetDeltaTimeScaler() { return mfDeltaTime * 60.0f; } // 60FPS基準の移動量補正スケール
};
\"\"\"

with codecs.open('Source/Master.h', 'w', 'utf-8-sig') as f:
    f.write(content)
print('Master.h done')
