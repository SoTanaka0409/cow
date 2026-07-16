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

// 循環参照や初期化順序バグの温床になりやすいため、各機能の結合度を下げる恒久対応までの暫定的なグローバルアクセス（Service Locator）として運用する
class Master
{
public:
	static SceneManager* scene_manager_;      // メモリリークを防ぐため、古いシーンの破棄と新規生成を中央集約で完全に保証する
	static SoundManager* sound_manager_;      // シーン遷移時もBGMを途切れさせず再生維持するため、ライフサイクルをゲーム全体と同期させる
	static Score* score_manager_;                    // 複数オブジェクトからの非同期なポイント加算による計算ズレや、UI表示との同期ズレを防ぐための一元管理用
	static ResourceManager* resource_manager_;// 毎フレームのファイルI/Oによる深刻な処理落ち（スパイク）を防ぐためのアセット共有キャッシュ
	static Camera* camera_;                   // プレイヤーの座標更新より先にカメラが動いて画面がガタつくバグを防ぐため、参照経由で最後尾に更新すること
	static DebugCamera* debug_camera_;        // リリースビルドへの意図せぬ混入やメインカメラ座標の破壊を防ぐため、必ずフラグ(is_debug_camera_)とセットで運用すること
	static bool is_debug_camera_;              // 有効時はプレイヤー入力の遮断と描画カメラの差し替えを行い、ゲームの進行状態を破壊せずにデバッグを行うためのフラグ
	static Level* level_manager_;                    // プレイ進行度に応じた難易度のインフレ計算を一元化し、バランス調整時の変更箇所を1箇所に絞るための参照
	static EffectManager* effect_manager_;    // 画面上に残存する全エフェクトの正しいZバッファ描画順序と、再生終了時の自動メモリ解放を保証する

	static bool SelectSkill;                  // 有効化中はUpdateの進行をスキップし、スキル選択UIの操作以外を受け付けないようにするための排他ロックフラグ
	static int tutorial_count_;               // 想定外の操作による進行不能（スタック）バグを防ぐため、特定のアクションを強制するためのステップ状態管理
	static bool GameFinishFlag;               // 死亡時などに複数回リザルト画面へ遷移（多重発火）してしまうクラッシュバグを防ぐためのエッジトリガ
	static bool FeverFlag;                    // ゲームのコアメカニクスを一時的に書き換えるため、有効化時は専用のUI演出やBGM変更の処理を確実に連動させること
	static int caught_cow_count_;              // スコア計算とは独立した「累計捕獲数」の要件（実績解除や内部ランク変動など）を満たすための専用カウンター
	static bool tutorial_vacum_flag_;         // チュートリアル中、特定の手順を踏む前に吸い込みアクションが暴発して進行不能になるのを防ぐ制約フラグ

	static float delta_time_;                 // OSやハードウェアの性能差によるゲームスピードの変動（FPS依存バグ）を防ぐために乗算する前フレームからの経過時間(秒)

	// 固定60FPS前提で実装された過去の移動処理ロジックを破壊せずに、可変FPS対応へと安全にリファクタリングするためのラッパー関数
	static float GetDeltaTimeScaler() { return delta_time_ * 60.0f; }
};