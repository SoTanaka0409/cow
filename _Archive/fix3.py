import os

def w(path, text):
    with open(path, "w", encoding="utf-8-sig") as f:
        f.write(text)

files = {}

files["Source/CharacterState.cpp"] = """#include "CharacterState.h"
#include "CharacterMove.h"
#include "GameConstants.h"

/*
 * 待機状態への遷移
 * [入力] pCharacter: 対象キャラクター
 * [出力] なし
 * [副作用] 待機タイマーの設定と移動ベクトルの初期化
 */
void StateIdle::Enter(CharacterMove* pCharacter)
{
	pCharacter->SetActionTimer(60);
	pCharacter->SetMoveVec(VGet(0.0f, 0.0f, 0.0f));
}

/*
 * 待機状態の毎フレーム更新
 * [入力] pCharacter: 対象キャラクター
 * [出力] なし
 * [副作用] タイマー減少により歩行状態へ移行
 */
void StateIdle::Update(CharacterMove* pCharacter)
{
	pCharacter->DecreaseActionTimer();
	if (pCharacter->GetActionTimer() <= 0)
	{
		pCharacter->SetCurrentState(STATE_WALK);
		pCharacter->ChangeState(new StateWalk());
	}
}

void StateIdle::Exit(CharacterMove* pCharacter)
{
}

/*
 * 歩行状態への遷移
 * [入力] pCharacter: 対象キャラクター
 * [出力] なし
 * [副作用] 移動方向のランダム設定と歩行タイマーの設定
 */
void StateWalk::Enter(CharacterMove* pCharacter)
{
	pCharacter->SetActionTimer(180);
	VECTOR dir = VGet(GetRand(200) - 100.0f, 0.0f, GetRand(200) - 100.0f);
	if (VSquareSize(dir) > 0.0f)
	{
		dir = VNorm(dir);
	}
	pCharacter->SetMoveVec(dir);
}

/*
 * 歩行状態の毎フレーム更新
 * [入力] pCharacter: 対象キャラクター
 * [出力] なし
 * [副作用] 座標の更新およびタイマー減少による待機状態への移行
 */
void StateWalk::Update(CharacterMove* pCharacter)
{
	VECTOR pos = pCharacter->GetPosition();
	pos = VAdd(pos, VScale(pCharacter->GetMoveVec(), pCharacter->GetSpeed()));
	pCharacter->SetPosition(pos);

	pCharacter->DecreaseActionTimer();
	if (pCharacter->GetActionTimer() <= 0)
	{
		pCharacter->SetCurrentState(STATE_IDLE);
		pCharacter->ChangeState(new StateIdle());
	}
}

void StateWalk::Exit(CharacterMove* pCharacter)
{
}

/*
 * 吸い込まれ状態への遷移
 * [入力] pCharacter: 対象キャラクター
 * [出力] なし
 * [副作用] キャラクターのリストからの除外
 */
void StateVacuum::Enter(CharacterMove* pCharacter)
{
	pCharacter->Deactivate();
}

/*
 * 吸い込まれ状態の毎フレーム更新
 * [入力] pCharacter: 対象キャラクター
 * [出力] なし
 * [副作用] プレイヤー方向への移動と死亡判定
 */
void StateVacuum::Update(CharacterMove* pCharacter)
{
	if (pCharacter->GetTargetPlayer() != nullptr)
	{
		VECTOR pPos = pCharacter->GetTargetPlayer()->GetPosition();
		VECTOR ePos = pCharacter->GetPosition();
		VECTOR dir = VSub(pPos, ePos);
		float len = VSize(dir);

		if (len > 0.1f)
		{
			dir = VNorm(dir);
			ePos = VAdd(ePos, VScale(dir, GameConstants::kVacuumSpeed));
			pCharacter->SetPosition(ePos);
		}

		pCharacter->IncreaseVacuumTimer();
		if (pCharacter->GetVacuumTimer() >= GameConstants::kVacuumTimeLimit)
		{
			pCharacter->Die(DEATH_VACUUM);
		}
	}
}

void StateVacuum::Exit(CharacterMove* pCharacter)
{
}
"""

files["Source/CharacterState.h"] = """#pragma once

#pragma warning(push)
#pragma warning(disable : 4819)
#include "DxLib.h"
#pragma warning(pop)

class CharacterMove;

class CharacterState
{
public:
	virtual ~CharacterState() {}
	virtual void Enter(CharacterMove* pCharacter) = 0;
	virtual void Update(CharacterMove* pCharacter) = 0;
	virtual void Exit(CharacterMove* pCharacter) = 0;
};

class StateIdle : public CharacterState
{
public:
	void Enter(CharacterMove* pCharacter) override;
	void Update(CharacterMove* pCharacter) override;
	void Exit(CharacterMove* pCharacter) override;
};

class StateWalk : public CharacterState
{
public:
	void Enter(CharacterMove* pCharacter) override;
	void Update(CharacterMove* pCharacter) override;
	void Exit(CharacterMove* pCharacter) override;
};

class StateVacuum : public CharacterState
{
public:
	void Enter(CharacterMove* pCharacter) override;
	void Update(CharacterMove* pCharacter) override;
	void Exit(CharacterMove* pCharacter) override;
};
"""

files["Source/Collider.cpp"] = """#include "Collider.h"

/*
 * コライダーの初期化
 * [入力] なし
 * [出力] なし
 * [副作用] 各フラグと親オブジェクトポインタの初期設定
 */
Collider::Collider()
	: is_active_(true), is_trigger_(false), delete_flag_(false),
	parent_object_(nullptr), is_hit_(false)
{
}

Collider::~Collider()
{
}
"""

