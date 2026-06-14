#pragma once
#include "DxLib.h"
#include <string>

// 3Dモデル（MV1形式）の読み込み、描画、座標・スケール・テクスチャ制御をカプセル化するクラス
class Model
{
public:
	/*
	 * @brief 3Dモデルファイルを読み込み、初期座標でインスタンスを生成する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標, isSeparateAnimation: アニメーション分離フラグ
	 * [出力] なし
	 * [副作用] リソースマネージャー経由でモデルハンドルが確保される
	 */
	Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);
	~Model();   

	/*
	 * @brief モデルのトランスフォーム情報をDXライブラリのモデルハンドルに適用する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update();  

	/*
	 * @brief 3Dモデルを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 3D空間への描画が行われる
	 */
	void Draw();    

	VECTOR GetPosition() { return mvPosition; } 
	void SetPosition(VECTOR pos) { mvPosition = pos; }  

	VECTOR GetRotation() { return mvRotation; } 
	void SetRotation(VECTOR rot) { mvRotation = rot; }  

	/*
	 * @brief モデルの拡大率を3軸個別に設定する
	 * [入力] scale: 各軸の拡大率
	 * [出力] なし
	 * [副作用] モデルの表示サイズが変化する
	 */
	void SetScale(VECTOR scale);

	/*
	 * @brief モデルの拡大率を均等に設定する
	 * [入力] scale: 拡大倍率
	 * [出力] なし
	 * [副作用] なし
	 */
	void SetScale(float scale);

	/*
	 * @brief モデルのテクスチャ画像を差し替える
	 * [入力] filename: 画像のファイルパス, index: マテリアルのインデックス番号
	 * [出力] なし
	 * [副作用] 既存のテクスチャハンドルがあれば解放され、新テクスチャが割り当てられる
	 */
	void SetTexture(std::string filename, int index = 0);

	/*
	 * @brief モデルのディフューズ（拡散）カラーを設定する
	 * [入力] r, g, b, a: カラー成分(0.0?1.0)
	 * [出力] なし
	 * [副作用] モデルの反射光の色が変更される
	 */
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	int mnHandle;                 // DXライブラリの3Dモデルハンドル
	VECTOR mvPosition;            // 配置座標
	VECTOR mvRotation;            // 回転値（ラジアン）
	float mfScale;                // スケール
	int mnChangeTextureHandle;    // テクスチャ差し替え時のグラフィックハンドル
};

