#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

// 景観を表現するため、静的な山モデルを管理する。
class Mountain : public Object3D
{
public:
	/*
	 * 背景演出用の静的モデルを構築するため
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度
	 * [出力] なし
	 * [副作用] model_に新しくModelインスタンスが確保される
	 */
	Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle);

	/*
	 * メモリリークを防ぐため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] model_のメモリが解放される
	 */
	virtual ~Mountain();

	/*
	 * 状態を最新に保つため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update() override;

	/*
	 * 画面にモデルを可視化するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面に描画される
	 */
	void Draw() override;

	/*
	 * モデルの大きさを柔軟に変更するため
	 * [入力] scale: 新しい拡大率
	 * [出力] なし
	 * [副作用] スケール状態が更新される
	 */
	void SetScale(VECTOR scale);

	/*
	 * モデルの向きを柔軟に変更するため
	 * [入力] angle: 新しい回転角度
	 * [出力] なし
	 * [副作用] 回転状態が更新される
	 */
	void SetRotation(VECTOR angle);

	/*
	 * 時間帯などの環境変化に合わせて色を変えるため
	 * [入力] r: 赤, g: 緑, b: 青, a: アルファ値
	 * [出力] なし
	 * [副作用] 色状態が更新される
	 */
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	Model* model_;
};
