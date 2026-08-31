#include "SkyBox.h"
#include "Model.h"

/// @brief 初期化処理を行う
/// @details filename (モデルパス)
/// @details pos (初期座標)
/// @details スカイボックス用3Dモデルの動的生成
SkyBox::SkyBox(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	model_ = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

/// @brief 確保したモデルリソースの解放
SkyBox::~SkyBox()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
}

/// @brief モデルの状態更新
void SkyBox::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

/// @brief ライティング状態の一時切り替えと3Dモデルの描画
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

/// @brief 値を設定する
/// @details scale (スケール倍率)
/// @details スカイボックスモデルの拡縮適用
void SkyBox::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

/// @brief 値を設定する
/// @details filename (テクスチャパス)
/// @details index (対象マテリアルのインデックス)
/// @details スカイボックスのテクスチャ差し替え
void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (model_ != nullptr)
	{
		model_->SetTexture(filename, index);
	}
}
