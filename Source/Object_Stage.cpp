#include "Object_Stage.h"

Object_Stage::Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle)
	: Object3D(initPos)
{
	mpModel = new Model(filename, initPos);
	mpModel->SetScale(scale);
	mpModel->SetRotation(angle);
}

Object_Stage::~Object_Stage()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void Object_Stage::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}

void Object_Stage::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
}

void Object_Stage::SetScale(float scale)
{
	if (mpModel != nullptr)
	{
		mpModel->SetScale(scale);
	}
}

void Object_Stage::SetRotation(VECTOR angle)
{
	if (mpModel != nullptr)
	{
		mpModel->SetRotation(angle);
	}
}

void Object_Stage::SetColor(float r, float g, float b, float a)
{
	if (mpModel != nullptr)
	{
		mpModel->SetColor(r, g, b, a);
	}
}
