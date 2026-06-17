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
		bool TrueFlag = true;
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
	GamePhase GetCurrentPhase() const { return mCurrentPhase; }

	/*
	 * ゲームフェーズを設定しイベント状態を切り替える
	 * [入力] phase: 新しいフェーズ状態
	 * [出力] なし
	 * [副作用] mCurrentPhase が更新される
	 */
	void SetCurrentPhase(GamePhase phase) { mCurrentPhase = phase; }

	/*
	 * スコア登録を伴う終了ステップへの遷移を行う
	 * [入力] type: 遷移先のステップタイプ
	 * [出力] なし
	 * [副作用] 進行ステップ変更、フラグ更新、ネーム入力開始
	 */
	void GameNextStep(GameStepType type);
	 
	GameStepType GetType() { return mnType; }

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

	GameTimer* GetGameTimer() const { return mpGameTimer; }

private:
	std::vector<GameStepData*> mData; // 動的確保されたステップデータのリスト
	GameStepType mnType;
	GamePhase mCurrentPhase;

	float Fadetimer;                  // シーン開始時のフェード演出用
	bool Fadeflag;
	
	int m_PhaseTimer;                 // フェーズ遷移イベントの発生タイミング基準
	int m_PhaseChangeCount;

	GameTimer* mpGameTimer;           // タイマー管理用インスタンス
};
