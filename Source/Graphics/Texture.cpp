#include "Texture.h"
#include "DxLib.h"
#include "Master.h"

// 入力：filename=アセット画像パス, centerPosition=描画の中心となる座標, graphsize_x/y=指定描画サイズ, transFlag=透過の有無
// 副作用：ResourceManagerを介したグラフィックハンドルのロード、およびオリジナル画像解像度の取得
Texture::Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag)
	: handle_(-1)
	, position_(centerPosition)
	, new_game_w_(graphsize_x)
	, new_game_h_(graphsize_y)
	, trans_flag_(transFlag)
{
	handle_ = Master::resource_manager_->LoadGraphics(filename.c_str());
	// アセット自体の元解像度を保持しておき、将来的にアスペクト比を維持した自動リサイズ処理等に拡張できるようにする
	GetGraphSize(handle_, &size_x_, &size_y_);
}

Texture::~Texture()
{
}

// 入力：なし
// 出力：なし
// 副作用：バックバッファへの拡大縮小描画
void Texture::Draw()
{
	// 設計ルール：回転や拡縮のアニメーション基準点を直感的に制御するため、左上基準ではなく指定の中心座標から逆算して描画
	DrawExtendGraph(
		static_cast<int>(position_.x - (new_game_w_ / 2)),
		static_cast<int>(position_.y - (new_game_h_ / 2)),
		static_cast<int>(position_.x + (new_game_w_ / 2)),
		static_cast<int>(position_.y + (new_game_h_ / 2)),
		handle_, trans_flag_
	);
}

// 入力：なし
// 出力：なし
// 副作用：指定の拡張幅（15px）を加えたサイズでのバックバッファ描画
void Texture::SizeDraw()
{
	// 一時対応：UIの決定アニメーション等で、一時的に現在の見た目を少し強調（ポップアップ）させるための拡大処理
	int expand = 15;
	int halfW = (new_game_w_ + expand) / 2;
	int halfH = (new_game_h_ + expand) / 2;

	DrawExtendGraph(
		static_cast<int>(position_.x - halfW),
		static_cast<int>(position_.y - halfH),
		static_cast<int>(position_.x + halfW),
		static_cast<int>(position_.y + halfH),
		handle_, trans_flag_
	);
}

void Texture::Update()
{
}

// 入力：scale=拡縮倍率（1.0fが指定サイズでの等倍）
// 出力：なし
// 副作用：指定倍率でスケーリングされたバックバッファ描画
void Texture::DrawScale(float scale)
{
	// レベルアップ時のカードUIが滑らかに出現・拡大フェードする演出（サイン波補間）をピクセル単位で正確に描画するための小数演算
	float halfW = (new_game_w_ * scale) / 2.0f;
	float halfH = (new_game_h_ * scale) / 2.0f;

	DrawExtendGraph(
		static_cast<int>(position_.x - halfW),
		static_cast<int>(position_.y - halfH),
		static_cast<int>(position_.x + halfW),
		static_cast<int>(position_.y + halfH),
		handle_, trans_flag_
	);
}
