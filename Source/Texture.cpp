#include "Texture.h"
#include "DxLib.h"
#include "Master.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag)
	: mnHandle(-1)
	, position_(centerPosition)
	, new_game_w_(graphsize_x)
	, new_game_h_(graphsize_y)
	, mnTransFlag(transFlag)
{
	mnHandle = Master::mpResourceManager->LoadGraphics(filename.c_str());
	GetGraphSize(mnHandle, &mnSizeX, &mnSizeY); // 拡大縮小描画の基準にするためオリジナルサイズを取征E
}

Texture::~Texture()
{

}

/*
	* @brief 設定された持E??サイズ(new_game_w_, new_game_h_)で、mvPositionを中?E??引き伸ばし描画する
	* [入力] なし
	* [出力] なし
	* [副作用] 持E??篁E??へチE??スチャ描画
	*/
void Texture::Draw()
{
	DrawExtendGraph(
		static_cast<int>(position_.x - (new_game_w_ / 2)),
		static_cast<int>(position_.y - (new_game_h_ / 2)),
		static_cast<int>(position_.x + (new_game_w_ / 2)),
		static_cast<int>(position_.y + (new_game_h_ / 2)),
		mnHandle, mnTransFlag
	);
}

/*
	* @brief ボタンホバー演?E用などに、E??常サイズに特定?E拡張?Eexpand)を加えて拡大描画する?E?現在未使用?E?E
	* [入力] なし
	* [出力] なし
	* [副作用] 持E??された拡張領域へ拡大描画
	*/
void Texture::SizeDraw()
{
	int expand = 15; // 拡大表示する際?E拡張ピクセル数
	int halfW = (new_game_w_ + expand) / 2;
	int halfH = (new_game_h_ + expand) / 2;

	DrawExtendGraph(
		static_cast<int>(position_.x - halfW),
		static_cast<int>(position_.y - halfH),
		static_cast<int>(position_.x + halfW),
		static_cast<int>(position_.y + halfH),
		mnHandle, mnTransFlag
	);
}

void Texture::Update()
{
}

/*
	* @brief 中?E??標を軸にして、比率持E??Escale)による拡大縮小描画を行う
	* [入力] scale: スケーリング倍率?E?E.0fが等倍！E
	* [出力] なし
	* [副作用] 持E??サイズでチE??スチャ描画
	*/
void Texture::DrawScale(float scale)
{
	float halfW = (new_game_w_ * scale) / 2.0f;
	float halfH = (new_game_h_ * scale) / 2.0f;

	DrawExtendGraph(
		static_cast<int>(position_.x - halfW),
		static_cast<int>(position_.y - halfH),
		static_cast<int>(position_.x + halfW),
		static_cast<int>(position_.y + halfH),
		mnHandle, mnTransFlag
	);
}
