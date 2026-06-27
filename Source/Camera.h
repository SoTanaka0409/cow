#pragma once
#include "DxLib.h"
#include "Model.h"
#include"Object3D.h"

class Skill;

// ゲーム内の3Dカメラ制御、位置・注視点計算、および振動（シェイク）演出を管理するクラス
class Camera
{
public:
	/*
	 * @brief メンバ変数を初期設定角度とデフォルト値に初期設定する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	Camera();
	~Camera();

	/*
	 * @brief カメラの位置、角度、およびシェイクパラメータを初期値にリセットする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各種カメラパラメータ変数のリセット
	 */
	void Initialize();

	/*
	 * @brief カメラ解放処理（現在は空メソッド）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Finalize();

	/*
	 * @brief カメラの現在情報をデバッグ等の目的で画面描画する（現在は空メソッド）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief マウスの移動量やゲームの進行フェーズ、シェイク状態を計算し、最終的な3Dカメラ位置と注視点をDXライブラリに反映する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] DXライブラリ of 3Dカメラ状態の設定変更
	 */
	void Update();

	/*
	 * @brief 3D空間上でマウス移動量とキー入力(0キー)に基づくカメラ角度の更新計算を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 水平回転角・垂直回転角変数の更新
	 */
	void UpdateRotate();

	/*
	 * @brief ゲームのフェーズ（演出カメラ、プレイヤー追従など）に合わせた専用のカメラ位置・注視点更新処理を行う
	 * [入力] phase: 現在のゲームフェーズ, ufoPos: プレイヤーUFOの座標, tornadoPos: 竜巻の座標
	 * [出力] なし
	 * [副作用] カメラ座標および注視点座標の更新
	 */
	void UpdateCameraByPhase(int phase, VECTOR ufoPos, VECTOR tornadoPos);

	// ポジション、注視点などのゲッター・セッター
	void SetPosition(VECTOR pos) { mvPosition = pos; }
	void SetTarget(Object3D* target) { target = target; }
	VECTOR GetPosition() { return mvPosition; }

	VECTOR GetLookAtPosition() { return mvLookAtPosition; }

	/*
	 * @brief 前フレームからマウスが移動したかどうかを検知する
	 * [入力] なし
	 * [出力] マウスが移動した場合はtrue
	 * [副作用] なし
	 */
	bool IsMouseMoved();

	/*
	 * @brief カメラの座標に対して現在累積されているシェイク量の減衰と加算処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mvShakePositionの書き換え
	 */
	void Shake();

	/*
	 * @brief カメラを揺らすシェイク演出の初期値を設定する
	 * [入力] time: 継続時間（秒）, width: 揺れ幅, angleSpeed: 揺れ周期速度, stepTime: 時間経過間隔
	 * [出力] なし
	 * [副作用] シェイク再生用タイマーとパラメータの初期設定
	 */
	void SetupShake(float time, float width, float angleSpeed, float stepTime = 1.0f);

	// フェーズごとの演出用特殊カメラが有効化されているかの判定ゲッター
	bool GetIsPhaseCameraActive() const { return isPhaseCameraActive; }

private:
	float horizontalAngle;  // カメラの水平回転角（ラジアン）
	float verticalAngle;    // カメラの垂直回転角（ラジアン）

	bool isPhaseCameraActive; // フェーズ専用カメラが有効かどうかのフラグ

	VECTOR mvPosition;        // カメラの基準位置座標
	VECTOR mvLookAtPosition;  // カメラの注視点（ルックアット）座標
	VECTOR dir;               // カメラの視線向きベクトル

	Object3D* target;       // 注視対象となる3Dオブジェクトへのポインタ

	const float ROTATE_SPEED = 0.2f; // カメラ回転の追従速度係数
	float targetAngle;      // 目標とする水平回転角度
	float angle;            // 現在の補間中水平回転角度
	int centerX;              // 画面中心のX座標
	int centerY;              // 画面中心のY座標

	int mouseX, mouseY;       // マウスの現在位置座標（一時変数）

	int currentMouseX = 0;          // 現在フレームでのマウス位置X
	int currentMouseY = 0;          // 現在フレームでのマウス位置Y

	int prevMouseX = 0;      // 1フレーム前でのマウス位置X
	int prevMouseY = 0;      // 1フレーム前でのマウス位置Y

	int shakeTimeFrames;          // シェイク持続時間フレームカウンタ
	int shakeTimeCount;     // シェイク経過フレームカウンタ

	float shakeAngle;       // シェイク計算用の正弦波角度カウンタ
	float shakeTimeCounter; // シェイクのフェードアウト用時間カウンタ
	float shakeTime;        // 設定されたシェイク時間
	float shakeWidth;       // 設定された最大揺れ幅
	float shakeAngleSpeed;  // 設定された揺れの周期速度
	float stepTime;         // 毎フレームのシェイク時間減算幅
	VECTOR mvShakePosition;   // シェイク計算によって加算されるカメラオフセット座標値
};

