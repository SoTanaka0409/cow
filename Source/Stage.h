#pragma once
#include "Object3D.h"
#include <string>

// 背景描画と物理判定を分離して最適化するため、2つのモデルを同期管理する
class Stage : public Object3D
{
public:
	/*
	 * @brief 地形描画モデルと衝突判定用モデルをそれぞれ読み込んで配置する
	 * [入力] initPos: 配置初期座標, StageModelName: 地形描画用モデルファイルパス, stageCollsionModelName: 衝突判定用モデルファイルパス
	 * [出力] なし
	 * [副作用] 各モデルのロード、スケール変更、コリジョン情報の構築が行われる
	 */
	Stage(VECTOR initPos, std::string StageModelName, std::string stageCollsionModelName);
	virtual ~Stage();

	void Update() override;
	void Draw() override;

	/*
	 * @brief 引数で指定されたカプセル領域とステージ地形との交差を判定し、衝突箇所をデバッグ描画する
	 * [入力] pos1, pos2: カプセルの始点・終点座標, r: カプセルの半径
	 * [出力] 交差した場合はtrue、そうでない場合はfalse
	 * [副作用] 交差したポリゴンが3Dデバッグ描画される
	 */
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	/*
	 * @brief 指定された線分とステージ地形との交差位置を計算して返す
	 * [入力] pos1, pos2: 線分の始点・終点座標
	 * [出力] 交差した位置座標（交差していない場合は(0,0,0)）
	 * [副作用] なし
	 */
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

	/*
	 * @brief 指定された線分とステージ地形との交差判定を行い、結果をデバッグ表示する
	 * [入力] pos1, pos2: 線分の始点・終点座標
	 * [出力] 交差した位置座標（交差していない場合は(0,0,0)）
	 * [副作用] 画面上に交差座標のデバッグテキストが描画される
	 */
	VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);

private:
	int mnModelHandle;
	int mnCollisionHandle;
};
