#include "ServiceLocator.h"
#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "CapsuleCollider.h"

Object3D::Object3D(VECTOR initPos)
	: mvPosition(initPos)
	, capsuleCollider(nullptr)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, deleteFlag(false)
	, tag(Tag3D::None3D)
	, drawFlag(true)
	, radius(0)
{
	// 生成したオブジェクトを管理・更新するため、現在のアクティブなシーンのマネージャーへ自動登録する
	ServiceLocator::GetObjectManager()->AddObject(this);
	capsuleCollider = new CapsuleCollider(this, initPos, initPos, 0);
}

Object3D::~Object3D()
{
	if (capsuleCollider != nullptr)
	{
		delete capsuleCollider;
		capsuleCollider = nullptr;
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

