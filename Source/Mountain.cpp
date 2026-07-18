#include "Mountain.h"

/*
 * 背景演出用の静的モデルを構築するため
 * [入力] filename: モデルのファイルパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度
 * [出力] なし
 * [副作用] model_に新しくModelインスタンスが確保される
 */
Mountain::Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle)
	: Object3D(initPos)
{
	model_ = new Model(filename, initPos);
	model_->SetScale(scale);
	model_->SetRotation(angle);
}

/*
 * メモリリークを防ぐため
 * [入力] なし
 * [出力] なし
 * [副作用] model_のメモリが解放される
 */
Mountain::~Mountain()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
}

/*
 * 状態を最新に保つため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Mountain::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

/*
 * 画面にモデルを可視化するため
 * [入力] なし
 * [出力] なし
 * [副作用] 画面に描画される
 */
void Mountain::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

/*
 * モデルの大きさを柔軟に変更するため
 * [入力] scale: 新しい拡大率
 * [出力] なし
 * [副作用] スケール状態が更新される
 */
void Mountain::SetScale(VECTOR scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

/*
 * モデルの向きを柔軟に変更するため
 * [入力] angle: 新しい回転角度
 * [出力] なし
 * [副作用] 回転状態が更新される
 */
void Mountain::SetRotation(VECTOR angle)
{
	if (model_ != nullptr)
	{
		model_->SetRotation(angle);
	}
}

/*
 * 時間帯などの環境変化に合わせて色を変えるため
 * [入力] r: 赤, g: 緑, b: 青, a: アルファ値
 * [出力] なし
 * [副作用] 色状態が更新される
 */
void Mountain::SetColor(float r, float g, float b, float a)
{
	if (model_ != nullptr)
	{
		model_->SetColor(r, g, b, a);
	}
}
