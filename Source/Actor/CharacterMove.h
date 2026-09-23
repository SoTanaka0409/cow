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
	kStateIdle,
	kStateWalk,
	kStateVacuum
};
enum DeathReason
{
	kDeathVacuum,
	kDeathBait,
	kDeathLimit
};
class CharacterMove : public Object3D
{
public:
	/// @brief TargetPlayerの設定
	void SetTargetPlayer(Player3D* player) { target_player_ = player; }
	/// @brief TargetPlayerの取得
	Player3D* GetTargetPlayer() const { return target_player_; }
	CharacterMove(const std::string& filename, VECTOR init_pos);
	virtual ~CharacterMove();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DrawShadowCaster() override;
	/// @brief キャラクターの挙動を毎フレーム反映するため。
	/// @details 座標(mvPosition)の更新
	virtual void MoveCharacter();
	/// @brief 状態に応じた自律移動を実現するため。
	/// @details StateクラスのUpdate実行
	virtual void UpdateWanderAI();
	/// @brief 壁へのめり込みを防ぐため。
	/// @details mvPositionの補正
	virtual void CheckWallCollision();
	/// @brief 当たり判定をモデルの現在位置に合わせるため。
	/// @details コライダーの座標更新
	virtual void ColliderMove();
	/// @brief 進行方向にキャラクターを向かせるため。
	/// @details モデルの回転角更新
	virtual void RotationCharacter();
	/// @brief 演出としてキャラクターを旋回させるため。
	/// @details モデルの回転角更新
	virtual void CharacterRotate();
	virtual void CharacterDied();
	/// @brief キャラクターの死亡状態を確定させるため。
	/// @param reason 死亡の理由
	/// @details 死亡フラグや演出の開始
	virtual void Die(DeathReason reason);
	virtual void Reset(VECTOR pos);
	virtual void Deactivate();
	void AddAnimation(AnimationState state, const std::string& filename) {}
	/// @brief Scaleの設定
	void SetScale(float scale);
	/// @brief CurrentStateの取得
	AIState GetCurrentState() const { return ai_state_; }
	/// @brief CurrentStateの設定
	void SetCurrentState(AIState state) { ai_state_ = state; }
	/// @brief 行動パターンを切り替えるため。
	/// @param newState 新しい状態クラスのポインタ
	/// @details 古いStateの破棄と新しいStateへの移行
	void ChangeState(CharacterState* newState);
	/// @brief プレイヤーに吸い込まれる挙動を開始するため。
	/// @details mCurrentStateをSTATE_VACUUMに変更
	void ChangeStateToVacuum();
	/// @brief ActionTimerの設定
	void SetActionTimer(int timer) { action_timer_ = timer; }
	/// @brief 処理の実行
	void DecreaseActionTimer() { action_timer_--; }
	/// @brief ActionTimerの取得
	int GetActionTimer() const { return action_timer_; }
	/// @brief MoveVecの設定
	void SetMoveVec(VECTOR vec) { move_vec_ = vec; }
	/// @brief MoveVecの取得
	VECTOR GetMoveVec() const { return move_vec_; }
	/// @brief Speedの取得
	float GetSpeed() const { return speed_; }
	/// @brief 処理の実行
	void IncreaseVacuumTimer() { vacuum_timer_++; }
	/// @brief 処理の実行
	void ResetVacuumTimer() { vacuum_timer_ = 0; }
	/// @brief VacuumTimerの取得
	int GetVacuumTimer() const { return vacuum_timer_; }
	/// @brief BaitFlagの取得
	bool GetBaitFlag() const { return bait_flag_; }
	/// @brief CharacterDeleteの取得
	bool GetCharacterDelete() const { return mDeleteFlag; }
protected:
	Player3D* target_player_ = nullptr; ///< 参照対象のオブジェクトを保持するポインタ
	Model* model_;                      ///< 3Dモデルの管理に使用する情報
	AIState ai_state_;              ///< 現在の状態や種別を管理する値
	CharacterState* current_state_;     ///< 現在の状態や種別を管理する値
	int action_timer_;                   ///< 時間経過や処理間隔を管理するカウンター
	float speed_;                       ///< 移動や回転の計算に使用する値
	float target_angle_;                ///< 移動や回転の計算に使用する値
	float angle_;                       ///< 移動や回転の計算に使用する値
	const float kRotateSpeed = 0.2f;    ///< ゲーム内で使用する固定値
	VECTOR UpMoveVector;                ///< 移動や回転の計算に使用する値
	VECTOR move_vec_;                     ///< 移動や回転の計算に使用する値
	VECTOR oldmoveVec;                  ///< 移動や回転の計算に使用する値
	VECTOR hitPos;                      ///< 座標や位置情報を管理する値
	int vacuum_timer_;                   ///< 時間経過や処理間隔を管理するカウンター
	bool mDeleteFlag;                   ///< 状態の有効・無効を管理するフラグ
	float death_timer_;                 ///< 時間経過や処理間隔を管理するカウンター
	float score_;                       ///< スコアや成長値の管理に使用する値
	float xp_;                          ///< スコアや成長値の管理に使用する値
	bool bait_flag_;                    ///< 状態の有効・無効を管理するフラグ
	bool is_visible_;                   ///< 状態の有効・無効を管理するフラグ
};
