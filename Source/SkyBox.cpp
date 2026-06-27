#include "SkyBox.h"
#include "Model.h"

SkyBox::SkyBox(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	// スカイボックスモデルの配置初期化
	model = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

SkyBox::~SkyBox()
{
	if (model != nullptr)
	{
		delete model;
	}
}

void SkyBox::Update()
{
	if (model != nullptr)
	{
		model->Update();
	}
}

void SkyBox::Draw()
{
	if (model != nullptr)
	{
		// 空モデルが光源計算や周囲の影の影響を受けて暗く沈まないように、ライティングを無効化して描画する
		SetUseLighting(FALSE);
		model->Draw();
		SetUseLighting(TRUE);
	}
}

void SkyBox::SetScale(float scale)
{
	if (model != nullptr)
	{
		model->SetScale(scale);
	}
}

void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (model != nullptr)
	{
		model->SetTexture(filename, index);
	}
}