files["Source/Collider.h"] = """#pragma once

class Object3D;

class Collider
{
public:
	Collider();
	virtual ~Collider();

	bool is_active_;
	bool is_trigger_;
	bool is_hit_;

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() const { return delete_flag_; }

	Object3D* parent_object_;

private:
	bool delete_flag_;
};
"""

files["Source/ColliderManager.cpp"] = """#include "ColliderManager.h"
#include "Collider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"
#include "DxLib.h"
#include <algorithm>

ColliderManager* ColliderManager::instance_ = nullptr;

/*
 * インスタンスの取得
 * [入力] なし
 * [出力] ColliderManagerのインスタンス
 * [副作用] 未生成の場合は新規に生成する
 */
ColliderManager* ColliderManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ColliderManager();
	}
	return instance_;
}

/*
 * インスタンスの破棄
 * [入力] なし
 * [出力] なし
 * [副作用] インスタンスのメモリを解放する
 */
void ColliderManager::Finalize()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

ColliderManager::ColliderManager()
{
}

ColliderManager::~ColliderManager()
{
	DeleteAllCollider();
}

/*
 * 毎フレームの更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] コライダー同士の当たり判定と不要なコライダーの削除
 */
void ColliderManager::Update()
{
	for (size_t i = 0; i < collider_list_.size(); ++i)
	{
		if (collider_list_[i] == nullptr || !collider_list_[i]->is_active_) continue;
		collider_list_[i]->is_hit_ = false;
	}

	for (size_t i = 0; i < collider_list_.size(); ++i)
	{
		Collider* colA = collider_list_[i];
		if (colA == nullptr || !colA->is_active_) continue;

		for (size_t j = i + 1; j < collider_list_.size(); ++j)
		{
			Collider* colB = collider_list_[j];
			if (colB == nullptr || !colB->is_active_) continue;

			bool isHit = CheckCollision(colA, colB);

			if (colA->parent_object_ != nullptr)
				colA->parent_object_->HitCheck(colB, isHit);

			if (colB->parent_object_ != nullptr)
				colB->parent_object_->HitCheck(colA, isHit);
		}
	}

	DeleteAllColliderIfNeeded();
}

void ColliderManager::Draw()
{
}

void ColliderManager::AddCollider(Collider* Collider)
{
	collider_list_.push_back(Collider);
}

/*
 * 全コライダーの削除
 * [入力] なし
 * [出力] なし
 * [副作用] すべてのコライダーに削除フラグを立てリストをクリア
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
 * 削除フラグが立ったコライダーの除去
 * [入力] なし
 * [出力] なし
 * [副作用] 管理リストから削除対象のコライダーを除外
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
"""

files["Source/ColliderManager.h"] = """#pragma once
#include <vector>

class Collider;

class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	void Update();
	void Draw();

	static ColliderManager* GetInstance();
	static void Finalize();

	void AddCollider(Collider* Collider);
	void DeleteAllCollider();
	void DeleteAllColliderIfNeeded();
	void RemoveCollider(Collider* collider);
	
	std::vector<Collider*>& GetColliderList() { return collider_list_; }

private:
	bool CheckCollision(Collider* colA, Collider* colB);

	std::vector<Collider*> collider_list_;
	static ColliderManager* instance_;
};
"""

files["Source/Combo.cpp"] = """#include "Combo.h"
#include "DxLib.h"
#include "InputManager.h"
#include "Master.h"
#include "Utility.h"
#include <string>

/*
 * コンボ管理の初期化
 * [入力] なし
 * [出力] なし
 * [副作用] 画像の読み込みとコンボタイマーの設定
 */
Combo::Combo()
{
	combo_count_ = 0;
	combo_timer_ = 0.0f;
	combo_max_time_ = 3.0f; // 難易度調整のためコンボ継続時間を3秒に固定
	combo_image_ = Master::mpResourceManager->LoadGraphics("Resource/2D/COMBO.png");
	combo_show_ = false;
	combo_show_timer_ = 0.0f;
}

Combo::~Combo()
{
}

/*
 * コンボUIの描画
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void Combo::Draw()
{
	// 0コンボ時は表示を省略しUIの描画負荷を抑える
	if (combo_count_ >= 1)
	{
		int x = Utility::kUiBaseX;
		int y = Utility::kUiComboY;
		int width = Utility::kUiPanelW;
		int height = Utility::kUiPanelH;

		DrawExtendGraph(x, y, x + width, y + height, combo_image_, TRUE);

		if (Master::mpScore)
		{
			Master::mpScore->DrawNumber(
				Utility::kUiDigitX,
				y,
				combo_count_,
				1.25f,
				1
			);
		}
	}
}

/*
 * 毎フレーム更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] タイマー減少とコンボのリセット
 */
void Combo::Update()
{
	if (combo_count_ > 0)
	{
		combo_timer_ -= 0.01f;

		if (combo_timer_ <= 0.0f)
		{
			Reset();
		}
	}
}

/*
 * ヒット加算
 * [入力] なし
 * [出力] なし
 * [副作用] コンボカウント増加とタイマーのリセット
 */
void Combo::AddHit()
{
	combo_count_++;
	combo_timer_ = combo_max_time_;

	if (combo_count_ > 1000)
	{
		combo_count_ = 1000; // 描画桁数のオーバーフローを防ぐ
	}
}

/*
 * コンボリセット
 * [入力] なし
 * [出力] なし
 * [副作用] カウントとタイマーの初期化
 */
void Combo::Reset()
{
	combo_count_ = 0;
	combo_timer_ = 3.0f;
}

int Combo::GetCombo() const
{
	return combo_count_;
}

float Combo::GetMultiplier() const
{
	if (combo_count_ >= 1)
	{
		return (float)combo_count_;
	}
	return 1.0f;
}
"""

for path, text in files.items():
    w(path, text)
