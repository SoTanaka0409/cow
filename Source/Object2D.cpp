#include "ServiceLocator.h"
#include "Object2D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Object2D::Object2D(VECTOR initPos)
	: position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag2D::kNone2d)
	, draw_flag_(true)
{
	
	ServiceLocator::GetObjectManager()->AddObject(this);
}

Object2D::~Object2D()
{
}

void Object2D::Draw()
{
}

void Object2D::Update()
{
}