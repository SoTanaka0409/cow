#include "ServiceLocator.h"
#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "CapsuleCollider.h"

Object3D::Object3D(VECTOR initPos)
	: mvPosition(initPos)
	, mpCapsuleCollider(nullptr)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mbDeleteFlag(false)
	, mnTag(Tag3D::None3D)
	, mbDrawFlag(true)
	, mfRadius(0)
{
	// 生成したオブジェクトを管理・更新するため、現在のアクティブなシーンのマネージャーへ自動登録する
	ServiceLocator::GetObjectManager()->AddObject(this);
	mpCapsuleCollider = new CapsuleCollider(this, initPos, initPos, 0);
}

Object3D::~Object3D()
{
	if (mpCapsuleCollider != nullptr)
	{
		delete mpCapsuleCollider;
		mpCapsuleCollider = nullptr;
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

