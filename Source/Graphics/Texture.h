#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

/// @brief 設計ルール：テクスチャファイルの重複ロードを防ぎ、基準点を「中心」に統一して直感的な2Dレイアウトを行うための画像描画クラス
class Texture
{
public:
	/// @param filename 画像パス
	/// @param centerPosition 中心描画の基準座標
	/// @param transFlag 透過有無
	/// @brief graphsize_x/y=指定描画サイズ
	/// @details DxLibによる画像リソースの読み込みとグラフィックハンドルの生成
	Texture(const std::string& filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag);
	~Texture();

	/// @brief 設計ルール：回転やスケール変更の基準点を制御しやすくするため、左上ではなく常に指定された「中心座標」を軸に描画する
	/// @details バックバッファへの画像描画
	void Draw();

	/// @brief 一時対応：UIのボタンホバー時のポップアップ演出用として作成。現在リデザインに伴い一時的に未呼出し状態
	/// @details バックバッファへの拡大画像描画
	void SizeDraw();

	/// @brief 毎フレームの更新処理
	void Update();

	/// @param scale 拡大縮小率（1.0fが等倍）
	/// @brief バックバッファへの変形描画
	void DrawScale(float scale);

	/// @brief Positionの設定
	void SetPosition(VECTOR centerPosition) { position_ = centerPosition; }
	/// @brief Positionの取得
	VECTOR GetPosition() const { return position_; }
	/// @brief SizeXの取得
	int GetSizeX() const { return size_x_; }
	/// @brief SizeYの取得
	int GetSizeY() const { return size_y_; }
	/// @brief Widthの取得
	int GetWidth() const { return new_game_w_; }
	/// @brief Heightの取得
	int GetHeight() const { return new_game_h_; }

private:
	int handle_;      ///< 描画やリソース管理に使用するハンドル
	VECTOR position_; ///< 座標や位置情報を管理する値
	int size_x_;      ///< サイズや範囲の計算に使用する値
	int size_y_;      ///< サイズや範囲の計算に使用する値
	bool trans_flag_; ///< 状態の有効・無効を管理するフラグ

	/// @brief 仕様制約：アセット本来の解像度とは別に、画面レイアウト上のUI枠に合わせて強制拡縮して描画するための指定サイズ
	int new_game_w_; ///< 内部状態を管理する値
	int new_game_h_; ///< 内部状態を管理する値
};

#endif
