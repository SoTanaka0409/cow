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
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] AI譖ｴ譁ｰ縺ｨ螢∝愛螳壹ｒ邨後※蠎ｧ讓吶ｒ遒ｺ螳壹☆繧・
	 */
	virtual void MoveCharacter();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 迴ｾ蝨ｨ縺ｮState縺ｮUpdate繧貞他縺ｳ蜃ｺ縺・
	 */
	virtual void UpdateWanderAI();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 螢√が繝悶ず繧ｧ繧ｯ繝医→縺ｮ蛻､螳壹ｒ陦後＞縲［vPosition繧定｣懈ｭ｣縺吶ｋ
	 */
	virtual void CheckWallCollision();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ菴咲ｽｮ繧偵Δ繝・Ν縺ｫ蜷梧悄縺輔○繧・
	 */
	virtual void ColliderMove();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 遘ｻ蜍輔・繧ｯ繝医Ν譁ｹ蜷代∈繝｢繝・Ν繧貞屓霆｢縺輔○繧・
	 */
	virtual void RotationCharacter();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繝｢繝・Ν繧剃ｸ螳夐溷ｺｦ縺ｧ譌句屓縺輔○邯壹￠繧・
	 */
	virtual void CharacterRotate();

	virtual void CharacterDied();

	/*
	 * [蜈･蜉嫋 reason: 豁ｻ莠｡縺ｮ逅・罰(DeathReason)
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 豁ｻ莠｡貍泌・繧・ヵ繝ｩ繧ｰ譖ｴ譁ｰ繧定｡後≧
	 */
	virtual void Die(DeathReason reason);

	virtual void Reset(VECTOR pos);
	virtual void Deactivate();

	void AddAnimation(AnimationState state, std::string filename) {}
	void SetScale(float scale);

	AIState GetCurrentState() const { return mCurrentState; }
	void SetCurrentState(AIState state) { mCurrentState = state; }

	/*
	 * [蜈･蜉嫋 newState: 譁ｰ縺励＞迥ｶ諷九け繝ｩ繧ｹ縺ｮ繝昴う繝ｳ繧ｿ
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜿､縺Тtate繧堤ｴ譽・＠縲∵眠縺励＞State縺ｫ遘ｻ陦後☆繧・
	 */
	void ChangeState(CharacterState* newState);

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 迥ｶ諷九ｒ蜷ｸ蠑穂ｸｭ(STATE_VACUUM)縺ｫ螟画峩縺吶ｋ
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
	float mfdeathTime;

	float mfScore;
	float mfXp;
	bool mbBaitFlag;
	bool mbIsVisible;
};