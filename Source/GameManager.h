#pragma once
#include"DxLib.h"
#include<vector>
class GameTimer;
// ゲーム全体の進行ステップやフェーズ遷移を統括するため。
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
		GameStepType type;
		bool TrueFlag = true;
	};
	enum class GamePhase
	{
		kNormal,
		kMassSpawn,
		kTornadoCrisis
	};
public:
	/*
	 * ゲーム進行の初期化を行うため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] メンバ変数の初期化、タイマーの生成
	 */
	GameManager();
	/*
	 * 確保したステップデータやタイマーを破棄するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 動的確保したメモリの解放
	 */
	~GameManager();
	/*
	 * 外部から現在のフェーズ状態を判定するため。
	 * [入力] なし
	 * [出力] 現在のGamePhase
	 * [副作用] なし
	 */
	GamePhase GetCurrentPhase() const { return current_phase_; }
	/*
	 * イベント進行等で強制的にフェーズを切り替えるため。
	 * [入力] phase: 新しいフェーズ状態
	 * [出力] なし
	 * [副作用] current_phase_が更新される
	 */
	void SetCurrentPhase(GamePhase phase) { current_phase_ = phase; }
	/*
	 * 条件達成時に次の進行ステップへ進めるため。
	 * [入力] type: 遷移先のステップタイプ
	 * [出力] なし
	 * [副作用] 進行ステップが変更され、フラグが更新される
	 */
	void GameNextStep(GameStepType type);
	 
	/*
	 * 現在のゲーム進行状況を外部から参照するため。
	 * [入力] なし
	 * [出力] 現在のステップタイプ
	 * [副作用] なし
	 */
	GameStepType GetType() { return type_; }
	/*
	 * フェードイン等の画面演出を描画するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面に暗転などの演出が描画される
	 */
	void Draw();
	/*
	 * タイマーやフェーズ遷移など、ゲームの進行を管理するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各種タイマーが更新され、フェーズが切り替わる
	 */
	void Update();
	/*
	 * 外部から残り時間を参照・操作するため。
	 * [入力] なし
	 * [出力] ゲームタイマーのポインタ
	 * [副作用] なし
	 */
	GameTimer* GetGameTimer() const { return game_timer_; }
private:
	std::vector<GameStepData*> data_;
	GameStepType type_;
	GamePhase current_phase_;
	float fade_timer_;
	bool fade_flag_;
	
	int phase_timer_;
	int phase_change_count_;
	GameTimer* game_timer_;
};