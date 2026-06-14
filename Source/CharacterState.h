#pragma once

class CharacterMove;

class CharacterState
{
public:
	virtual ~CharacterState() = default;

	virtual void Enter(CharacterMove* character) {}
	virtual void Update(CharacterMove* character) = 0;
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
