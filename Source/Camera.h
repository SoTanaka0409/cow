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
	void UpdatePositionAndTarget();
	void UpdateEffekseerAndLight();


	// ジンバルロック（特異点による回転異常）回避のため、縦回転の適用時は±89度のクランプ処理を必須とする
	float horizontal_angle_;
	float vertical_angle_;

	// イベント演出時のカメラワークを優先するため、true時はプレイヤーからの視点操作入力を遮断する
	bool is_phase_camera_active_;

	// 毎フレームtarget_の位置と各angle_から再計算されるため、他処理からの直接的な座標上書きは禁止
	VECTOR position_;
	VECTOR look_at_position_;
	VECTOR dir_;

	// 追従対象。対象オブジェクト破棄時の参照エラーによるクラッシュを防ぐため、アクセス前のnullチェック必須
	Object3D* target_;

	// 3D酔いの軽減と操作レスポンスのバランスを考慮し、プランナー仕様により設定された旋回速度の規定値
	const float kRotateSpeed = 0.2f;

	// 視点移動のカクつきを防ぐため、即値の代入ではなく現在角度から目標角度へ補間（Lerp等）を行うための状態保持
	float target_angle_;
	float angle_;

	// 画面端へのマウス到達による旋回不能状態を防ぐため、毎フレームカーソルを強制的に戻す基準となる座標
	int center_x_;
	int center_y_;

	// 1フレーム間のマウス移動量（デルタ）を算出しカメラ回転量へ変換するため、前フレームとの差分計算に使用する
	int mouse_x_, mouse_y_;
	int current_mouse_x_ = 0;
	int current_mouse_y_ = 0;
	int prev_mouse_x_ = 0;
	int prev_mouse_y_ = 0;

	// 【カメラシェイク制御用変数群】
	// 揺れ処理による本来のカメラ座標(position_)の意図せぬ座標ズレ（汚染）を防ぐため、
	// 揺れ幅はオフセット座標(shake_position_)として計算し、描画行列の構築直前にのみ一時加算すること
	int shake_time_;
	int shake_time_count_;
	float shake_angle_;
	float shake_time_counter_;

	// 旧シェイクアセット仕様との互換性を保つための変数。命名規則外だが外部仕様依存のため維持（リファクタリング保留）
	float mfShakeTime;

	float shake_width_;
	float shake_angle_speed_;
	float step_time_;
	VECTOR shake_position_;
};
