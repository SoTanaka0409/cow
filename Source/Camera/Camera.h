#pragma once
#include "DxLib.h"
#include "Model.h"
#include "Object3D.h"

class Camera
{
public:
	Camera();
	~Camera();

	/// @brief カメラパラメータおよびシェイク状態の初期化
	void Initialize();

	/// @brief 入力：なし
	void Finalize();

	/// @brief カメラ座標やデバッグ情報を画面へレンダリング
	void Draw();

	/// @brief マウス入力による視点操作およびカメラ座標・注視点の再計算
	void Update();

	/// @brief マウス入力のデルタ値を水平・垂直回転角へ変換
	void UpdateRotate();

	/// @param phase 現在の演出フェーズ
	/// @brief ufoPos/tornadoPos = ターゲット座標
	/// @details プレイヤー操作を遮断し、特定の座標を強制注視させるカメラワークへの移行
	void UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos);

	void SetPosition(VECTOR pos) { position_ = pos; }
	void SetTarget(Object3D* target) { target_ = target; }
	VECTOR GetPosition() { return position_; }
	VECTOR GetLookAtPosition() { return look_at_position_; }

	/// @brief 値を取得する
	/// @return 前フレームとのマウス座標差分があれば true
	bool IsMouseMoved();

	/// @brief 三角関数を用いた加算的な揺れ（shake_position_）の計算
	void Shake();

	/// @param time 揺れ継続時間
	/// @param width 揺れ幅
	/// @param angleSpeed 回転速度
	/// @param stepTime 時間経過係数
	/// @brief シェイク制御用カウンタの初期化
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

	bool GetIsPhaseCameraActive() const { return is_phase_camera_active_; }

private:
	void UpdatePositionAndTarget();
	void UpdateEffekseerAndLight();

	/// @brief 設計ルール：ジンバルロック回避のため、垂直回転は±89度のクランプ処理により計算の特異点を排除する
	float horizontal_angle_; ///< 移動や回転の計算に使用する値
	float vertical_angle_;   ///< 移動や回転の計算に使用する値

	/// @brief 設計ルール：演出中はプレイヤーの入力を遮断し、シネマティックな視点遷移を保護するロックフラグ
	bool is_phase_camera_active_; ///< 状態の有効・無効を管理するフラグ

	VECTOR position_;         ///< 座標や位置情報を管理する値
	VECTOR look_at_position_; ///< 座標や位置情報を管理する値
	VECTOR dir_;              ///< 移動や回転の計算に使用する値

	/// @brief メモリ管理：追従対象オブジェクトが破棄された際の参照エラーを回避するため、使用前には必ずポインタのnullチェックを行うこと
	Object3D* target_; ///< 参照対象のオブジェクトを保持するポインタ

	/// @brief UX仕様：3D酔いを軽減しつつレスポンスを担保するための、プランナー仕様に基づく旋回速度
	const float kRotateSpeed = 0.2f; ///< ゲーム内で使用する固定値

	/// @brief UX仕様：視点のカクつきを防ぐため、目標角度と現在角度の間で滑らかな補間処理を行う
	float target_angle_; ///< 移動や回転の計算に使用する値
	float angle_;        ///< 移動や回転の計算に使用する値

	/// @brief アーキテクチャ設計：画面端でのマウス到達による視点操作停止を回避するため、カーソル位置の強制リセットを行うための基準点
	int center_x_, center_y_; ///< 座標や位置情報を管理する値

	/// @brief アーキテクチャ設計：マウスの移動量（デルタ）をフレーム間で算出し、カメラの回転量へ反映させるための位置保持用
	int mouse_x_, mouse_y_;                         ///< 音量やサウンド再生状態を管理する値
	int current_mouse_x_ = 0, current_mouse_y_ = 0; ///< 音量やサウンド再生状態を管理する値
	int prev_mouse_x_ = 0, prev_mouse_y_ = 0;       ///< 音量やサウンド再生状態を管理する値

	/// @brief 【カメラシェイク制御】
	/// @details 揺れ処理は本来の座標(position_)を汚染しないよう「加算用のオフセット(shake_position_)」として計算し、最終描画直前のみ適用する設計
	int shake_time_;           ///< 時間経過や処理間隔を管理するカウンター
	int shake_time_count_;     ///< 時間経過や処理間隔を管理するカウンター
	float shake_angle_;        ///< 移動や回転の計算に使用する値
	float shake_time_counter_; ///< 時間経過や処理間隔を管理するカウンター

	/// @brief メモリ管理：旧アセット互換維持用（リファクタリング保留）
	float old_shake_time_;    ///< 時間経過や処理間隔を管理するカウンター
	float shake_width_;       ///< サイズや範囲の計算に使用する値
	float shake_angle_speed_; ///< 移動や回転の計算に使用する値
	float step_time_;         ///< 時間経過や処理間隔を管理するカウンター
	VECTOR shake_position_;   ///< 座標や位置情報を管理する値
};
