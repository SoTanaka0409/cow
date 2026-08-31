#pragma once
#include "DxLib.h"
#include <string>

/// @brief 外部リソース（MV1ファイル）の読み込みとDXライブラリ固有の描画・姿勢制御処理をカプセル化するクラス
/// @details 多重ロードによるVRAM枯渇を防ぐため、内部では必ずResourceManager経由でリソースを共有化すること
class Model
{
public:
	/// @brief 生成毎の同期ロードによるスパイク（処理落ち）を防ぐため、極力シーン初期化時にインスタンス化を済ませること
	/// @param filename モデルパス
	/// @param initPos 初期座標
	/// @param isSeparateAnimation アニメ分離フラグ [出力] なし [副作用] リソースマネージャーからのハンドル取得
	Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);

	/// @brief ResourceManagerで管理されない独自確保のリソース（差し替えテクスチャ等）のVRAMリークをここで確実に防ぐこと
	/// @details なし [出力] なし [副作用] 動的変更したテクスチャハンドルの解放
	~Model();

	/// @brief 当たり判定と描画モデルの1フレーム遅延（ガタつきバグ）を防ぐため、全座標計算が完了したUpdateの終盤で呼ぶこと
	/// @details なし [出力] なし [副作用] クラス内の座標・回転・スケール値のDXライブラリ側ハンドルへの一括適用
	void Update();

	/// @brief 半透明マテリアルを含むモデルの場合、Zバッファの書き込み仕様による描画破綻を避けるため呼び出し順（奥から手前）に注意
	/// @details なし [出力] なし [副作用] バックバッファへの3Dモデル描画
	void Draw();

	VECTOR GetPosition() { return position_; }
	void SetPosition(VECTOR pos) { position_ = pos; }

	VECTOR GetRotation() { return rotation_; }
	void SetRotation(VECTOR rot) { rotation_ = rot; }

	/// @brief 非等倍スケールは物理演算や当たり判定のスケール（半径など）との乖離を生みやすいため、適用時は判定側も同期させること
	/// @param scale 各軸の拡大率 [出力] なし [副作用] 内部スケール変数の更新
	void SetScale(VECTOR scale);

	/// @brief 値を設定する
	/// @param scale 拡大倍率 [出力] なし [副作用] 内部スケール変数の更新
	void SetScale(float scale);

	/// @brief 毎フレーム呼ぶと深刻なメモリリークと処理落ちを招くため、状態変化時（ダメージ演出など）の1回のみ実行するようガードすること
	/// @param filename 画像パス
	/// @param index マテリアル番号 [出力] なし [副作用] VRAMへの画像ロードとハンドル差し替え
	void SetTexture(std::string filename, int index = 0);

	/// @brief マテリアル設定を強制上書きするため、元モデルに焼き付けられた色情報が意図せず消失するリスクに注意して利用すること
	/// @param a カラー成分(0.0～1.0) [出力] なし [副作用] モデルマテリアルのディフューズ色変更
	/// @details r
	/// @details g
	/// @details b
	void SetColor(float r, float g, float b, float a = 1.0f);

private:

	int handle_;      ///< 描画やリソース管理に使用するハンドル
	VECTOR position_; ///< 座標や位置情報を管理する値
	VECTOR rotation_; ///< 移動や回転の計算に使用する値
	float scale_;     ///< サイズや範囲の計算に使用する値

	/// @brief 動的に差し替えたテクスチャハンドルの解放漏れ（VRAMリーク）を防ぐため、破棄時に参照して始末するための保持変数
	int change_texture_handle_; ///< 描画やリソース管理に使用するハンドル
};
