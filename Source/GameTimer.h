#pragma once
#include"dxlib.h"

// フレームレートに依存しない厳密な制限時間を管理し、タイムアップによるゲームオーバー遷移のトリガーを担う
// 外部からの安易な時間操作による進行不能バグを防ぐため、時間計算はクラス内部に隠蔽する
class GameTimer
{
public:
	// チュートリアル中などの意図しないタイムアップ死（進行不能バグ）を防ぐためのモード管理
	// 描画の有無だけでなく、裏側での時間進行自体を切り替える業務ルール仕様
	enum Tag_Num
	{
		Tag_Game,      // 時間進行あり・UI描画あり（本編用）
		Tag_NoGame,    // 時間進行なし・UI描画なし（チュートリアル・イベント用）
	};

public:
	// [入力] pos: UI描画座標, timer: 初期制限秒数, num: モードタグ [出力] なし [副作用] UIテクスチャのロード
	// メモリリークを防ぐため、コンストラクタで読み込んだ画像はデストラクタで確実に解放すること
	GameTimer(VECTOR pos, int timer, Tag_Num num);
	~GameTimer();

	// [入力] なし [出力] なし [副作用] mLastTimeを用いた残秒数の減算、および0到達時の状態変更
	// FPS低下によるタイマー進行の遅延バグを防ぐため、フレーム単位ではなくシステム時刻の差分ベースで計算する
	void Update();

	// [入力] なし [出力] なし [副作用] バックバッファへの制限時間UI描画
	// Tag_NoGame設定時は無駄な描画コールを発行しないよう、関数の先頭で早期リターンさせること
	void Draw();

	// タイムアップ演出の多重再生や、シーンの重複遷移バグを防ぐために外部から参照・制御するフラグ
	bool OutTimerFlag() { return mbFlag; }
	void SetOutTimerFlag(bool flag) { mbFlag = flag; }

	// スコア計算や演出分岐のためのゲッター。値のマイナス転落（UI表示崩れ）がないことを保証する前提
	int GetTime() const { return Time; }

private:
	int Time;                  // 残り秒数
	VECTOR position_;          // UI描画の基準座標
	bool mbFlag;               // タイムアップ判定フラグ
	bool mbStopFlag;           // 時間進行の停止フラグ
	int score_text_image_;     // 制限時間UIの画像ハンドル
	int mLastTime;             // 前回計算時のシステム時刻
	Tag_Num tag_;              // 現在のタイマー動作モード
};