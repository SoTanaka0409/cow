#include "SphereCollider.h"
#include"CapsuleCollider.h"

SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	position_ = center;
	radius_ = radius;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Draw()
{
	// 逅・・謠冗判蜃ｦ逅・
	DrawSphere3D(
		position_,
		radius_,
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