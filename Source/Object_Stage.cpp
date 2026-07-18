#include "Object_Stage.h"

Object_Stage::Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle)
	: Object3D(initPos)
{
	model_ = new Model(filename, initPos);
	model_->SetScale(scale);
	model_->SetRotation(angle);
}

Object_Stage::~Object_Stage()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
}

void Object_Stage::Update()
{
	if (model_ != nullptr)
	{
		model_->Update();
	}
}

void Object_Stage::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

void Object_Stage::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

void Object_Stage::SetRotation(VECTOR angle)
{
	if (model_ != nullptr)
	{
		model_->SetRotation(angle);
	}
}

void Object_Stage::SetColor(float r, float g, float b, float a)
{
	if (model_ != nullptr)
	{
		model_->SetColor(r, g, b, a);
	}
}
