#include "Collider.h"
#include "Object3D.h"
#include "ColliderManager.h"
#include <cassert>

Collider::Collider(Object3D* parent)
	: parentObject(parent)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvPosition2(VGet(0.0f, 0.0f, 0.0f))
	, radius(0.0f)
	, deleteFlag(false)
{
	assert(parent);
	ColliderManager::GetInstance()->AddCollider(this); // 生成時に自動でマネージャーへ登録
}

Collider::~Collider()
{
	ColliderManager::GetInstance()->RemoveCollider(this); // 破棄時に自動でマネージャーから登録解除
}

/*
 * @brief 別のコライダーとの幾何学的な交差状態を元に、適切なコリジョンイベントを通知する
 * [入力] check: 判定対象の相手コライダー, isHit: 当たり判定の交差計算結果
 * [出力] なし
 * [副作用] mCollisionListの挿入・削除、OnEnter / OnTrigger / OnExit の親アクターへのコールバック通知
 */
void Collider::HitCheck(Collider* check, bool isHit)
{
	if (isHit)
	{
		// 既に前フレームで同じ相手と衝突していたかを検索
		auto itr = std::find_if(
			collisionList.begin(),
			collisionList.end(),
			[&](Collider* col) { return col == check; }
		);

		if (itr != collisionList.end())
		{
			// 前フレームから衝突が継続しているため、OnTrigger（滞在イベント）を通知
			if (this->parentObject != nullptr)
			{
				parentObject->OnTrigger(this, check);
			}
		}
		else
		{
			// 新規の衝突が発生したため、リストに登録して OnEnter（開始イベント）を通知
			collisionList.push_back(check);
			if (this->parentObject != nullptr)
			{
				parentObject->OnEnter(this, check);
			}
		}
	}
	else
	{
		// 衝突していない場合、前フレームまで衝突していたかの状態をチェックする
		auto itr = std::find_if(
			collisionList.begin(),
			collisionList.end(),
			[&](Collider* col) { return col == check; }
		);

		if (itr != collisionList.end())
		{
			// 衝突が切れた（離脱した）瞬間のため、OnExit（終了イベント）を通知しリストから除外する
			if (this->parentObject != nullptr)
			{
				this->parentObject->OnExit(this, check);
			}
			collisionList.erase(itr);
		}
	}
}

void Collider::Update(Collider* check)
{
}

void Collider::Draw()
{
}

void Collider::OnEnter()
{
}

void Collider::OnTrigger()
{
}

void Collider::OnExit()
{
}

