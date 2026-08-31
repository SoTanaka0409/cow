#include "ColliderManager.h"
#include "Collider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "DxLib.h"
#include "Master.h"
#include <vector>
#include <algorithm>

ColliderManager* ColliderManager::instance_ = nullptr;

/// @brief コライダーリストのメモリ事前確保
ColliderManager::ColliderManager()
{
	// 大量発生時などのオブジェクト一斉生成において、動的メモリ確保によるスパイク(処理落ち)を防ぐため領域を事前確保する
	collider_list_.reserve(1000);
}

ColliderManager::~ColliderManager()
{
}

/// @return ColliderManagerのシングルトンインスタンス
/// @brief 初回呼び出し時にインスタンスを生成する
ColliderManager* ColliderManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ColliderManager();
	}
	return instance_;
}

/// @brief インスタンスの破棄とポインタの初期化
void ColliderManager::Finalize()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

/// @brief 全コライダー間の総当たり判定と、不要コライダーの一括削除
void ColliderManager::Update()
{
	// パフォーマンス注記: 現状O(N^2)の総当たり。将来的にオブジェクト数が増大する場合は空間分割(Octree等)の導入を推奨
	for (size_t i = 0; i < collider_list_.size(); ++i)
	{
		auto colA = collider_list_[i];
		if (colA == nullptr || colA->IsDeleteFlag()) continue;

		for (size_t j = i + 1; j < collider_list_.size(); ++j)
		{
			auto colB = collider_list_[j];
			if (colB == nullptr || colB->IsDeleteFlag()) continue;

			bool isHit = CheckCollision(colA, colB);

			// 衝突時の反発やダメージ処理を各オブジェクト自身に委譲し、Managerへの依存度を下げるため双方に通知する
			colA->HitCheck(colB, isHit);
			colB->HitCheck(colA, isHit);
		}
	}

	// 判定ループ中にリスト要素を直接削除するとイテレータが無効化されクラッシュするため、フレーム末尾で一括削除する
	DeleteAllColliderIfNeeded();
}

/// @brief 入力: なし
void ColliderManager::Draw()
{
	if (!Master::is_debug_mode_) return;
	for (auto* col : collider_list_)
	{
		if (col != nullptr && !col->IsDeleteFlag())
		{
			col->Draw();
		}
	}
}

/// @brief データの追加処理を行う
/// @details Collider (追加対象のコライダー)
/// @details リストへのコライダーポインタの追加
void ColliderManager::AddCollider(Collider* Collider)
{
	collider_list_.push_back(Collider);
}

/// @brief 全コライダーへの削除フラグ設定と、リストの完全消去
void ColliderManager::DeleteAllCollider()
{
	// 即時deleteは他クラスが保持しているポインタをダングリング(不正な参照)にする危険があるため、フラグのみ立てる
	for (size_t i = 0; i < collider_list_.size(); ++i)
	{
		if (collider_list_[i] != nullptr)
		{
			collider_list_[i]->SetDeleteFlag(true);
		}
	}
	collider_list_.clear();
}

/// @brief 削除フラグが立ったコライダーのリストからの除外
void ColliderManager::DeleteAllColliderIfNeeded()
{
	// 配列中途の要素削除によるメモリの詰め直し(O(N))が多発するのを防ぐため、erase-removeイディオムで一括処理する
	auto newEnd = std::remove_if(collider_list_.begin(), collider_list_.end(), [](Collider* col) {
		return col->IsDeleteFlag();
		});

	if (newEnd != collider_list_.end())
	{
		collider_list_.erase(newEnd, collider_list_.end());
	}
}

/// @brief データの除外処理を行う
/// @details collider (除外対象のコライダー)
/// @details 指定されたコライダーのリストからの除外
void ColliderManager::RemoveCollider(Collider* collider)
{
	// 線形探索(O(N))が発生するため多用厳禁。原則としてDeleteAllColliderIfNeeded()による遅延一括削除を優先すること
	auto itr = std::find(collider_list_.begin(), collider_list_.end(), collider);
	if (itr != collider_list_.end())
	{
		collider_list_.erase(itr);
	}
}

/// @return 衝突していればtrue
/// @brief colA
/// @details colB (判定対象のコライダー2つ)
/// @details 内部でのダウンキャスト実行
bool ColliderManager::CheckCollision(Collider* colA, Collider* colB)
{
	// パフォーマンス注記: 毎フレーム数万回呼ばれる箇所でのdynamic_castは重いため、Enum等による型判定へのリファクタリングを推奨
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
