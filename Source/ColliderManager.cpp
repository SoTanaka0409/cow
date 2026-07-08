#include "ColliderManager.h"
#include "Collider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "DxLib.h"
#include "Master.h"
#include <vector>
#include <algorithm>

ColliderManager* ColliderManager::Instance = nullptr;

ColliderManager::ColliderManager()
{
	mColliderList.reserve(1000);
}

ColliderManager::~ColliderManager()
{
}

ColliderManager* ColliderManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new ColliderManager();
	}
	return Instance;
}

void ColliderManager::Finalize()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void ColliderManager::Update()
{
	for (size_t i = 0; i < mColliderList.size(); ++i)
	{
		auto colA = mColliderList[i];
		if (colA == nullptr || colA->IsDeleteFlag()) continue;

		for (size_t j = i + 1; j < mColliderList.size(); ++j)
		{
			auto colB = mColliderList[j];
			if (colB == nullptr || colB->IsDeleteFlag()) continue;

			bool isHit = CheckCollision(colA, colB);
			
			// AとBの双方に判定結果を通知する
			colA->HitCheck(colB, isHit);
			colB->HitCheck(colA, isHit);
		}
	}

	DeleteAllColliderIfNeeded(); // フレーム終了時に不要なコライダーをクリーンアップ
}

void ColliderManager::Draw()
{
}

void ColliderManager::AddCollider(Collider* Collider)
{
	mColliderList.push_back(Collider);
}

/*
 * @brief すべての登録済みコライダーの削除フラグを立て、管理リストを空にする
 * [入力] なし
 * [出力] なし
 * [副作用] 全コライダーへの削除フラグ通知、リストのクリア
 */
void ColliderManager::DeleteAllCollider()
{
	for (size_t i = 0; i < mColliderList.size(); ++i)
	{
		if (mColliderList[i] != nullptr)
		{
			mColliderList[i]->SetDeleteFlag(true);
		}
	}
	mColliderList.clear();
}

/*
 * @brief 削除フラグ(mbDeleteFlag)が真に設定されているコライダーをリストから安全に除外する
 * [入力] なし
 * [出力] なし
 * [副作用] 該当コライダーのリスト除外、イテレータの安全な進行
 */
void ColliderManager::DeleteAllColliderIfNeeded()
{
	auto newEnd = std::remove_if(mColliderList.begin(), mColliderList.end(), [](Collider* col) {
		return col->IsDeleteFlag();
	});

	if (newEnd != mColliderList.end())
	{
		mColliderList.erase(newEnd, mColliderList.end());
	}
}

void ColliderManager::RemoveCollider(Collider* collider)
{
	auto itr = std::find(mColliderList.begin(), mColliderList.end(), collider);
	if (itr != mColliderList.end())
	{
		mColliderList.erase(itr);
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
		return HitCheck_Capsule_Capsule(capA->mvPosition, capA->mvPosition2, capA->mfRadius,
										capB->mvPosition, capB->mvPosition2, capB->mfRadius);
	}
	else if (sphA && sphB)
	{
		return HitCheck_Sphere_Sphere(sphA->mvPosition, sphA->mfRadius,
									  sphB->mvPosition, sphB->mfRadius);
	}
	else if (capA && sphB)
	{
		return HitCheck_Sphere_Capsule(sphB->mvPosition, sphB->mfRadius,
									   capA->mvPosition, capA->mvPosition2, capA->mfRadius);
	}
	else if (sphA && capB)
	{
		return HitCheck_Sphere_Capsule(sphA->mvPosition, sphA->mfRadius,
									   capB->mvPosition, capB->mvPosition2, capB->mfRadius);
	}

	return false;
}

