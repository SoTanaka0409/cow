import os

def w(path, text):
    with open(path, "w", encoding="utf-8-sig") as f:
        f.write(text)

files = {}

files["Source/Bait.cpp"] = """#include "Bait.h"
#include "CapsuleCollider.h"

/*
 * 餌オブジェクトの初期化
 * [入力] initPos: 初期座標
 * [出力] なし
 * [副作用] コライダーの初期化と有効期限のタイマー設定
 */
Bait::Bait(VECTOR initPos)
	: Object3D(initPos)
{
	model_ = new Model("data/model/character/Bait/kusa.mv1");
	capsule_collider_ = new CapsuleCollider(VGet(0, 0, 0), VGet(0, 0, 0), 10.0f);
	SetTag(Object3D::kTag3dBait);
	model_->SetScale(50.0f);

	capsule_collider_->parent_object_ = this;
	mActiveTimer = 300;
}

Bait::~Bait()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
	if (capsule_collider_ != nullptr)
	{
		delete capsule_collider_;
	}
}

/*
 * 餌の毎フレーム更新
 * [入力] なし
 * [出力] なし
 * [副作用] タイマーの減少と生存限界による削除フラグの有効化
 */
void Bait::Update()
{
	mActiveTimer--;
	if (mActiveTimer <= 0)
	{
		mDeleteFlag = true;
	}
	ColliderMove();
}

void Bait::Draw()
{
	model_->Draw();
}

/*
 * コライダーの座標追従
 * [入力] なし
 * [出力] なし
 * [副作用] モデル座標に合わせてコライダー位置を更新
 */
void Bait::ColliderMove()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		capsule_collider_->radius_ = 500.0f;
	}
}
"""

files["Source/Bait.h"] = """#pragma once
#include "Object3D.h"
#include "Model.h"

class CapsuleCollider;

class Bait : public Object3D
{
public:
	Bait(VECTOR initPos);
	virtual ~Bait();

	virtual void Update() override;
	virtual void Draw() override;
	void ColliderMove();

	bool GetDeleteFlag() const { return mDeleteFlag; }

private:
	Model* model_;
	int mActiveTimer;
	bool mDeleteFlag = false;
};
"""

files["Source/Camera.cpp"] = """#include "Camera.h"
#include <cmath>

/*
 * カメラの初期化
 * [入力] なし
 * [出力] なし
 * [副作用] 注視点、位置、回転角の初期設定
 */
Camera::Camera()
	: target_(VGet(0.0f, 0.0f, 0.0f)),
	position_(VGet(0.0f, 50.0f, -100.0f)),
	distance_(2000.0f),
	rotationX_(30.0f),
	rotationY_(0.0f),
	look_target_(nullptr)
{
	SetCameraNearFar(0.1f, 15000.0f);
	SetCameraPositionAndTarget_UpVecY(position_, target_);
}

Camera::~Camera()
{
}

/*
 * カメラの毎フレーム更新
 * [入力] なし
 * [出力] なし
 * [副作用] ターゲット追従による位置計算とDxLibカメラの更新
 */
void Camera::Update()
{
	if (look_target_ != nullptr)
	{
		target_ = look_target_->position_;
		target_.y += target_offset_y_;
	}

	float radX = rotationX_ * DX_PI_F / 180.0f;
	float radY = rotationY_ * DX_PI_F / 180.0f;

	float horizontalDistance = distance_ * cosf(radX);
	float verticalDistance = distance_ * sinf(radX);

	position_.x = target_.x - horizontalDistance * sinf(radY);
	position_.y = target_.y + verticalDistance;
	position_.z = target_.z - horizontalDistance * cosf(radY);

	SetCameraPositionAndTarget_UpVecY(position_, target_);
}

/*
 * カメラ回転の加算
 * [入力] dx: Y軸回転の変化量, dy: X軸回転の変化量
 * [出力] なし
 * [副作用] 回転角を更新し、ジンバルロック防止のためX軸の角度を制限する
 */
void Camera::Rotate(float dx, float dy)
{
	rotationY_ += dx;
	rotationX_ += dy;

	if (rotationX_ > 80.0f) rotationX_ = 80.0f;
	if (rotationX_ < 20.0f) rotationX_ = 20.0f;
}
"""

