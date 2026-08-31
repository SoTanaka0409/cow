#pragma once
#include"DxLib.h"
#include<vector>
class GameTimer;
/// @brief ゲーム全体の進行ステップやフェーズ遷移を統括するため。
class GameManager
{
public:
	enum GameStepType
	{
		kCowGet,
		kFinal,
	};
	struct GameStepData
	{
		GameStepType type;    ///< 現在の状態や種別を管理する値
		bool TrueFlag = true; ///< 状態の有効・無効を管理するフラグ
	};
	enum class GamePhase
	{
		kNormal,
		kMassSpawn,
		kTornadoCrisis
	};
public:
	/// @brief ゲーム進行の初期化を行うため。
	/// @details メンバ変数の初期化、タイマーの生成
	GameManager();
	/// @brief 確保したステップデータやタイマーを破棄するため。
	/// @details 動的確保したメモリの解放
	~GameManager();
	/// @brief 外部から現在のフェーズ状態を判定するため。
	/// @return 現在のGamePhase
	GamePhase GetCurrentPhase() const { return current_phase_; }
	/// @brief イベント進行等で強制的にフェーズを切り替えるため。
	/// @param phase 新しいフェーズ状態
	/// @details current_phase_が更新される
	void SetCurrentPhase(GamePhase phase) { current_phase_ = phase; }
	/// @brief 条件達成時に次の進行ステップへ進めるため。
	/// @param type 遷移先のステップタイプ
	/// @details 進行ステップが変更され、フラグが更新される
	void GameNextStep(GameStepType type);

	/// @brief 現在のゲーム進行状況を外部から参照するため。
	/// @return 現在のステップタイプ
	GameStepType GetType() { return type_; }
	/// @brief フェードイン等の画面演出を描画するため。
	/// @details 画面に暗転などの演出が描画される
	void Draw();
	/// @brief タイマーやフェーズ遷移など、ゲームの進行を管理するため。
	/// @details 各種タイマーが更新され、フェーズが切り替わる
	void Update();
	/// @brief 外部から残り時間を参照・操作するため。
	/// @return ゲームタイマーのポインタ
	GameTimer* GetGameTimer() const { return game_timer_; }
private:
	std::vector<GameStepData*> data_; ///< 複数のデータを管理するコンテナ
	GameStepType type_;               ///< 現在の状態や種別を管理する値
	GamePhase current_phase_;         ///< 現在の状態や種別を管理する値
	float fade_timer_;                ///< 時間経過や処理間隔を管理するカウンター
	bool fade_flag_;                  ///< 状態の有効・無効を管理するフラグ

	int phase_timer_;        ///< 時間経過や処理間隔を管理するカウンター
	int phase_change_count_; ///< 時間経過や処理間隔を管理するカウンター
	GameTimer* game_timer_;  ///< 時間経過や処理間隔を管理するカウンター

	void UpdateGameTimerAndPhase();
};
