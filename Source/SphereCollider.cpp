#include "SphereCollider.h"
#include"CapsuleCollider.h"

SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	mvPosition = center;
	mfRadius = radius;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Draw()
{
	// 球の描画処理
	DrawSphere3D(
		mvPosition,
		mfRadius,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

void SphereCollider::OnEnter()
{

}

void SphereCollider::OnTrigger()
{

}

void SphereCollider::OnExit()
{

}

