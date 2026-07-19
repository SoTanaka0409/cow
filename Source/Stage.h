#pragma once
#include "Object3D.h"
#include <string>

// 設計ルール：ポリゴン数の多い複雑な描画用モデルと、処理を軽量化した衝突判定用（コリジョン用）モデルを同期管理するクラス
class Stage : public Object3D
{
public:
	// 入力：initPos=配置初期座標, StageModelName=地形描画用モデルパス, stageCollsionModelName=衝突判定用モデルパス
	// 副作用：各種地形モデルハンドル（DxLib）のロード
	Stage(VECTOR initPos, std::string StageModelName, std::string stageCollsionModelName);
	virtual ~Stage();

	void Update() override;
	void Draw() override;

	// 入力：pos1, pos2=カプセルの始点・終点座標, r=カプセルの半径
	// 出力：地形との衝突有無（真偽値）
	// 副作用：なし
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	// 入力：pos1, pos2=レイ（線分）の始点・終点座標
	// 出力：地形と交差したヒット位置の3D座標（非衝突時はゼロベクトル）
	// 業務ルール：UFOから真下に放つアブダクションビームや、牛の接地（高さ合わせ）の計算に使用する
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

	// 入力：pos1, pos2=レイ（線分）の始点・終点座標
	// 出力：地形と交差したヒット位置の3D座標（非衝突時はゼロベクトル）
	// 副作用：ゲーム画面上にレイの軌跡と交差点の座標テキストをデバッグ表示
	VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);

private:
	// パフォーマンス理由：描画負荷と物理演算（メッシュ衝突判定）の計算負荷をそれぞれ最適化するため、ハンドルを完全に分離
	int model_handle_;
	int collision_handle_;
};
