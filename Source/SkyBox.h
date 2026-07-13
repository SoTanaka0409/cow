#pragma once
class Model;

#include "Object3D.h"

// 設計ルール：プレイヤーの移動に追従し、世界の広がりを演出するための巨大な天球（背景モデル）を管理するクラス
class SkyBox : public Object3D
{
public:
	// 入力：filename=スカイボックスのモデルファイルパス, pos=初期座標
	// 副作用：天球用Modelクラスのインスタンス生成
	SkyBox(std::string filename, VECTOR pos);
	virtual ~SkyBox();

	// 副作用：天球モデルのトランスフォーム（位置・回転・縮尺）行列の更新
	void Update() override;

	// 副作用：3D空間への背景描画
	// パフォーマンス理由：光源計算による陰影が空に発生して不自然に見えるのを防ぐため、一時的にライティングを無効化して描画する
	void Draw() override;

	// 入力：scale=拡大率
	void SetScale(float scale);

	// 入力：filename=テクスチャ画像パス, index=マテリアルインデックス
	// 業務ルール：時間経過や天候変化（昼・夜・フェーバータイム等）に応じて、空のテクスチャを動的に差し替える
	void SetModelTexture(std::string filename, int index = 0);

private:
	// バグ回避：カメラが天球のポリゴン外に飛び出して背景が消失するのを防ぐため、プレイヤーの現在座標を毎フレーム追従させる
	Model* model_;
};