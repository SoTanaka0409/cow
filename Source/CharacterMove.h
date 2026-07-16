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
	void SetTargetPlayer(Player3D* player) { target_player_ = player; }
	Player3D* GetTargetPlayer() const { return target_player_; }
	CharacterMove(std::string filename, VECTOR initPos);
	virtual ~CharacterMove();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DrawShadowCaster() override;
	/*
	 * キャラクターの挙動を毎フレーム反映するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標(mvPosition)の更新
	 */
	virtual void MoveCharacter();
	/*
	 * 状態に応じた自律移動を実現するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] StateクラスのUpdate実行
	 */
	virtual void UpdateWanderAI();
	/*
	 * 壁へのめり込みを防ぐため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mvPositionの補正
	 */
	virtual void CheckWallCollision();
	/*
	 * 当たり判定をモデルの現在位置に合わせるため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コライダーの座標更新
	 */
	virtual void ColliderMove();
	/*
	 * 進行方向にキャラクターを向かせるため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] モデルの回転角更新
	 */
	virtual void RotationCharacter();
	/*
	 * 演出としてキャラクターを旋回させるため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] モデルの回転角更新
	 */
	virtual void CharacterRotate();
	virtual void CharacterDied();
	/*
	 * キャラクターの死亡状態を確定させるため。
	 * [入力] reason: 死亡の理由
	 * [出力] なし
	 * [副作用] 死亡フラグや演出の開始
	 */
	virtual void Die(DeathReason reason);
	virtual void Reset(VECTOR pos);
	virtual void Deactivate();
	void AddAnimation(AnimationState state, std::string filename) {}
	void SetScale(float scale);
	AIState GetCurrentState() const { return mCurrentState; }
	void SetCurrentState(AIState state) { mCurrentState = state; }
	/*
	 * 行動パターンを切り替えるため。
	 * [入力] newState: 新しい状態クラスのポインタ
	 * [出力] なし
	 * [副作用] 古いStateの破棄と新しいStateへの移行
	 */
	void ChangeState(CharacterState* newState);
	/*
	 * プレイヤーに吸い込まれる挙動を開始するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mCurrentStateをSTATE_VACUUMに変更
	 */
	void ChangeStateToVacuum();
	void SetActionTimer(int timer) { mActionTimer = timer; }
	void DecreaseActionTimer() { mActionTimer--; }
	int GetActionTimer() const { return mActionTimer; }
	void SetMoveVec(VECTOR vec) { moveVec = vec; }
	VECTOR GetMoveVec() const { return moveVec; }
	float GetSpeed() const { return speed_; }
	void IncreaseVacuumTimer() { mVacuumTimer++; }
	void ResetVacuumTimer() { mVacuumTimer = 0; }
	int GetVacuumTimer() const { return mVacuumTimer; }
	bool GetBaitFlag() const { return bait_flag_; }
	bool GetCharacterDelete() const { return mDeleteFlag; }
protected:
	Player3D* target_player_ = nullptr;
	Model* model_;
	AIState mCurrentState;
	CharacterState* current_state_;
	int mActionTimer;
	float speed_;
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
	float score_;
	float xp_;
	bool bait_flag_;
	bool is_visible_;
};
