#include "ColliderManager.h"
#include "Collider.h"
#include "Master.h"
#include <vector>

ColliderManager* ColliderManager::Instance = nullptr;

ColliderManager::ColliderManager()
{
}

ColliderManager::~ColliderManager()
{
}

void ColliderManager::Update()
{
	// 登録されている全コライダー間で総当たり当たり判定計算を行い、衝突イベントを更新する
	for (auto itr = colliderList.begin(); itr != colliderList.end(); ++itr)
	{
		if ((*itr) == nullptr)
		{
			continue;
		}

		if ((*itr)->IsDeleteFlag())
		{
			continue;
		}

		for (auto itr_check = colliderList.begin(); itr_check != colliderList.end(); ++itr_check)
		{
			if (itr == itr_check)
			{
				continue;
			}

			if ((*itr_check) == nullptr)
			{
				continue;
			}

			if ((*itr_check)->IsDeleteFlag())
			{
				continue;
			}

			(*itr)->Update((*itr_check));
		}
	}

	DeleteAllColliderIfNeeded(); // フレーム終了時に不要なコライダーをクリーンアップ
}

void ColliderManager::Draw()
{
}

void ColliderManager::AddCollider(Collider* Collider)
{
	colliderList.push_back(Collider);
}

/*
 * @brief すべての登録済みコライダーの削除フラグを立て、管理リストを空にする
 * [入力] なし
 * [出力] なし
 * [副作用] 全コライダーへの削除フラグ通知、リストのクリア
 */
void ColliderManager::DeleteAllCollider()
{
	// イテレータ破損（DeleteAllColliderIfNeededの並行実行によるクラッシュ）を完全に防ぐため、
	// フラグ設定後に一括でクリアする安全なロジックを実装
	for (auto col : colliderList)
	{
		if (col != nullptr)
		{
			col->SetDeleteFlag(true);
		}
	}
	colliderList.clear();
}

/*
 * @brief 削除フラグ(deleteFlag)が真に設定されているコライダーをリストから安全に除外する
 * [入力] なし
 * [出力] なし
 * [副作用] 該当コライダーのリスト除外、イテレータの安全な進行
 */
void ColliderManager::DeleteAllColliderIfNeeded()
{
	for (auto itr = colliderList.begin(); itr != colliderList.end(); /* ループ内でイテレータを進める */)
	{
		if ((*itr)->IsDeleteFlag())
		{
			itr = colliderList.erase(itr); // 削除後の次の有効要素のイテレータを取得
		}
		else
		{
			itr++;
		}
	}
}

void ColliderManager::RemoveCollider(Collider* collider)
{
	colliderList.remove(collider);
}

