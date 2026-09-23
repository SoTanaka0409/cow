#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Thunder.h"
#include"CowManager.h"
#include"AnimalManager.h"

class GameScene : public Scene
{
public:
	enum Phase
	{
		kNormal,
		kTornadoCrisis,
		kMobSpawn,
	};

public:
	GameScene();
	~GameScene();

	/// @brief リソース確保と初期状態へのリセットを行うため
	/// @details オブジェクト生成、BGM再生
	void Initialize() override;

	/// @brief リソースリークを防ぎ、次のシーンへ安全に遷移するため
	/// @details BGM停止、リソース解放
	void Finalize() override;

	/// @brief 3D空間とUIを合成してプレイヤーに状況を視覚伝達するため
	/// @details 画面描画
	void Draw() override;

	/// @brief 毎フレームのゲームロジックと状態遷移を処理するため
	/// @details タイムアップ時にリザルトへ遷移
	void Update() override;

	/// @brief 進行度に応じて難易度を段階的に引き上げるため
	/// @details カメラ・竜巻状態変更、スポーン処理
	void PhaseUpdate();

private:
	/// @brief 処理の実行
	void DrawShadowMap();
	/// @brief 処理の実行
	void DrawGrid();
	/// @brief 処理の実行
	void DrawPhaseUI();



	int tornado_spawn_timer_; ///< 時間経過や処理間隔を管理するカウンター
	int mass_spawn_timer_;     ///< 時間経過や処理間隔を管理するカウンター
	Phase phase_;              ///< 現在の状態や種別を管理する値
	int font_back_graph_;      ///< 描画やリソース管理に使用するハンドル
	int shadow_map_handle_;    ///< 描画やリソース管理に使用するハンドル
	int phase_font_handle_;    ///< MASS SPAWN!/TORNADO CRISIS!警告表示用フォント
};
