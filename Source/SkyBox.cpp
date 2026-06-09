#include "SkyBox.h"
#include "Model.h"

SkyBox::SkyBox(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	// スカイボックスモデルの配置初期化
	mpModel = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

SkyBox::~SkyBox()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}

void SkyBox::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}

void SkyBox::Draw()
{
	if (mpModel != nullptr)
	{
		// 空モデルが光源計算や周囲の影の影響を受けて暗く沈まないように、ライティングを無効化して描画する
		SetUseLighting(FALSE);
		mpModel->Draw();
		SetUseLighting(TRUE);
	}
}

void SkyBox::SetScale(float scale)
{
	if (mpModel != nullptr)
	{
		mpModel->SetScale(scale);
	}
}

void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (mpModel != nullptr)
	{
		mpModel->SetTexture(filename, index);
	}
}
