#pragma once
class Model;

#include "Object3D.h"

// 背景の空（スカイボックスモデル）を管理・描画するクラス
class SkyBox : public Object3D
{
public:
	/*
	 * @brief スカイボックス用のモデルと初期座標を指定してインスタンスを生成する
	 * [入力] filename: スカイボックスのモデルファイルパス, pos: 初期座標
	 * [出力] なし
	 * [副作用] スカイボックス用Modelクラスのインスタンス生成
	 */
	SkyBox(std::string filename, VECTOR pos);
	virtual ~SkyBox();

	/*
	 * @brief スカイボックスのトランスフォーム更新処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 内部モデルの位置情報の更新
	 */
	void Update() override;

	/*
	 * @brief ライティング計算を一時的に無効化し、空のモデルを明るく描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	/*
	 * @brief 空モデル全体の表示スケール（大きさ）を設定する
	 * [入力] scale: 拡大率
	 * [出力] なし
	 * [副作用] なし
	 */
	void SetScale(float scale);

	/*
	 * @brief スカイボックスモデルのテクスチャを切り替える
	 * [入力] filename: テクスチャ画像パス, index: マテリアルインデックス
	 * [出力] なし
	 * [副作用] なし
	 */
	void SetModelTexture(std::string filename, int index = 0);

private:
	Model* mpModel;          // スカイボックスを構成する3Dモデルへのポインタ
};
