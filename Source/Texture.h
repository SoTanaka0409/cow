#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

// 2D画像アセチE??を読み込み、位置めE??イズ、スケーリングを指定して画面描画を行うクラス
class Texture
{
public:
	Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag);
	~Texture();

	/*
	 * @brief チE??スチャめEposition_ を中?E??持E??サイズで描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面?E?バチE??バッファ?E?に画像を描画
	 */
	void Draw();

	/*
	 * @brief チE??スチャを通常サイズより少し拡大した状態で中?E??画する?E??ボタンホバー用など、現在未使用?E?E
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面に拡大画像を描画
	 */
	void SizeDraw();

	/*
	 * @brief チE??スチャの毎フレームの更新処?E??現在処?E??し！E
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update();

	/*
	 * @brief チE??スチャを任意?Eスケール比率で中?E??画する
	 * [入力] scale: スケーリング倍率?E?E.0fが等倍！E
	 * [出力] なし
	 * [副作用] 画面に拡大/縮小した画像を描画
	 */
	void DrawScale(float scale);

	// セチE??ー・ゲチE??ー群
	void SetPosition(VECTOR centerPosition) { position_ = centerPosition; }
	VECTOR GetPosition() { return position_; }
	int GetSizeX() { return mnSizeX; }
	int GetSizeY() { return mnSizeY; }
	int GetWidth() { return new_game_w_; }
	int GetHeight() { return new_game_h_; }

private:
	int mnHandle;       // ロードした画像?EグラフィチE??ハンドル
	VECTOR position_;  // 画面描画時?E中?E???E
	int mnSizeX;        // 画像ファイル本来の横?E
	int mnSizeY;        // 画像ファイル本来の縦?E
	bool mnTransFlag;   // 背景の透過?E?透過チャンネル?E?を有効にするかどぁE??のフラグ

	int new_game_w_;      // 持E??された描画用の横?E
	int new_game_h_;      // 持E??された描画用の縦?E
};

#endif
