#pragma once
#include "DxLib.h"
#include "Object3D.h"

// 3D空間上でテクスチャを用いたシンプルなパーティクル（爆発や火花など）を放出・制御するエフェクトクラス
class Effect : public Object3D
{
public:
	static const int PARTICLE_NUM = 256; // 1つのエフェクトが放出する最大パーティクル数

	// 個々のパーティクルの物理・描画状態
	struct ParticleInfo
	{
		VECTOR pos;         // 現在の位置座標
		VECTOR dir;         // 移動方向の単位ベクトル
		float speed;        // 移動速度
		float size;         // 描画サイズ（スケール値）
		float alpha;        // 不透明度 (0.0 〜 1.0)
		float visibleTime;  // 完全に消滅を開始するまでの残り表示維持時間
	};

	// エフェクト全体のカラーおよび内包する全パーティクルの情報
	struct EffectInfo
	{
		COLOR_U8 color;                      // パーティクルに適用するブレンドカラー
		ParticleInfo particle[PARTICLE_NUM]; // 放出された各パーティクルのデータ配列
	};

public:
	/*
	 * @brief 指定されたテクスチャ、色、サイズ、表示時間でパーティクルをランダムな方向へ256個放出する
	 * [入力] initPos: 放出中心座標, filename: 使用する画像パス, Changecolor: ブレンドカラー, Size: パーティクルサイズ, VisibleTime: 残存時間
	 * [出力] なし
	 * [副作用] EffectInfo構造体の動的メモリ確保、テクスチャのロード
	 */
	Effect(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime);

	/*
	 * @brief ロードしたグラフィックハンドルおよび動的配列データを解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] グラフィックのアンロード、EffectInfoの破棄
	 */
	~Effect();

	/*
	 * @brief 全パーティクルの移動更新、速度減衰、および寿命経過によるアルファ値の減衰を処理する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] パーティクル座標・アルファ値の更新、全消滅時の自身への削除フラグ設定
	 */
	void Update() override;

	/*
	 * @brief 3Dビルボードを用いて、加算合成および反転転送ブレンドでパーティクル群を描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] Zバッファ書き込み設定の一時的変更、描画ブレンドモードの変更
	 */
	void Draw() override;

private:
	int mnGraphHandle;    // 描画に使用するパーティクルテクスチャハンドル
	EffectInfo* mpEffect; // 動的に確保されたエフェクト構造体へのポインタ

	const int SPEED_RAND_MAX = 550; // パーティクル初期速度ランダム範囲上限
	const int SPEED_RAND_MIN = 200; // パーティクル初期速度ランダム範囲下限

	const int SIZE_RAND_MAX = 1600; // パーティクルサイズランダム範囲上限（未使用）
	const int SIZE_RAND_MIN = 800;  // パーティクルサイズランダム範囲下限（未使用）

	const int VISIBLE_TIME_RAND_MAX = 30; // 表示維持時間ランダム範囲上限（未使用）
	const int VISIBLE_TIME_RAND_MIN = 5;  // 表示維持時間ランダム範囲下限（未使用）
};
