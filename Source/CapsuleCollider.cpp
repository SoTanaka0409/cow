#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"

CapsuleCollider::CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius)
	: Collider(parent)
{
	position_ = pos1;
	position2_ = pos2;
	radius_ = radius;
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::Draw()
{
}

void CapsuleCollider::OnEnter()
{
}

void CapsuleCollider::OnTrigger()
{
}

void CapsuleCollider::OnExit()
{
}