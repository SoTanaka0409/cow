#pragma once
#include "DxLib.h"
#include <string>
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "Skill.h"
#include "Level.h"
#include "camera.h"
#include "Combo.h"
#include "Score.h"
#include "EffekseerEffect.h"

class CapsuleCollider;

class Player3D : public Object3D
{
private:
	// 仕様により吸引アクションのオンオフを制御する
	bool mbVacuumFlag = true;
public:
	// スチE?EタスID?E?EIめE??キル加算判定に使用?E?E
	enum StatusID
	{
		Status_Hp,
		Status_AttackS,
		kStatusSpeed,
	};

	bool GetVacuumFlag() const
	{
		return mbVacuumFlag;
	}

public:
	/*
	 * [入力] filename: モチE??ファイルパス, initPos: 初期座?E
	 * [出力] なし
	 * [副作用] 吁E?Eネ?Eジャー(Score, Level?Eとコライダーの生?E
	 */
	Player3D(std::string filename, VECTOR initPos);
	virtual ~Player3D();

	void Draw() override;
	void Update() override;

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mvPositionの更新
	 */
	void MoveEx();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mIsVacuumActiveとmVacuumGaugeの更新
	 */
	void Play();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mpCapsuleColliderとビ?Eムエフェクト?E更新
	 */
	void ColliderUpdate();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面外時の座標リセチE??
	 */
	void ScreenOutCheck();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mvRotationの更新
	 */
	void RotationByMove();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] サブモジュールのUpdate/Draw呼び出ぁE
	 */
	void ManagerUpdate();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] UI描画
	 */
	void bar();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] チE??チE??時?E状態変化
	 */
	void test();

	/*
	 * [入力] id: スチE?EタスID
	 * [出力] スキル補正後?EスチE?Eタス値
	 * [副作用] なし
	 */
	float Status(StatusID id);

	void SetStatusAttack(float f) { mfAttack_Speed = f; }
	float GetStatusAttack() { return mfAttack_Speed; }

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	CapsuleCollider* GetCollisionCollider() { return capsule_collider_; }

	void SetScale(float scale);

	/*
	 * [入力] stunTime: 気絶時間?E?フレーム数?E?E
	 * [出力] なし
	 * [副作用] mIsStunnedをtrueにし、操作不?E時間を発生させる
	 */
	void ApplyStun(int stunTime);

	void PlaySkillEffect();

public:
	Model* model_;
	Skill* mpSkill;
	Level* mpLevel;
	Combo* combo_;
	Score* mpScore;

private:
	float mfSpeed;
	float mfHp;
	float mfAttack_Speed;
	float target_angle_;
	float angle_;
	const float kRotateSpeed = 0.2f;
	const float JUMP_POWER = 30.0f;
	bool mIsOutOfBounds = false;
	int mnLighGraph;
	int mnGaugeFrameGraph;

	bool mIsCowInVacuumRange;
	const float VACUUM_RADIUS = 300.0f;
	const int VACUUM_REQUIRE_TIME = 120;

	float mVacuumGauge = 100.0f;
	const float VACUUM_GAUGE_MAX = 100.0f;
	const float VACUUM_COST_PER_FRAME = 0.2f;
	const float VACUUM_RECOVER_PER_FRAME = 0.4f;
	bool mIsVacuumActive = false;

	int CatchNowCount;
	float currentSpeed;

	VECTOR UpMoveVector;
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);
	VECTOR oldmoveVec;
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);

	float horizontal_angle_;
	float vertical_angle_;

	const int SIZE_RAND_MAX = 800;
	const int SIZE_RAND_MIN = 400;
	const int VISIBLE_TIME_RAND_MAX = 30;
	const int VISIBLE_TIME_RAND_MIN = 5;

	EffekseerEffect* mpSpeed;
	EffekseerEffect* mpBeam;
	int effect_timer_;

	bool mIsStunned;
	int mStunTimer;
};