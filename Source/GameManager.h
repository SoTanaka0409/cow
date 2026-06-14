#pragma once
#include"DxLib.h"
#include<vector>

class GameTimer;

// ゲーム本編における進行フェーズ（通常、竜巻巨大化、大量発生）およびステップを管理するクラス
class GameManager
{
public:
	// ゲーム本編のステップ段階定義
	enum GameStepType
	{
		game_CowGet,    // 通常の牛ゲット
		game_final,     // タイムアップ後の終了・画面遷移待機ち段階
	};

	// 各ステップの有効フラグ構造体
	struct GameStepData
	{
		GameStepType type;
		bool TrueFlag = true;
	};

	// 進行中イベントのフェーズ（演出段階）
	enum class GamePhase
	{
		Normal,         // 通常時
		MassSpawn,      // 牛の大量発生・落下フェーズ
		TornadoCrisis   // 竜巻が巨大化して動き回るフェーズ
	};

public:
	GameManager();
	~GameManager();

	/*
	 * @brief 指定されたゲームフェーズタイプを取得する
	 * [入力] なし
	 * [出力] 現在のGamePhase（Normal / MassSpawn / TornadoCrisis）
	 * [副作用] なし
	 */
	GamePhase GetCurrentPhase() const { return mCurrentPhase; }

	/*
	 * @brief 指定されたゲームフェーズタイプを設定する
	 * [入力] phase: 新しいフェーズ状態
	 * [出力] なし
	 * [副作用] mCurrentPhase の更新
	 */
	void SetCurrentPhase(GamePhase phase) { mCurrentPhase = phase; }

	/*
	 * @brief 次のステップ（終了画面等）へ状態遷移させ、スコアランキング登録を行う
	 * [入力] type: 遷移先のステップタイプ
	 * [出力] なし
	 * [副作用] mnTypeの更新、Master::GameFinishFlagの有効化、スコアの追加・ネーム入力の開始
	 */
	void GameNextStep(GameStepType type);
	 
	GameStepType GetType() { return mnType; }

	/*
	 * @brief シーン開始時のフェードイン暗転などを画面に描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 暗転矩形（DrawBox）の描画
	 */
	void Draw();

	/*
	 * @brief 制限時間タイマーの更新、および時間経過によるイベントフェーズ切り替えの抽選を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] タイマー更新、フェーズ遷移、名前入力の更新
	 */
	void Update();

	GameTimer* GetGameTimer() const { return mpGameTimer; }

private:
	std::vector<GameStepData*> mData; // 各ステップデータのリスト（メモリ解放対象）
	GameStepType mnType;              // 現在アクティブなステップタイプ
	GamePhase mCurrentPhase;          // 現在のゲーム内フェーズ

	float Fadetimer;                  // 開始時のフェードイン用不透明度カウンタ
	bool Fadeflag;                    // フェードイン演出中かどうかのフラグ
	
	int m_PhaseTimer;                 // フェーズ切り替え間隔（ミリ秒）計測用基準値
	int m_PhaseChangeCount;           // 前回のフェーズ切り替えからの経過秒数カウンター

	GameTimer* mpGameTimer;           // 制限時間を管理するタイマーオブジェクトポインタ
};
