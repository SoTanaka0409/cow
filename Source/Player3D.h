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
	bool vacuumFlag = true;
public:
	// ステータスID（UIやスキル加算判定に使用）
	enum StatusID
	{
		Status_Hp,
		Status_AttackS,
		Status_Speed,
	};

	bool GetVacuumFlag() const
	{
		return vacuumFlag;
	}

public:
	/*
	 * [入力] filename: モデルファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] 各マネージャー(Score, Level等)とコライダーの生成
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
	 * [副作用] mpCapsuleColliderとビームエフェクトの更新
	 */
	void ColliderUpdate();

	/*
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面外時の座標リセット
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
	 * [副作用] サブモジュールのUpdate/Draw呼び出し
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
	 * [副作用] デバッグ時の状態変化
	 */
	void test();

	/*
	 * [入力] id: ステータスID
	 * [出力] スキル補正後のステータス値
	 * [副作用] なし
	 */
	float Status(StatusID id);

	void SetStatusAttack(float f) { attack_Speed = f; }
	float GetStatusAttack() { return attack_Speed; }

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	CapsuleCollider* GetCollisionCollider() { return capsuleCollider; }

	void SetScale(float scale);

	/*
	 * [入力] stunTime: 気絶時間（フレーム数）
	 * [出力] なし
	 * [副作用] mIsStunnedをtrueにし、操作不能時間を発生させる
	 */
	void ApplyStun(int stunTime);

	void PlaySkillEffect();

public:
	Model* model;
	Skill* skill;
	Level* level;
	Combo* combo;
	Score* score;

private:
	float speed;
	float hp;
	float attack_Speed;
	float targetAngle;
	float angle;
	const float ROTATE_SPEED = 0.2f;
	const float JUMP_POWER = 30.0f;
	bool isOutOfBounds = false;
	int lighGraph;

	bool isCowInVacuumRange;
	const float VACUUM_RADIUS = 300.0f;
	const int VACUUM_REQUIRE_TIME = 120;

	float vacuumGauge = 100.0f;
	const float VACUUM_GAUGE_MAX = 100.0f;
	const float VACUUM_COST_PER_FRAME = 0.2f;
	const float VACUUM_RECOVER_PER_FRAME = 0.4f;
	bool isVacuumActive = false;

	int catchNowCount;
	float currentSpeed;

	VECTOR UpMoveVector;
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);
	VECTOR oldmoveVec;
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);

	float horizontalAngle;
	float verticalAngle;

	const int SIZE_RAND_MAX = 800;
	const int SIZE_RAND_MIN = 400;
	const int VISIBLE_TIME_RAND_MAX = 30;
	const int VISIBLE_TIME_RAND_MIN = 5;

	EffekseerEffect* speedEffect;
	EffekseerEffect* beam;
	int effectTimer;

	bool isStunned;
	int stunTimer;
};