files["Source/Camera.h"] = """#pragma once

#pragma warning(push)
#pragma warning(disable : 4819)
#include "DxLib.h"
#pragma warning(pop)

#include "Object3D.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update();
	void Rotate(float dx, float dy);
	void SetTarget(Object3D* target) { look_target_ = target; }

private:
	VECTOR target_;
	VECTOR position_;
	float distance_;
	float rotationX_;
	float rotationY_;

	Object3D* look_target_;
	float target_offset_y_ = 50.0f;
};
"""

files["Source/CapsuleCollider.cpp"] = """#include "CapsuleCollider.h"

/*
 * カプセルコライダーの初期化
 * [入力] pos: 底面座標, pos2: 上面座標, r: 半径
 * [出力] なし
 * [副作用] 当たり判定用の各パラメータを設定
 */
CapsuleCollider::CapsuleCollider(VECTOR pos, VECTOR pos2, float r)
	: position_(pos), position2_(pos2), radius_(r)
{
}

CapsuleCollider::~CapsuleCollider()
{
}

/*
 * カプセル同士の衝突判定
 * [入力] other: 対象のカプセルコライダー
 * [出力] 衝突していれば true
 * [副作用] なし
 */
bool CapsuleCollider::CheckCollision(const CapsuleCollider* other) const
{
	return HitCheck_Capsule_Capsule(position_, position2_, radius_, other->position_, other->position2_, other->radius_);
}

/*
 * カプセルと球の衝突判定
 * [入力] other: 対象の球コライダー
 * [出力] 衝突していれば true
 * [副作用] なし
 */
bool CapsuleCollider::CheckCollisionSphere(const SphereCollider* other) const
{
	return HitCheck_Capsule_Sphere(position_, position2_, radius_, other->position_, other->radius_);
}
"""

files["Source/CapsuleCollider.h"] = """#pragma once
#include "Collider.h"
#include "SphereCollider.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(VECTOR pos, VECTOR pos2, float r);
	virtual ~CapsuleCollider();

	bool CheckCollision(const CapsuleCollider* other) const;
	bool CheckCollisionSphere(const SphereCollider* other) const;

	VECTOR position_;
	VECTOR position2_;
	float radius_;
};
"""

