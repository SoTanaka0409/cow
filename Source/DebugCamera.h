#pragma once
#include "DxLib.h"

// デバッグ用の一人称視点（スペクテイター）の自由移動カメラ制御クラス
class DebugCamera
{
public:
	/*
	 * @brief メンバ変数を初期座標とデフォルト角度に初期化する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	DebugCamera();
	~DebugCamera();

	/*
	 * @brief 現在のアクティブなゲームカメラの位置と角度を引き継いでデバッグカメラを初期設定する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] カメラ座標と角度変数の初期設定
	 */
	void Initialize();

	/*
	 * @brief キーボード入力(WASD / LSHIFT)による移動と、マウスによる視点回転を更新し、DXライブラリのカメラを更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] DXライブラリの3Dカメラ状態の設定変更
	 */
	void Update();

	/*
	 * @brief マウスの移動量に応じて、水平角度と垂直角度の回転量を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 水平・垂直回転角度変数の更新、マウスポインタの画面中心への引き戻し
	 */
	void UpdateRotate();

private:
	VECTOR mvPosition;        // デバッグカメラの現在座標
	float mfHorizontalAngle;  // 水平（左右ヨー）回転角度
	float mfVerticalAngle;    // 垂直（上下ピッチ）回転角度
};

