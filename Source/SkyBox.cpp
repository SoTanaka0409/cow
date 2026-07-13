#include "SkyBox.h"
#include "Model.h"

/*
 * 入力: filename (モデルパス), pos (初期座標)
 * 出力: なし
 * 副作用: スカイボックス用3Dモデルの動的生成
 */
SkyBox::SkyBox(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	model_ = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 確保したモデルリソースの解放
 */
SkyBox::~SkyBox()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: モデルの状態更新
 */
void SkyBox::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: ライティング状態の一時切り替えと3Dモデルの描画
 */
void SkyBox::Draw()
{
	if (model_ != nullptr)
	{
		// 天球が光源や影の影響を受けて不自然に暗くなる（昼間なのに空が黒ずむ等）描画バグを防ぐため、
		// スカイボックスの描画時のみライティングの影響を完全に無効化する
		SetUseLighting(FALSE);
		model_->Draw();
		SetUseLighting(TRUE);
	}
}

/*
 * 入力: scale (スケール倍率)
 * 出力: なし
 * 副作用: スカイボックスモデルの拡縮適用
 */
void SkyBox::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

/*
 * 入力: filename (テクスチャパス), index (対象マテリアルのインデックス)
 * 出力: なし
 * 副作用: スカイボックスのテクスチャ差し替え
 */
void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (model_ != nullptr)
	{
		model_->SetTexture(filename, index);
	}
}