files["Source/CharacterMove.cpp"] = """#include "CharacterMove.h"
#include "CharacterState.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Math.h"
#include "ServiceLocator.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Wall.h"
#include "GameConstants.h"

/*
 * キャラクター基底クラスの初期化
 * [入力] filename: モデルパス, initPos: 初期座標
 * [出力] なし
 * [副作用] モデル生成、コライダー追加、状態マシンの初期化
 */
CharacterMove::CharacterMove(std::string filename, VECTOR initPos)
	: Object3D(initPos), mpCurrentState(nullptr), mActionTimer(0),
	mfSpeed(0.0f), mVacuumTimer(0), mDeleteFlag(false),
	mCurrentState(STATE_WALK), target_angle_(0.0f), angle_(0.0f),
	mfScore(0.0f), mfXp(0.0f), mbBaitFlag(false), mbIsVisible(true)
{
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	oldmoveVec = VGet(0.0f, 0.0f, 0.0f);

	model_ = new Model(filename);
	capsule_collider_ = new CapsuleCollider(initPos, VAdd(initPos, VGet(0.0f, 50.0f, 0.0f)), 50.0f);
	capsule_collider_->parent_object_ = this;

	ChangeState(new StateIdle());
}

CharacterMove::~CharacterMove()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
	if (capsule_collider_ != nullptr)
	{
		delete capsule_collider_;
	}

	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Exit(this);
		delete mpCurrentState;
	}
}

/*
 * キャラクターの状態リセット
 * [入力] pos: 新しい座標
 * [出力] なし
 * [副作用] 各種タイマーの初期化、状態の待機への変更
 */
void CharacterMove::Reset(VECTOR pos)
{
	position_ = pos;
	rotation_ = VGet(0.0f, 0.0f, 0.0f);
	moveVec = VGet(0.0f, 0.0f, 0.0f);
	mVacuumTimer = 0;
	mActionTimer = 60;
	mDeleteFlag = false;
	mbBaitFlag = false;
	mCurrentState = STATE_IDLE;

	ChangeState(new StateIdle());
}

/*
 * 管理リストから自身を除外する処理
 * [入力] なし
 * [出力] なし
 * [副作用] ObjectManagerのリストから自身を削除
 */
void CharacterMove::Deactivate()
{
	if (auto scene = Master::mpSceneManager->GetCurrentScene())
	{
		if (auto objMgr = scene->GetObjectManager())
		{
			objMgr->RemoveObjectNoDelete(this);
		}
	}
}

/*
 * 毎フレーム更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] 移動、死亡判定、回転、コライダー更新
 */
void CharacterMove::Update()
{
	MoveCharacter();
	CharacterDied();

	if (mCurrentState != STATE_VACUUM)
	{
		RotationCharacter();
	}

	ColliderMove();
	model_->Update();
}

void CharacterMove::Draw()
{
	model_->Draw();
}

/*
 * 移動処理
 * [入力] なし
 * [出力] なし
 * [副作用] AI更新と壁判定を経て座標を確定する
 */
void CharacterMove::MoveCharacter()
{
	if (mCurrentState == STATE_VACUUM) return;

	old_position_ = position_;

	UpdateWanderAI();
	CheckWallCollision();

	model_->SetPosition(position_);
}

/*
 * AIの更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] 現在のStateのUpdateを呼び出す
 */
void CharacterMove::UpdateWanderAI()
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Update(this);
	}
}

/*
 * ステートマシンの状態変更
 * [入力] newState: 新しい状態クラスのポインタ
 * [出力] なし
 * [副作用] 古いStateを破棄し、新しいStateに移行する
 */
void CharacterMove::ChangeState(CharacterState* newState)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Exit(this);
		delete mpCurrentState;
	}
	mpCurrentState = newState;
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Enter(this);
	}
}

/*
 * 吸引状態への変更
 * [入力] なし
 * [出力] なし
 * [副作用] 状態を吸引中(STATE_VACUUM)に変更する
 */
void CharacterMove::ChangeStateToVacuum()
{
	mCurrentState = STATE_VACUUM;
	ChangeState(new StateVacuum());
}

/*
 * 壁との衝突判定と押し出し
 * [入力] なし
 * [出力] なし
 * [副作用] 壁オブジェクトとの判定を行い、座標を補正する
 */
void CharacterMove::CheckWallCollision()
{
	bool hitwall = false;
	bool hitwalls = false;

	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dWall);
	if (!walls.empty())
	{
		for (auto& w : walls)
		{
			Wall* wall = dynamic_cast<Wall*>(w);
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				if (HitCheck_Capsule_Triangle(
					position_,
					VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						position_,
						VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);

					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					if (hitwall && !hitwalls)
					{
						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 複数壁への埋まりを防止
					else if (hitwalls)
					{
						position_ = old_position_;
					}
				}
			}
		}
	}
}

/*
 * コライダーの位置追従
 * [入力] なし
 * [出力] なし
 * [副作用] コライダーの位置をモデル座標に同期させる
 */
void CharacterMove::ColliderMove()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
		capsule_collider_->radius_ = 50.0f;
	}
}

/*
 * 移動方向への回転
 * [入力] なし
 * [出力] なし
 * [副作用] 移動ベクトル方向へモデルを回転させる
 */
void CharacterMove::RotationCharacter()
{
	if (moveVec.x != 0.0f || moveVec.z != 0.0f)
	{
		float targetAngle = atan2f(moveVec.x, moveVec.z);
		rotation_.y = targetAngle + DX_PI_F;
		model_->SetRotation(rotation_);
	}
}

/*
 * キャラクターの旋回
 * [入力] なし
 * [出力] なし
 * [副作用] モデルを一定速度で旋回させ続ける
 */
void CharacterMove::CharacterRotate()
{
	rotation_.y += 0.1f;

	if (rotation_.y > DX_PI_F * 2.0f)
	{
		rotation_.y -= DX_PI_F * 2.0f;
	}
	model_->SetRotation(rotation_);
}

void CharacterMove::SetScale(float scale)
{
	if (model_ != nullptr)
	{
		model_->SetScale(scale);
	}
}

void CharacterMove::CharacterDied()
{
}

/*
 * 死亡処理
 * [入力] reason: 死亡の理由
 * [出力] なし
 * [副作用] 死亡演出や削除フラグの更新を行う
 */
void CharacterMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	switch (reason)
	{
	}
}
"""

