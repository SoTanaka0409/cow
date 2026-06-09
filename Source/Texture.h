#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

// 2D画像アセットを読み込み、位置やサイズ、スケーリングを指定して画面描画を行うクラス
class Texture
{
public:
	Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag);
	~Texture();

	/*
	 * @brief テクスチャを mvPosition を中心に指定サイズで描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面（バックバッファ）に画像を描画
	 */
	void Draw();

	/*
	 * @brief テクスチャを通常サイズより少し拡大した状態で中心描画する（ボタンホバー用など、現在未使用）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面に拡大画像を描画
	 */
	void SizeDraw();

	/*
	 * @brief テクスチャの毎フレームの更新処理（現在処理なし）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update();

	/*
	 * @brief テクスチャを任意のスケール比率で中心描画する
	 * [入力] scale: スケーリング倍率（1.0fが等倍）
	 * [出力] なし
	 * [副作用] 画面に拡大/縮小した画像を描画
	 */
	void DrawScale(float scale);

	// セッター・ゲッター群
	void SetPosition(VECTOR centerPosition) { mvPosition = centerPosition; }
	VECTOR GetPosition() { return mvPosition; }
	int GetSizeX() { return mnSizeX; }
	int GetSizeY() { return mnSizeY; }
	int GetWidth() { return mNewGameW; }
	int GetHeight() { return mNewGameH; }

private:
	int mnHandle;       // ロードした画像のグラフィックハンドル
	VECTOR mvPosition;  // 画面描画時の中心座標
	int mnSizeX;        // 画像ファイル本来の横幅
	int mnSizeY;        // 画像ファイル本来の縦幅
	bool mnTransFlag;   // 背景の透過（透過チャンネル）を有効にするかどうかのフラグ

	int mNewGameW;      // 指定された描画用の横幅
	int mNewGameH;      // 指定された描画用の縦幅
};

#endif
