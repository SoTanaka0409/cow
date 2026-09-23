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

/// @brief ゲーム全体で共有する管理クラスへの参照をまとめる
class Master
{
public:
	static SceneManager* scene_manager_;       ///< シーンの生成、破棄、遷移を管理する
	static SoundManager* sound_manager_;       ///< BGMと効果音の再生を管理する
	static Score* score_manager_;              ///< スコア計算と表示用データを管理する
	static ResourceManager* resource_manager_; ///< 画像、モデル、音源などの読み込みを管理する
	static Camera* camera_;                    ///< 通常プレイ時のカメラを管理する
	static DebugCamera* debug_camera_;         ///< デバッグ用カメラを管理する
	static bool is_debug_camera_;              ///< デバッグカメラを使うかどうかのフラグ
	static Level* level_manager_;              ///< 難易度や進行度を管理する
	static EffectManager* effect_manager_;     ///< 画面上のエフェクトを管理する

	static bool SelectSkill;          ///< スキル選択中かどうかのフラグ
	static int tutorial_count_;       ///< チュートリアル進行を管理するカウンター
	static bool GameFinishFlag;       ///< ゲーム終了処理の重複を防ぐフラグ
	static bool FeverFlag;            ///< フィーバー状態かどうかのフラグ
	static int caught_cow_count_;     ///< 捕まえた牛の数を記録するカウンター
	static bool tutorial_vacum_flag_; ///< チュートリアル中、特定の手順を踏む前に吸い込みアクションが暴発して進行不能になるのを防ぐ制約フラグ
	static bool is_shadow_enabled_;   ///< 設定画面でのON/OFF選択を引き継ぐためのフラグ。trueのときのみScene3Dがシャドウマップを生成する
	static bool is_debug_mode_;       ///< 設定画面でのON/OFF選択を引き継ぐためのデバッグモードフラグ

	static float delta_time_; ///< 前フレームからの経過時間

	/// @brief 60FPS基準の移動量を現在のフレーム時間に合わせる
	static float GetDeltaTimeScaler() { return delta_time_ * 60.0f; }
};
