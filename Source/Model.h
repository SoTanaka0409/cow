#pragma once
#include "DxLib.h"
#include <string>

// 外部リソース（MV1ファイル）の読み込みとDXライブラリ固有の描画・姿勢制御処理をカプセル化するクラス
// 多重ロードによるVRAM枯渇を防ぐため、内部では必ずResourceManager経由でリソースを共有化すること
class Model
{
public:
	// [入力] filename: モデルパス, initPos: 初期座標, isSeparateAnimation: アニメ分離フラグ [出力] なし [副作用] リソースマネージャーからのハンドル取得
	// 生成毎の同期ロードによるスパイク（処理落ち）を防ぐため、極力シーン初期化時にインスタンス化を済ませること
	Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);

	// [入力] なし [出力] なし [副作用] 動的変更したテクスチャハンドルの解放
	// ResourceManagerで管理されない独自確保のリソース（差し替えテクスチャ等）のVRAMリークをここで確実に防ぐこと
	~Model();

	// [入力] なし [出力] なし [副作用] クラス内の座標・回転・スケール値のDXライブラリ側ハンドルへの一括適用
	// 当たり判定と描画モデルの1フレーム遅延（ガタつきバグ）を防ぐため、全座標計算が完了したUpdateの終盤で呼ぶこと
	void Update();

	// [入力] なし [出力] なし [副作用] バックバッファへの3Dモデル描画
	// 半透明マテリアルを含むモデルの場合、Zバッファの書き込み仕様による描画破綻を避けるため呼び出し順（奥から手前）に注意
	void Draw();

	VECTOR GetPosition() { return position_; }
	void SetPosition(VECTOR pos) { position_ = pos; }

	VECTOR GetRotation() { return rotation_; }
	void SetRotation(VECTOR rot) { rotation_ = rot; }

	// [入力] scale: 各軸の拡大率 [出力] なし [副作用] 内部スケール変数の更新
	// 非等倍スケールは物理演算や当たり判定のスケール（半径など）との乖離を生みやすいため、適用時は判定側も同期させること
	void SetScale(VECTOR scale);

	// [入力] scale: 拡大倍率 [出力] なし [副作用] 内部スケール変数の更新
	void SetScale(float scale);

	// [入力] filename: 画像パス, index: マテリアル番号 [出力] なし [副作用] VRAMへの画像ロードとハンドル差し替え
	// 毎フレーム呼ぶと深刻なメモリリークと処理落ちを招くため、状態変化時（ダメージ演出など）の1回のみ実行するようガードすること
	void SetTexture(std::string filename, int index = 0);

	// [入力] r,g,b,a: カラー成分(0.0～1.0) [出力] なし [副作用] モデルマテリアルのディフューズ色変更
	// マテリアル設定を強制上書きするため、元モデルに焼き付けられた色情報が意図せず消失するリスクに注意して利用すること
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	
	int mnHandle;
	VECTOR position_;
	VECTOR rotation_;
	float mfScale;

	// 動的に差し替えたテクスチャハンドルの解放漏れ（VRAMリーク）を防ぐため、破棄時に参照して始末するための保持変数
	int mnChangeTextureHandle;
};