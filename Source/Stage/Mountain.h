#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

/// @brief 景観を表現するため、静的な山モデルを管理する。
class Mountain : public Object3D
{
public:
	/// @brief 背景演出用の静的モデルを構築するため
	/// @param filename モデルのファイルパス
	/// @param initPos 初期座標
	/// @param scale 拡大率
	/// @param angle 回転角度
	/// @details model_に新しくModelインスタンスが確保される
	Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle);

	/// @brief メモリリークを防ぐため
	/// @details model_のメモリが解放される
	virtual ~Mountain();

	/// @brief 状態を最新に保つため
	void Update() override;

	/// @brief 画面にモデルを可視化するため
	/// @details 画面に描画される
	void Draw() override;

	/// @brief モデルの大きさを柔軟に変更するため
	/// @param scale 新しい拡大率
	/// @details スケール状態が更新される
	void SetScale(VECTOR scale);

	/// @brief モデルの向きを柔軟に変更するため
	/// @param angle 新しい回転角度
	/// @details 回転状態が更新される
	void SetRotation(VECTOR angle);

	/// @brief 時間帯などの環境変化に合わせて色を変えるため
	/// @param r 赤
	/// @param g 緑
	/// @param b 青
	/// @param a アルファ値
	/// @details 色状態が更新される
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	Model* model_; ///< 3Dモデルの管理に使用する情報
};
