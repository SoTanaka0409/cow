#pragma once
#include"dxlib.h"


// ゲーム本編の制限時間タイマーを管理・描画するクラス
class GameTimer 
{
public:
	// タイマーを使用する状況タグ
	enum Tag_Num
	{
		Tag_Game,      // ゲーム本編での使用（タイマー描画あり）
		Tag_NoGame,    // 非ゲーム中（タイマー描画なし）
	};

public:
	GameTimer(VECTOR pos, int timer, Tag_Num num);
	~GameTimer();

	/*
	 * @brief 毎フレームのタイマー時間経過処理を行う（1000msごとに残時間を1減算）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] Timeの減算、タイムアップ時の mbFlag / mbStopFlag の有効化、mLastTimeの更新
	 */
	void Update();

	/*
	 * @brief 画面上部に「LIMIT」テキスト画像と残り秒数の数値を並べて描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] バックバッファへのグラフィック描画
	 */
	void Draw();

	// ゲッター・セッター群
	bool OutTimerFlag() { return mbFlag; }
	void SetOutTimerFlag(bool flag) { mbFlag = flag; }
	int GetTime() const { return Time; }

private:
	int Time;                  // 残り時間（秒）
	VECTOR mvPosition;         // タイマーUIの描画開始座標
	bool mbFlag;               // タイムアップ（時間切れ）に達したかどうかのフラグ
	bool mbStopFlag;           // タイマーの更新処理を停止するフラグ

	int scoreTextImage;        // 「LIMIT」テキストの画像ハンドル
	int numberImg[10];         // 残り秒数を描画するための 0?9 のデジタル数字画像ハンドル

	int mLastTime;             // 前回の1秒経過判定時の GetNowCount() の値
	Tag_Num mnTag;             // タイマーの使用シーンを示す分類タグ
};

