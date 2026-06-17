#pragma once

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

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] AI更新と壁判定を経て座標を確定する
	 */
	virtual void MoveCharacter();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 現在のStateのUpdateを呼び出す
	 */
	virtual void UpdateWanderAI();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 壁オブジェクトとの判定を行い、mvPositionを補正する
	 */
	virtual void CheckWallCollision();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コライダーの位置をモデルに同期させる
	 */
	virtual void ColliderMove();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 移動ベクトル方向へモデルを回転させる
	 */
	virtual void RotationCharacter();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] モデルを一定速度で旋回させ続ける
	 */
	virtual void CharacterRotate();

	virtual void CharacterDied();

	/*
	 * [入力] reason: 死亡の理由(DeathReason)
	 * [出力] なし
	 * [副作用] 死亡演出やフラグ更新を行う
	 */
	virtual void Die(DeathReason reason);

	virtual void Reset(VECTOR pos);
	virtual void Deactivate();

	void AddAnimation(AnimationState state, std::string filename) {}
	void SetScale(float scale);

	AIState GetCurrentState() const { return mCurrentState; }
	void SetCurrentState(AIState state) { mCurrentState = state; }

	/*
	 * [入力] newState: 新しい状態クラスのポインタ
	 * [出力] なし
	 * [副作用] 古いStateを破棄し、新しいStateに移行する
	 */
	void ChangeState(CharacterState* newState);

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 状態を吸引中(STATE_VACUUM)に変更する
	 */
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
	Model* mpModel;

	AIState mCurrentState;
	CharacterState* mpCurrentState;

	int mActionTimer;
	float mfSpeed;

	float mfTargetAngle;
	float mfAngle;
	const float ROTATE_SPEED = 0.2f;

	VECTOR UpMoveVector;
	VECTOR moveVec;
	VECTOR oldmoveVec;
	VECTOR hitPos;

	int mVacuumTimer;
	bool mDeleteFlag;
	float mfdeathTime;

	float mfScore;
	float mfXp;
	bool mbBaitFlag;
	bool mbIsVisible;
};
