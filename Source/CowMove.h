#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "EffekseerEffect.h"
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// AI縺ｫ繧医ｋ閾ｪ蠕狗ｧｻ蜍輔→蜷ｸ縺・ｾｼ縺ｿ蛻､螳壹ｒ邂｡逅・☆繧句渕蠎輔け繝ｩ繧ｹ
class CowMove : public CharacterMove
{
public:
	// 蛟句挨莉墓ｧ伜・蟯舌・縺溘ａ縺ｮ迚帷ｨｮ鬘櫁ｭ伜挨逕ｨ繧ｿ繧ｰ
	enum TagCow
	{
		kNone,
		kCow1,
		kCow2,
		kCow3,
		kCowGold,
		kCowT,
	};

public:
	/*
	 * @brief 蛻晄悄蛹・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝代せ, initPos: 蛻晄悄驟咲ｽｮ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] Effekseer繧ｨ繝輔ぉ繧ｯ繝医・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	 */
	CowMove(std::string filename, VECTOR initPos);
	virtual ~CowMove();

	void Update() override;
	void Draw() override;

	/*
	 * @brief 遘ｻ蜍募・逅・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蠎ｧ讓吶・譖ｴ譁ｰ縲、I閾ｪ蠕狗ｧｻ蜍輔♀繧医・蜷ｸ蠑墓凾荳頑・蜷梧悄
	 */
	virtual void MoveCharacter() override;

	virtual void ColliderMove() override;

	virtual void Reset(VECTOR pos) override;

	void AvoidOtherCows();

	bool SeekBait();

	virtual void UpdateWanderAI() override;

	virtual void CheckWallCollision() override;

	virtual void RotationCharacter() override;

	virtual void CharacterRotate() override;

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * @brief 髯千阜鬮伜ｺｦ蛻ｰ驕疲凾縺ｮ豸域ｻ・・逅・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｹ繧ｳ繧｢・邨碁ｨ灘､縺ｮ蜉邂励√お繝輔ぉ繧ｯ繝亥・逕・
	 */
	virtual void CharacterDied() override;

	/*
	 * @brief 鬢後↓繧医ｋ繧ｭ繝ｫ蜃ｦ逅・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜑企勁繝輔Λ繧ｰ縺ｮ譛牙柑蛹・
	 */
	virtual void KilledByBait();

	/*
	 * @brief 豁ｻ莠｡譎ゅ・蜈ｱ騾壼・逅・
	 * [蜈･蜉嫋 reason: 豁ｻ莠｡逅・罰
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｳ繝ｳ繝懊・繧ｹ繧ｳ繧｢縺ｮ險育ｮ励∝炎髯､繝輔Λ繧ｰ縺ｮ譛牙柑蛹・
	 */
	virtual void Die(DeathReason reason) override;

	void SetTagCow(TagCow tag) { tag_cow_ = tag; }
	TagCow GetTagCow() { return tag_cow_; }

	void SetColliderRadius(float radius) { collider_radius_ = radius; }

protected:
	TagCow tag_cow_;                  // 蛟句挨莉墓ｧ伜・蟯舌・縺溘ａ縺ｮ繧ｿ繧ｰ
	float collider_radius_ = 50.0f;      // 陦晉ｪ∝愛螳夂畑縺ｮ蜊雁ｾ・宛邏・

	EffekseerEffect* cow_vm_;           // 蜷ｸ蠑墓ｼ泌・逕ｨ繧ｨ繝輔ぉ繧ｯ繝・
	int effect_timer_;                   // 繧ｨ繝輔ぉ繧ｯ繝亥・逕溷ｮ御ｺ・∪縺ｧ縺ｮ蠕・ｩ溘ヵ繝ｬ繝ｼ繝
	bool tutorial_vacum_flag_;              // 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ迚ｹ蛻･縺ｪ蛻､螳壹ヵ繝ｩ繧ｰ
};


