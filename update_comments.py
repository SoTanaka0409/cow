import codecs

# 1. Animal.h
animal_h_content = """#pragma once
#include "AnimalMove.h"

// 仕様制約: プレイヤーのターゲット対象となる牛以外の一般動物を定義する
class Animal : public AnimalMove
{
public:
	/*
	 * @brief 動物オブジェクトの生成と初期配置
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] オブジェクトの初期化および内部パラメータの設定
	 */
	Animal(std::string filename, VECTOR initPos);
	virtual ~Animal();

	/*
	 * @brief オブジェクトの毎フレーム更新
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標やコライダーなどの内部状態更新
	 */
	void Update() override;
};
"""

# 2. Animal.cpp
animal_cpp_content = """#include "Animal.h"

Animal::Animal(std::string filename, VECTOR initPos)
	: AnimalMove(filename, initPos)
{
	// Animal_1をデフォルトとし、固有の経験値を設定
	mfXp = 10;
	SetTag_animal(AnimalMove::Animal_1);
}

Animal::~Animal()
{
}

void Animal::Update()
{
	MoveCharacter();

	// 吸い込まれ中は不自然な回転を防ぐため回転処理をスキップする
	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCharacter();
	}

	ColliderMove();
	CharacterDied();
	mpModel->Update();
}
"""

# 3. AnimalMove.h
animal_move_h_content = """#pragma once
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
"""

# 4. AnimalMove.cpp
animal_move_cpp_content = """#include "AnimalMove.h"
#include "GameConstants.h"
#include "Master.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Wall.h"
#include "stage.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include <cmath>
#include "Bait.h"
#include "TutorialScene.h"
#include "ServiceLocator.h"
#include "Player3D.h"

namespace {
	// 暫定対応: コンボボーナス計算用のグローバル変数群
	int s_mnTagCount = 0;
	AnimalMove::Tag_animal s_tag1 = AnimalMove::none;
	AnimalMove::Tag_animal s_tag2 = AnimalMove::none;
	AnimalMove::Tag_animal s_tag3 = AnimalMove::none;
}

AnimalMove::AnimalMove(std::string filename, VECTOR initPos)
	: CharacterMove(filename, initPos)
{
	// 基礎パラメータとして羊の定数を適用
	mfSpeed = GameConstants::ANIMAL_SHEEP.speed;
	mActionTimer = 60;
	mfScore = GameConstants::ANIMAL_SHEEP.score;
	mfXp = GameConstants::ANIMAL_SHEEP.xp;
	mbBaitFlag = false;
	mfdeathTime = GameConstants::ANIMAL_SHEEP.deathTimeHeight;
	SetTag(Object3D::Tag3D_Animal);
}

AnimalMove::~AnimalMove()
{
}

void AnimalMove::Reset(VECTOR pos)
{
	CharacterMove::Reset(pos);

	if (mpCapsuleCollider != nullptr)
	{
		mpCapsuleCollider->mvPosition = pos;
	}
}

void AnimalMove::MoveCharacter()
{
	CharacterMove::MoveCharacter();
}

void AnimalMove::AddAnimation(AnimationState state, std::string filename)
{
}

void AnimalMove::OnEnter(Collider* collider, Collider* check)
{
	// 餌オブジェクトへの接触を検知し、誘導フラグを立てる
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = true;
		}
	}
}

void AnimalMove::OnTrigger(Collider* collider, Collider* check)
{
}

void AnimalMove::OnExit(Collider* collider, Collider* check)
{
	// 餌の有効範囲外に出たため誘導フラグを解除する
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Bait)
		{
			mbBaitFlag = false;
		}
	}
}

void AnimalMove::CharacterDied()
{
	// 演出都合上、フィーバー中および吸い込み状態以外では死亡判定を行わない
	if (mCurrentState != STATE_VACUUM||ServiceLocator::GetFever()->IsFever()) return;

	Player3D* player = mpTargetPlayer;

	CharacterRotate();
	if (player != nullptr)
	{
		mvPosition.y += player->Status(Player3D::Status_AttackS);
	}

	// プレイヤーへ向けて浮遊し、一定高度に達した段階で捕獲完了とする
	if (mvPosition.y > mfdeathTime && !mDeleteFlag)
	{
		Die(DEATH_VACUUM);
	}

	mpModel->SetPosition(mvPosition);
}

void AnimalMove::Die(DeathReason reason)
{
	if (mDeleteFlag) return;

	Player3D* player = mpTargetPlayer;

	switch (reason)
	{
	case DEATH_VACUUM:
	case DEATH_BAIT:
		if (player != nullptr)
		{
			player->mpLevel->AddXp(mfXp);
			player->mpCombo->Reset();
			player->mpScore->AddScore(mfScore);

			// 暫定対応: 同種連続捕獲時に追加経験値を付与するためのコンボロジック
			if (mntag_animal == AnimalMove::Tag_animal::Animal_T)
			{
				Master::mnTutorialcount++;
			}

			s_mnTagCount++;
			if (s_mnTagCount == 1)
			{
				s_tag1 = mntag_animal;
			}
			else if (s_mnTagCount == 2 && s_tag1 == mntag_animal)
			{
				s_tag2 = mntag_animal;
			}
			else if (s_mnTagCount == 3 && s_tag2 == mntag_animal)
			{
				s_tag3 = mntag_animal;
				if (s_tag3 == AnimalMove::Animal_1) player->mpLevel->AddXp(10);
				if (s_tag2 == AnimalMove::Animal_2) player->mpLevel->AddXp(20);
				if (s_tag3 == AnimalMove::Animal_3) player->mpLevel->AddXp(30);
			}
			else
			{
				s_mnTagCount = 0;
				s_tag1 = AnimalMove::none;
				s_tag2 = AnimalMove::none;
				s_tag3 = AnimalMove::none;
			}
		}
		mDeleteFlag = true;
		break;

	case DEATH_LIMIT:
		mDeleteFlag = true;
		break;
	}
}
"""

def save_file(path, content):
    with codecs.open(path, 'w', 'utf-8-sig') as f:
        f.write(content.replace('\\r\\n', '\\n').replace('\\n', '\\r\\n'))

save_file(r"c:\\Users\\student\\Desktop\\programing\\cow\\Source\\Animal.h", animal_h_content)
save_file(r"c:\\Users\\student\\Desktop\\programing\\cow\\Source\\Animal.cpp", animal_cpp_content)
save_file(r"c:\\Users\\student\\Desktop\\programing\\cow\\Source\\AnimalMove.h", animal_move_h_content)
save_file(r"c:\\Users\\student\\Desktop\\programing\\cow\\Source\\AnimalMove.cpp", animal_move_cpp_content)

print("Files updated successfully.")
