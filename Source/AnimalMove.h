#pragma once
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// 自律移動と捕獲（吸い込み）状態を管理する動物の基底クラス
class AnimalMove : public CharacterMove
{
public:
	// コンボボーナス計算に用いる動物種別
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
	 * @brief 動物オブジェクトの生成
	 * [入力] filename: モデルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] ステータス初期化
	 */
	AnimalMove(std::string filename, VECTOR initPos);
	virtual ~AnimalMove();

	/*
	 * @brief キャラクターの移動処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標の更新
	 */
	virtual void MoveCharacter() override;
	virtual void Reset(VECTOR pos) override;

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	virtual void CharacterDied() override;
	virtual void Die(DeathReason reason) override;

	void SetTag_animal(TagAnimal tag) { tag_animal_ = tag; }
	TagAnimal GetTag_animal() { return tag_animal_; }

protected:
	TagAnimal tag_animal_;
};