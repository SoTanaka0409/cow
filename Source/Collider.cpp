#include "Collider.h"
#include "Object3D.h"
#include "ColliderManager.h"
#include <cassert>

/*
 * コライダー生成時に必須となる親オブジェクトを設定するため
 * [入力] parent: このコライダーを所有する親オブジェクト
 * [出力] なし
 * [副作用] ColliderManagerに自身を登録
 */
Collider::Collider(Object3D* parent)
	: parent_object_(parent)
	, position_(VGet(0.0f, 0.0f, 0.0f))
	, position2_(VGet(0.0f, 0.0f, 0.0f))
	, radius_(0.0f)
	, delete_flag_(false)
{
	assert(parent);
	ColliderManager::GetInstance()->AddCollider(this);
}

/*
 * 破棄時に他オブジェクトへの影響をなくすため
 * [入力] なし
 * [出力] なし
 * [副作用] ColliderManagerから自身を解除
 */
Collider::~Collider()
{
	ColliderManager::GetInstance()->RemoveCollider(this);
}

/*
 * 衝突状態の変化を検知し、適切なイベントを通知するため
 * [入力] check: 判定対象のコライダー, isHit: 衝突判定の計算結果
 * [出力] なし
 * [副作用] collision_list_の更新、親オブジェクトへのイベント通知
 */
void Collider::HitCheck(Collider* check, bool isHit)
{
	if (isHit)
	{
		auto itr = std::find_if(
			collision_list_.begin(),
			collision_list_.end(),
			[&](Collider* col) { return col == check; }
		);

		if (itr != collision_list_.end())
		{
			if (this->parent_object_ != nullptr)
			{
				parent_object_->OnTrigger(this, check);
			}
		}
		else
		{
			collision_list_.push_back(check);
			if (this->parent_object_ != nullptr)
			{
				parent_object_->OnEnter(this, check);
			}
		}
	}
	else
	{
		auto itr = std::find_if(
			collision_list_.begin(),
			collision_list_.end(),
			[&](Collider* col) { return col == check; }
		);

		if (itr != collision_list_.end())
		{
			if (this->parent_object_ != nullptr)
			{
				this->parent_object_->OnExit(this, check);
			}
			collision_list_.erase(itr);
		}
	}
}

/*
 * デバッグ用描画のため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Collider::Draw()
{
}

/*
 * 衝突開始時の処理を定義するため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Collider::OnEnter()
{
}

/*
 * 衝突中の処理を定義するため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Collider::OnTrigger()
{
}

/*
 * 衝突終了時の処理を定義するため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Collider::OnExit()
{
}