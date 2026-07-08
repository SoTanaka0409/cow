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
	// 莉墓ｧ倥↓繧医ｊ蜷ｸ蠑輔い繧ｯ繧ｷ繝ｧ繝ｳ縺ｮ繧ｪ繝ｳ繧ｪ繝輔ｒ蛻ｶ蠕｡縺吶ｋ
	bool mbVacuumFlag = true;
public:
	// 繧ｹ繝・・繧ｿ繧ｹID・・I繧・せ繧ｭ繝ｫ蜉邂怜愛螳壹↓菴ｿ逕ｨ・・
	enum StatusID
	{
		Status_Hp,
		Status_AttackS,
		Status_Speed,
	};

	bool GetVacuumFlag() const
	{
		return mbVacuumFlag;
	}

public:
	/*
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝輔ぃ繧､繝ｫ繝代せ, initPos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜷・・繝阪・繧ｸ繝｣繝ｼ(Score, Level遲・縺ｨ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ逕滓・
	 */
	Player3D(std::string filename, VECTOR initPos);
	virtual ~Player3D();

	void Draw() override;
	void Update() override;

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mvPosition縺ｮ譖ｴ譁ｰ
	 */
	void MoveEx();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mIsVacuumActive縺ｨmVacuumGauge縺ｮ譖ｴ譁ｰ
	 */
	void Play();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mpCapsuleCollider縺ｨ繝薙・繝繧ｨ繝輔ぉ繧ｯ繝医・譖ｴ譁ｰ
	 */
	void ColliderUpdate();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢螟匁凾縺ｮ蠎ｧ讓吶Μ繧ｻ繝・ヨ
	 */
	void ScreenOutCheck();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mvRotation縺ｮ譖ｴ譁ｰ
	 */
	void RotationByMove();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｵ繝悶Δ繧ｸ繝･繝ｼ繝ｫ縺ｮUpdate/Draw蜻ｼ縺ｳ蜃ｺ縺・
	 */
	void ManagerUpdate();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] UI謠冗判
	 */
	void bar();

	/*
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繝・ヰ繝・げ譎ゅ・迥ｶ諷句､牙喧
	 */
	void test();

	/*
	 * [蜈･蜉嫋 id: 繧ｹ繝・・繧ｿ繧ｹID
	 * [蜃ｺ蜉嫋 繧ｹ繧ｭ繝ｫ陬懈ｭ｣蠕後・繧ｹ繝・・繧ｿ繧ｹ蛟､
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
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
	 * [蜈･蜉嫋 stunTime: 豌礼ｵｶ譎る俣・医ヵ繝ｬ繝ｼ繝謨ｰ・・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mIsStunned繧稚rue縺ｫ縺励∵桃菴應ｸ崎・譎る俣繧堤匱逕溘＆縺帙ｋ
	 */
	void ApplyStun(int stunTime);

	void PlaySkillEffect();

public:
	Model* model_;
	Skill* mpSkill;
	Level* mpLevel;
	Combo* mpCombo;
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