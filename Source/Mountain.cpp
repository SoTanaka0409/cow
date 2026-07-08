#include "Mountain.h"

Mountain::Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle)
	: Object3D(initPos)
{
	mpModel = new Model(filename, initPos);
	mpModel->SetScale(scale);
	mpModel->SetRotation(angle);
}

Mountain::~Mountain()
{
	// メモリリーク防止のため、動的に生成したModelインスタンスを破棄する
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void Mountain::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}

void Mountain::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
}

void Mountain::SetScale(VECTOR scale)
{
	if (mpModel != nullptr)
	{
		mpModel->SetScale(scale);
	}
}

void Mountain::SetRotation(VECTOR angle)
{
	if (mpModel != nullptr)
	{
		mpModel->SetRotation(angle);
	}
}

void Mountain::SetColor(float r, float g, float b, float a)
{
	if (mpModel != nullptr)
	{
		mpModel->SetColor(r, g, b, a);
	}
}

