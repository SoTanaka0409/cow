#pragma once
#include"dxlib.h"


// プレイヤーの行動制限となるゲーム全体の制限時間を管理するクラス
class GameTimer 
{
public:
	// タイマーを使用する状況タグ
	enum Tag_Num
	{
		Tag_Game,      // プレイ中などの描画が必要なシーン
		Tag_NoGame,    // チュートリアルなど時間制限を描画しないシーン
	};

public:
	GameTimer(VECTOR pos, int timer, Tag_Num num);
	~GameTimer();

	/*
	 * @brief 制限時間の減算処理を行い、終了判定を監視する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 残り時間の更新、時間切れ時にmbFlagおよびmbStopFlagをtrue化
	 */
	void Update();

	/*
	 * @brief プレイヤーに残り時間を通知するためのUIを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] グラフィック描画
	 */
	void Draw();

	// 時間切れ検知などの外部アクセス用メソッド
	bool OutTimerFlag() { return mbFlag; }
	void SetOutTimerFlag(bool flag) { mbFlag = flag; }
	int GetTime() const { return Time; }

private:
	int Time;                  // 描画および時間切れ判定の基準となる残り秒数
	VECTOR mvPosition;         // 画面解像度に合わせてUIを配置するための基準座標
	bool mbFlag;               // ゲームオーバー遷移のトリガーとなる時間切れフラグ
	bool mbStopFlag;           // ポーズ中やリザルト画面での時間進行を止めるフラグ

	int scoreTextImage;        // 毎フレームのロードを防ぐためのLIMIT画像ハンドル

	int mLastTime;             // 1秒経過を判定するための前回計測時間
	Tag_Num mnTag;             // チュートリアルと本編で描画処理を分けるためのタグ
};
