#pragma once
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// ステージ上を自律移動する一般動物（AI搭載アクター）のクラス
class AnimalMove : public CharacterMove
{
public:
	// 動物の種類識別用タグ
	enum Tag_animal
	{
		none,
		Animal_1,
		Animal_2,
		Animal_3,
		Animal_gold,
		Animal_T,
	};

public:
	AnimalMove(std::string filename, VECTOR initPos);
	virtual ~AnimalMove();

	virtual void MoveCharacter() override;
	virtual void Reset(VECTOR pos) override;

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	virtual void CharacterDied() override;
	virtual void Die(DeathReason reason) override;

	void SetTag_animal(Tag_animal tag) { mntag_animal = tag; }
	Tag_animal GetTag_animal() { return mntag_animal; }

protected:
	Tag_animal mntag_animal;            // タグ
};
