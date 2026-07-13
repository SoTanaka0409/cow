#pragma once
#include "DxLib.h"
#include "Model.h"
#include "Object3D.h"

class Skill;

class Camera
{
public:
	/*
	 * カメラの基本情報や揺れ状態を初期状態にするため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各種カメラパラメータ変数のリセット
	 */
	Camera();
	~Camera();

	/*
	 * 再スタートやシーン切り替え時にカメラ状態をリセットするため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] カメラ座標やシェイクなどのメンバ変数を初期値に戻す
	 */
	void Initialize();

	/*
	 * リソース解放など終了処理用
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Finalize();

	/*
	 * デバッグや状態確認用の情報を画面に表示するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * 毎フレームのマウス入力や状態変化をDXライブラリのカメラに反映するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] DXライブラリのカメラ座標・注視点の更新
	 */
	void Update();

	/*
	 * プレイヤーの視点操作をカメラの回転角に反映させるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 水平回転角・垂直回転角を更新
	 */
	void UpdateRotate();

	/*
	 * ゲーム進行状況に応じて専用の視点へ切り替えるため
	 * [入力] phase: 現在のゲームフェーズ, ufoPos: プレイヤー座標, tornadoPos: 竜巻の座標
	 * [出力] なし
	 * [副作用] phaseに応じたカメラ座標および注視点の更新
	 */
	void UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos);

	void SetPosition(VECTOR pos) { position_ = pos; }
	void SetTarget(Object3D* target) { target_ = target; }
	VECTOR GetPosition() { return position_; }

	VECTOR GetLookAtPosition() { return look_at_position_; }

	/*
	 * カメラの視点移動処理を行うべきか判定するため
	 * [入力] なし
	 * [出力] 前フレームからマウスが移動していればtrue
	 * [副作用] なし
	 */
	bool IsMouseMoved();

	/*
	 * ダメージや演出などでの画面揺れを適用するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] shake_position_を更新
	 */
	void Shake();

	/*
	 * 特定の演出や被弾時に画面を揺らすパラメータを登録するため
	 * [入力] time: 揺れ時間, width: 揺れ幅, angleSpeed: 揺れ速度, stepTime: 時間経過幅
	 * [出力] なし
	 * [副作用] シェイク時間や揺れ幅などのメンバ変数を設定
	 */
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

	bool GetIsPhaseCameraActive() const { return is_phase_camera_active_; }

private:
	float horizontal_angle_;
	float vertical_angle_;

	bool is_phase_camera_active_;

	VECTOR position_;
	VECTOR look_at_position_;
	VECTOR dir_;

	Object3D* target_;

	const float kRotateSpeed = 0.2f;
	float target_angle_;
	float angle_;
	int center_x_;
	int center_y_;

	int mouse_x_, mouse_y_;

	int current_mouse_x_ = 0;
	int current_mouse_y_ = 0;

	int prev_mouse_x_ = 0;
	int prev_mouse_y_ = 0;

	int shake_time_;
	int shake_time_count_;

	float shake_angle_;
	float shake_time_counter_;
	float mfShakeTime;
	float shake_width_;
	float shake_angle_speed_;
	float step_time_;
	VECTOR shake_position_;
};
