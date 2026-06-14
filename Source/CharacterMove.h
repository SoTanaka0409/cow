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
