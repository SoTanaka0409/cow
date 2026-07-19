#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

// 設計ルール：テクスチャファイルの重複ロードを防ぎ、基準点を「中心」に統一して直感的な2Dレイアウトを行うための画像描画クラス
class Texture
{
public:
	// 入力：filename=画像パス, centerPosition=中心描画の基準座標, graphsize_x/y=指定描画サイズ, transFlag=透過有無
	// 副作用：DxLibによる画像リソースの読み込みとグラフィックハンドルの生成
	Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag);
	~Texture();

	// 副作用：バックバッファへの画像描画
	// 設計ルール：回転やスケール変更の基準点を制御しやすくするため、左上ではなく常に指定された「中心座標」を軸に描画する
	void Draw();

	// 副作用：バックバッファへの拡大画像描画
	// 一時対応：UIのボタンホバー時のポップアップ演出用として作成。現在リデザインに伴い一時的に未呼出し状態
	void SizeDraw();

	void Update();

	// 入力：scale=拡大縮小率（1.0fが等倍）
	// 副作用：バックバッファへの変形描画
	void DrawScale(float scale);

	void SetPosition(VECTOR centerPosition) { position_ = centerPosition; }
	VECTOR GetPosition() { return position_; }
	int GetSizeX() { return size_x_; }
	int GetSizeY() { return size_y_; }
	int GetWidth() { return new_game_w_; }
	int GetHeight() { return new_game_h_; }

private:
	int handle_;
	VECTOR position_;
	int size_x_;
	int size_y_;
	bool trans_flag_;

	// 仕様制約：アセット本来の解像度とは別に、画面レイアウト上のUI枠に合わせて強制拡縮して描画するための指定サイズ
	int new_game_w_;
	int new_game_h_;
};

#endif
