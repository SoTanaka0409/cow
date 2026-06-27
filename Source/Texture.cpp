#include "Texture.h"
#include "DxLib.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag)
	: handle(-1)
	, mvPosition(centerPosition)
	, newGameW(graphsize_x)
	, newGameH(graphsize_y)
	, transFlag(transFlag)
{
	handle = LoadGraph(filename.c_str());
	GetGraphSize(handle, &sizeX, &sizeY); // 拡大縮小描画の基準にするためオリジナルサイズを取得
}

Texture::~Texture()
{
	DeleteGraph(handle);
}

/*
	* @brief 設定された指定サイズ(newGameW, newGameH)で、mvPositionを中心に引き伸ばし描画する
	* [入力] なし
	* [出力] なし
	* [副作用] 指定範囲へテクスチャ描画
	*/
void Texture::Draw()
{
	DrawExtendGraph(
		static_cast<int>(mvPosition.x - (newGameW / 2)),
		static_cast<int>(mvPosition.y - (newGameH / 2)),
		static_cast<int>(mvPosition.x + (newGameW / 2)),
		static_cast<int>(mvPosition.y + (newGameH / 2)),
		handle, transFlag
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
	int halfW = (newGameW + expand) / 2;
	int halfH = (newGameH + expand) / 2;

	DrawExtendGraph(
		static_cast<int>(mvPosition.x - halfW),
		static_cast<int>(mvPosition.y - halfH),
		static_cast<int>(mvPosition.x + halfW),
		static_cast<int>(mvPosition.y + halfH),
		handle, transFlag
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
	float halfW = (newGameW * scale) / 2.0f;
	float halfH = (newGameH * scale) / 2.0f;

	DrawExtendGraph(
		static_cast<int>(mvPosition.x - halfW),
		static_cast<int>(mvPosition.y - halfH),
		static_cast<int>(mvPosition.x + halfW),
		static_cast<int>(mvPosition.y + halfH),
		handle, transFlag
	);
}

