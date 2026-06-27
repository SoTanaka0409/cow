#include "Mountain.h"

Mountain::Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle)
	: Object3D(initPos)
{
	model = new Model(filename, initPos);
	model->SetScale(scale);
	model->SetRotation(angle);
}

Mountain::~Mountain()
{
	// メモリリーク防止のため、動的に生成したModelインスタンスを破棄する
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void Mountain::Update()
{
	if (model != nullptr)
	{
		model->Update();
	}
}

void Mountain::Draw()
{
	if (model != nullptr)
	{
		model->Draw();
	}
}

void Mountain::SetScale(VECTOR scale)
{
	if (model != nullptr)
	{
		model->SetScale(scale);
	}
}

void Mountain::SetRotation(VECTOR angle)
{
	if (model != nullptr)
	{
		model->SetRotation(angle);
	}
}

void Mountain::SetColor(float r, float g, float b, float a)
{
	if (model != nullptr)
	{
		model->SetColor(r, g, b, a);
	}
}

