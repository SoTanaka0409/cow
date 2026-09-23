#pragma once
#include "Object3D.h"
#include <string>

/// @brief 設計ルール：ポリゴン数の多い複雑な描画用モデルと、処理を軽量化した衝突判定用（コリジョン用）モデルを同期管理するクラス
class Stage : public Object3D
{
public:
	/// @param init_pos 配置初期座標
	/// @param stage_model_name 地形描画用モデルパス
	/// @param stage_collision_model_name 衝突判定用モデルパス
	/// @brief 各種地形モデルハンドル（DxLib）のロード
	Stage(VECTOR init_pos, const std::string& stage_model_name, const std::string& stage_collision_model_name);
	virtual ~Stage();

	/// @brief 毎フレームの更新処理
	void Update() override;
	/// @brief 描画処理
	void Draw() override;

	/// @param pos2 カプセルの始点・終点座標
	/// @param r カプセルの半径
	/// @return 地形との衝突有無（真偽値）
	/// @brief pos1
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	/// @param pos2 レイ（線分）の始点・終点座標
	/// @return 地形と交差したヒット位置の3D座標（非衝突時はゼロベクトル）
	/// @brief pos1
	/// @details UFOから真下に放つアブダクションビームや、牛の接地（高さ合わせ）の計算に使用する
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);


private:
	/// @brief 描画負荷と物理演算（メッシュ衝突判定）の計算負荷をそれぞれ最適化するため、ハンドルを完全に分離
	int model_handle_;     ///< 描画やリソース管理に使用するハンドル
	int collision_handle_; ///< 描画やリソース管理に使用するハンドル
};
