#pragma once
#include "Object3D.h"
#include <vector>
#include <string>

/// @brief ステージの基盤となる床面描画クラス。3Dモデルではなく直接ポリゴンを描画する設計
/// @details カリング（裏面非描画）の仕様に依存するため、頂点定義の順序（時計回り等）に注意すること
class Floor : public Object3D
{
public:
	/// @brief 毎フレームのロードによる深刻なパフォーマンス低下を防ぐため、初期化時に一度だけ読み込み保持する
	/// @param filename テクスチャパス
	/// @param centerPos 中心座標
	/// @details topLeft/bottomRight: 矩形サイズ [出力] なし [副作用] VRAMへのテクスチャロード
	Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);

	/// @brief VRAMリークを避けるため、DXライブラリのDeleteGraphを確実に呼び出して解放すること
	/// @details なし [出力] なし [副作用] graph_handle_の解放
	virtual ~Floor();

	/// @brief 床の動的移動やUVアニメーション等の計算用。処理負荷軽減のため、静的オブジェクトの場合は空実装とする
	/// @details なし [出力] なし [副作用] 座標やUVスクロールの計算
	void Update() override;

	/// @brief DrawPolygon3Dによる描画。半透明テクスチャを使用する場合、Zバッファ書き込み順序による描画破綻に注意
	/// @details なし [出力] なし [副作用] バックバッファへのポリゴン描画
	void Draw() override;

private:
	/// @brief 大量生成時のVRAM枯渇を防ぐため、複数床で同画像を使い回す場合は外部のテクスチャ管理機構を通すよう今後要回収
	int graph_handle_; ///< 描画やリソース管理に使用するハンドル

	/// @brief 描画負荷軽減のため3Dモデルファイルを経由せず直接生成する。左上からZ字順などの頂点配列ルールを絶対に崩さないこと
	VERTEX3D vertex_[4]; ///< 描画や当たり判定に使用する頂点情報
};
