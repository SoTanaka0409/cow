#include "Object_Stage.h"

Object_Stage::Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle)
	: Object3D(initPos)
{
	model = new Model(filename, initPos);
	model->SetScale(scale);
	model->SetRotation(angle);
}

Object_Stage::~Object_Stage()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void Object_Stage::Update()
{
	if (model != nullptr)
	{
		model->Update();
	}
}

void Object_Stage::Draw()
{
	if (model != nullptr)
	{
		model->Draw();
	}
}

void Object_Stage::SetScale(float scale)
{
	if (model != nullptr)
	{
		model->SetScale(scale);
	}
}

void Object_Stage::SetRotation(VECTOR angle)
{
	if (model != nullptr)
	{
		model->SetRotation(angle);
	}
}

void Object_Stage::SetColor(float r, float g, float b, float a)
{
	if (model != nullptr)
	{
		model->SetColor(r, g, b, a);
	}
}
