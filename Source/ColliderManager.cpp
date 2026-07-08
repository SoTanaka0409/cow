#include "ColliderManager.h"
#include "Collider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "DxLib.h"
#include "Master.h"
#include <vector>
#include <algorithm>

ColliderManager* ColliderManager::instance_ = nullptr;

ColliderManager::ColliderManager()
{
	collider_list_.reserve(1000);
}

ColliderManager::~ColliderManager()
{
}

ColliderManager* ColliderManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ColliderManager();
	}
	return instance_;
}

void ColliderManager::Finalize()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void ColliderManager::Update()
{
	for (size_t i = 0; i < collider_list_.size(); ++i)
	{
		auto colA = collider_list_[i];
		if (colA == nullptr || colA->IsDeleteFlag()) continue;

		for (size_t j = i + 1; j < collider_list_.size(); ++j)
		{
			auto colB = collider_list_[j];
			if (colB == nullptr || colB->IsDeleteFlag()) continue;

			bool isHit = CheckCollision(colA, colB);
			
			// A縺ｨB縺ｮ蜿梧婿縺ｫ蛻､螳夂ｵ先棡繧帝夂衍縺吶ｋ
			colA->HitCheck(colB, isHit);
			colB->HitCheck(colA, isHit);
		}
	}

	DeleteAllColliderIfNeeded(); // 繝輔Ξ繝ｼ繝邨ゆｺ・凾縺ｫ荳崎ｦ√↑繧ｳ繝ｩ繧､繝繝ｼ繧偵け繝ｪ繝ｼ繝ｳ繧｢繝・・
}

void ColliderManager::Draw()
{
}

void ColliderManager::AddCollider(Collider* Collider)
{
	collider_list_.push_back(Collider);
}

/*
 * @brief 縺吶∋縺ｦ縺ｮ逋ｻ骭ｲ貂医∩繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蜑企勁繝輔Λ繧ｰ繧堤ｫ九※縲∫ｮ｡逅・Μ繧ｹ繝医ｒ遨ｺ縺ｫ縺吶ｋ
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 蜈ｨ繧ｳ繝ｩ繧､繝繝ｼ縺ｸ縺ｮ蜑企勁繝輔Λ繧ｰ騾夂衍縲√Μ繧ｹ繝医・繧ｯ繝ｪ繧｢
 */
void ColliderManager::DeleteAllCollider()
{
	for (size_t i = 0; i < collider_list_.size(); ++i)
	{
		if (collider_list_[i] != nullptr)
		{
			collider_list_[i]->SetDeleteFlag(true);
		}
	}
	collider_list_.clear();
}

/*
 * @brief 蜑企勁繝輔Λ繧ｰ(delete_flag_)縺檎悄縺ｫ險ｭ螳壹＆繧後※縺・ｋ繧ｳ繝ｩ繧､繝繝ｼ繧偵Μ繧ｹ繝医°繧牙ｮ牙・縺ｫ髯､螟悶☆繧・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 隧ｲ蠖薙さ繝ｩ繧､繝繝ｼ縺ｮ繝ｪ繧ｹ繝磯勁螟悶√う繝・Ξ繝ｼ繧ｿ縺ｮ螳牙・縺ｪ騾ｲ陦・
 */
void ColliderManager::DeleteAllColliderIfNeeded()
{
	auto newEnd = std::remove_if(collider_list_.begin(), collider_list_.end(), [](Collider* col) {
		return col->IsDeleteFlag();
	});

	if (newEnd != collider_list_.end())
	{
		collider_list_.erase(newEnd, collider_list_.end());
	}
}

void ColliderManager::RemoveCollider(Collider* collider)
{
	auto itr = std::find(collider_list_.begin(), collider_list_.end(), collider);
	if (itr != collider_list_.end())
	{
		collider_list_.erase(itr);
	}
}

bool ColliderManager::CheckCollision(Collider* colA, Collider* colB)
{
	CapsuleCollider* capA = dynamic_cast<CapsuleCollider*>(colA);
	SphereCollider* sphA = dynamic_cast<SphereCollider*>(colA);
	CapsuleCollider* capB = dynamic_cast<CapsuleCollider*>(colB);
	SphereCollider* sphB = dynamic_cast<SphereCollider*>(colB);

	if (capA && capB)
	{
		return HitCheck_Capsule_Capsule(capA->position_, capA->position2_, capA->radius_,
										capB->position_, capB->position2_, capB->radius_);
	}
	else if (sphA && sphB)
	{
		return HitCheck_Sphere_Sphere(sphA->position_, sphA->radius_,
									  sphB->position_, sphB->radius_);
	}
	else if (capA && sphB)
	{
		return HitCheck_Sphere_Capsule(sphB->position_, sphB->radius_,
									   capA->position_, capA->position2_, capA->radius_);
	}
	else if (sphA && capB)
	{
		return HitCheck_Sphere_Capsule(sphA->position_, sphA->radius_,
									   capB->position_, capB->position2_, capB->radius_);
	}

	return false;
}