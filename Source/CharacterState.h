#pragma once

class CharacterMove;

class CharacterState
{
public:
	virtual ~CharacterState() = default;

	/*
	 * [入力] character: 状態を適用する対象キャラクター
	 * [出力] �Ȃ�
	 * [副作用] 状態開始時の初期化�E琁E
	 */
	virtual void Enter(CharacterMove* character) {}

	/*
	 * [入力] character: 更新対象のキャラクター
	 * [出力] �Ȃ�
	 * [副作用] キャラクターの座標やフラグなどを毎フレーム更新する
	 */
	virtual void Update(CharacterMove* character) = 0;

	/*
	 * [入力] character: 状態を解除する対象キャラクター
	 * [出力] �Ȃ�
	 * [副作用] 状態終亁E��のクリーンアチE�E処琁E
	 */
	virtual void Exit(CharacterMove* character) {}
};

class StateIdle : public CharacterState
{
public:
	void Enter(CharacterMove* character) override;
	void Update(CharacterMove* character) override;
};

class StateWalk : public CharacterState
{
public:
	void Enter(CharacterMove* character) override;
	void Update(CharacterMove* character) override;
};

class StateVacuum : public CharacterState
{
public:
	void Enter(CharacterMove* character) override;
	void Update(CharacterMove* character) override;
};