files["Source/CharacterMove.h"] = """#pragma once

#pragma warning(push)
#pragma warning(disable : 4819)
#include "DxLib.h"
#pragma warning(pop)

#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"

class SphereCollider;
class CapsuleCollider;
class CharacterState;
class Player3D;

enum AIState
{
	STATE_IDLE,
	STATE_WALK,
	STATE_VACUUM
};

enum DeathReason
{
	DEATH_VACUUM,
	DEATH_BAIT,
	DEATH_LIMIT
};

class CharacterMove : public Object3D
{
public:
	void SetTargetPlayer(Player3D* player) { mpTargetPlayer = player; }
	Player3D* GetTargetPlayer() const { return mpTargetPlayer; }
	CharacterMove(std::string filename, VECTOR initPos);
	virtual ~CharacterMove();

	virtual void Update() override;
	virtual void Draw() override;

	virtual void MoveCharacter();
	virtual void UpdateWanderAI();
	virtual void CheckWallCollision();
	virtual void ColliderMove();
	virtual void RotationCharacter();
	virtual void CharacterRotate();
	virtual void CharacterDied();
	virtual void Die(DeathReason reason);

	virtual void Reset(VECTOR pos);
	virtual void Deactivate();

	void AddAnimation(AnimationState state, std::string filename) {}
	void SetScale(float scale);

	AIState GetCurrentState() const { return mCurrentState; }
	void SetCurrentState(AIState state) { mCurrentState = state; }

	void ChangeState(CharacterState* newState);
	void ChangeStateToVacuum();

	void SetActionTimer(int timer) { mActionTimer = timer; }
	void DecreaseActionTimer() { mActionTimer--; }
	int GetActionTimer() const { return mActionTimer; }

	void SetMoveVec(VECTOR vec) { moveVec = vec; }
	VECTOR GetMoveVec() const { return moveVec; }
	float GetSpeed() const { return mfSpeed; }

	void IncreaseVacuumTimer() { mVacuumTimer++; }
	void ResetVacuumTimer() { mVacuumTimer = 0; }
	int GetVacuumTimer() const { return mVacuumTimer; }

	bool GetBaitFlag() const { return mbBaitFlag; }
	bool GetCharacterDelete() const { return mDeleteFlag; }

protected:
	Player3D* mpTargetPlayer = nullptr;
	Model* model_;

	AIState mCurrentState;
	CharacterState* mpCurrentState;

	int mActionTimer;
	float mfSpeed;

	float target_angle_;
	float angle_;
	const float kRotateSpeed = 0.2f;

	VECTOR UpMoveVector;
	VECTOR moveVec;
	VECTOR oldmoveVec;
	VECTOR hitPos;

	int mVacuumTimer;
	bool mDeleteFlag;
	float death_timer_;

	float mfScore;
	float mfXp;
	bool mbBaitFlag;
	bool mbIsVisible;
};
"""

for path, text in files.items():
    w(path, text)
