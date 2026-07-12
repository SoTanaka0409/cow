#pragma once
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// 閾ｪ蠕狗ｧｻ蜍輔→謐慕佐・亥精縺・ｾｼ縺ｿ・臥憾諷九ｒ邂｡逅・☆繧句虚迚ｩ縺ｮ蝓ｺ蠎輔け繝ｩ繧ｹ
class AnimalMove : public CharacterMove
{
public:
	// 繧ｳ繝ｳ繝懊・繝ｼ繝翫せ險育ｮ励↓逕ｨ縺・ｋ蜍慕黄遞ｮ蛻･
	enum TagAnimal
	{
		kNone,
		kAnimal1,
		kAnimal2,
		kAnimal3,
		kAnimalGold,
		kAnimalT,
	};

public:
	/*
	 * @brief 蜍慕黄繧ｪ繝悶ず繧ｧ繧ｯ繝医・逕滓・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν繝代せ, initPos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｹ繝・・繧ｿ繧ｹ蛻晄悄蛹・
	 */
	AnimalMove(std::string filename, VECTOR initPos);
	virtual ~AnimalMove();

	/*
	 * @brief 繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ遘ｻ蜍募・逅・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蠎ｧ讓吶・譖ｴ譁ｰ
	 */
	virtual void MoveCharacter() override;
	virtual void Reset(VECTOR pos) override;

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	virtual void CharacterDied() override;
	virtual void Die(DeathReason reason) override;

	void SetTagAnimal(TagAnimal tag) { tag_animal_ = tag; }
	TagAnimal GetTagAnimal() { return tag_animal_; }

protected:
	TagAnimal tag_animal_;
};