#include "Texture.h"
#include "DxLib.h"
#include "Master.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag)
	: mnHandle(-1)
	, mvPosition(centerPosition)
	, mNewGameW(graphsize_x)
	, mNewGameH(graphsize_y)
	, mnTransFlag(transFlag)
{
	mnHandle = Master::mpResourceManager->LoadGraphics(filename.c_str());
	GetGraphSize(mnHandle, &mnSizeX, &mnSizeY); // 拡大縮小描画の基準にするためオリジナルサイズを取得
}

Texture::~Texture()
{

}

/*
	* @brief 設定された指定サイズ(mNewGameW, mNewGameH)で、mvPositionを中心に引き伸ばし描画する
	* [入力] なし
	* [出力] なし
	* [副作用] 指定範囲へテクスチャ描画
	*/
void Texture::Draw()
{
	DrawExtendGraph(
		static_cast<int>(mvPosition.x - (mNewGameW / 2)),
		static_cast<int>(mvPosition.y - (mNewGameH / 2)),
		static_cast<int>(mvPosition.x + (mNewGameW / 2)),
		static_cast<int>(mvPosition.y + (mNewGameH / 2)),
		mnHandle, mnTransFlag
	);
}

/*
	* @brief ボタンホバー演出用などに、通常サイズに特定の拡張幅(expand)を加えて拡大描画する（現在未使用）
	* [入力] なし
	* [出力] なし
	* [副作用] 指定された拡張領域へ拡大描画
	*/
void Texture::SizeDraw()
{
	int expand = 15; // 拡大表示する際の拡張ピクセル数
	int halfW = (mNewGameW + expand) / 2;
	int halfH = (mNewGameH + expand) / 2;

	DrawExtendGraph(
		static_cast<int>(mvPosition.x - halfW),
		static_cast<int>(mvPosition.y - halfH),
		static_cast<int>(mvPosition.x + halfW),
		static_cast<int>(mvPosition.y + halfH),
		mnHandle, mnTransFlag
	);
}

void Texture::Update()
{
}

/*
	* @brief 中心座標を軸にして、比率指定(scale)による拡大縮小描画を行う
	* [入力] scale: スケーリング倍率（1.0fが等倍）
	* [出力] なし
	* [副作用] 指定サイズでテクスチャ描画
	*/
void Texture::DrawScale(float scale)
{
	float halfW = (mNewGameW * scale) / 2.0f;
	float halfH = (mNewGameH * scale) / 2.0f;

	DrawExtendGraph(
		static_cast<int>(mvPosition.x - halfW),
		static_cast<int>(mvPosition.y - halfH),
		static_cast<int>(mvPosition.x + halfW),
		static_cast<int>(mvPosition.y + halfH),
		mnHandle, mnTransFlag
	);
}

