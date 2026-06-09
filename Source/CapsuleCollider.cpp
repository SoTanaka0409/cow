#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"

CapsuleCollider::CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius)
	: Collider(parent)
{
	mvPosition = pos1;
	mvPosition2 = pos2;
	mfRadius = radius;
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::Update(Collider* check)
{
	if (check != nullptr)
	{
		// 判定相手がカプセル型コライダーの場合の交差判定計算
		CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(check);
		
		if (capsule != nullptr)
		{
			bool isHit = HitCheck_Capsule_Capsule(
				this->mvPosition,
				this->mvPosition2,
				this->mfRadius,
				capsule->mvPosition,
				capsule->mvPosition2,
				capsule->mfRadius
			);

			HitCheck(check, isHit); // 交差結果を基底コライダーのイベント処理へ通知
		}
	}
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
