#include "ServiceLocator.h"
#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "CapsuleCollider.h"

Object3D::Object3D(VECTOR initPos)
	: position_(initPos)
	, capsule_collider_(nullptr)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag3D::kNone3d)
	, draw_flag_(true)
	, radius_(0)
{
	// ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg・ｽ・ｽﾇ暦ｿｽ・ｽE・ｽX・ｽV・ｽ・ｽ・ｽ驍ｽ・ｽﾟ、・ｽ・ｽ・ｽﾝのア・ｽN・ｽe・ｽB・ｽu・ｽﾈシ・ｽ[・ｽ・ｽ・ｽﾌマ・ｽl・ｽ[・ｽW・ｽ・ｽ・ｽ[・ｽﾖ趣ｿｽ・ｽ・ｽ・ｽo・ｽ^・ｽ・ｽ・ｽ・ｽ
	ServiceLocator::GetObjectManager()->AddObject(this);
	capsule_collider_ = new CapsuleCollider(this, initPos, initPos, 0);
}

Object3D::~Object3D()
{
	if (capsule_collider_ != nullptr)
	{
		delete capsule_collider_;
		capsule_collider_ = nullptr;
	}
}

void Object3D::Draw()
{
}

void Object3D::Update()
{
}

void Object3D::OnEnter(Collider* collider, Collider* check)
{
}

void Object3D::OnTrigger(Collider* collider, Collider* check)
{
}

void Object3D::OnExit(Collider* collider, Collider* check)
{
}