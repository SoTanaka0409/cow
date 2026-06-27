#pragma once
#include"DxLib.h"
#include<vector>

class GameTimer;

// イベント進行フェーズを管理するクラス
class GameManager
{
public:
	enum GameStepType
	{
		game_CowGet,
		game_final,
	};

	struct GameStepData
	{
		GameStepType type;
		bool trueFlag = true;
	};

	enum class GamePhase
	{
		Normal,
		MassSpawn,
		TornadoCrisis
	};

public:
	GameManager();
	~GameManager();

	/*
	 * 現在のゲームフェーズを取得する
	 * [入力] なし
	 * [出力] 現在のGamePhase
	 * [副作用] なし
	 */
	GamePhase GetCurrentPhase() const { return currentPhase; }

	/*
	 * ゲームフェーズを設定しイベント状態を切り替える
	 * [入力] phase: 新しいフェーズ状態
	 * [出力] なし
	 * [副作用] currentPhase が更新される
	 */
	void SetCurrentPhase(GamePhase phase) { currentPhase = phase; }

	/*
	 * スコア登録を伴う終了ステップへの遷移を行う
	 * [入力] type: 遷移先のステップタイプ
	 * [出力] なし
	 * [副作用] 進行ステップ変更、フラグ更新、ネーム入力開始
	 */
	void GameNextStep(GameStepType type);
	 
	GameStepType GetType() { return type; }

	/*
	 * フェードイン等、ゲーム進行に必要な演出描画を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面への暗転矩形描画
	 */
	void Draw();

	/*
	 * ゲームの進行状態と制限時間を監視・更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] タイマー更新、フェーズ遷移抽選実行
	 */
	void Update();

	GameTimer* GetGameTimer() const { return gameTimer; }

private:
	std::vector<GameStepData*> gameStepDataList; // 動的確保されたステップデータのリスト
	GameStepType type;
	GamePhase currentPhase;

	float Fadetimer;                  // シーン開始時のフェード演出用
	bool Fadeflag;
	
	int phaseTimer;                 // フェーズ遷移イベントの発生タイミング基準
	int phaseChangeCount;

	GameTimer* gameTimer;           // タイマー管理用インスタンス
};
