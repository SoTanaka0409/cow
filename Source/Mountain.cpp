#include "Mountain.h"

Mountain::Mountain(std::string filename, VECTOR initPos, VECTOR scale, VECTOR angle)
	: Object3D(initPos)
{
	model_ = new Model(filename, initPos);
	model_->SetScale(scale);
	model_->SetRotation(angle);
}

Mountain::~Mountain()
{
	// 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ髦ｲ豁｢縺ｮ縺溘ａ縲∝虚逧・↓逕滓・縺励◆Model繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤ｴ譽・☆繧・
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
}

void Mountain::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

void Mountain::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

void Mountain::SetScale(VECTOR scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

void Mountain::SetRotation(VECTOR angle)
{
	if (model_ != nullptr)
	{
		model_->SetRotation(angle);
	}
}

void Mountain::SetColor(float r, float g, float b, float a)
{
	if (model_ != nullptr)
	{
		model_->SetColor(r, g, b, a);
	}
}
