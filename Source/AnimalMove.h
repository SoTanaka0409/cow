#pragma once
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// 自律移動と捕獲（吸い込み）状態を管理する動物の基底クラス
class AnimalMove : public CharacterMove
{
public:
	// コンボボーナス計算に用いる動物種別
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

	void SetTag_animal(Tag_animal tag) { mntag_animal = tag; }
	Tag_animal GetTag_animal() { return mntag_animal; }

protected:
	Tag_animal mntag_animal;
};
