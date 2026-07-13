#pragma once

class CharacterMove;

class CharacterState
{
public:
	virtual ~CharacterState() = default;

	/*
	 * [蜈･蜉嫋 character: 迥ｶ諷九ｒ驕ｩ逕ｨ縺吶ｋ蟇ｾ雎｡繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 迥ｶ諷矩幕蟋区凾縺ｮ蛻晄悄蛹門・逅・
	 */
	virtual void Enter(CharacterMove* character) {}

	/*
	 * [蜈･蜉嫋 character: 譖ｴ譁ｰ蟇ｾ雎｡縺ｮ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蠎ｧ讓吶ｄ繝輔Λ繧ｰ縺ｪ縺ｩ繧呈ｯ弱ヵ繝ｬ繝ｼ繝譖ｴ譁ｰ縺吶ｋ
	 */
	virtual void Update(CharacterMove* character) = 0;

	/*
	 * [蜈･蜉嫋 character: 迥ｶ諷九ｒ隗｣髯､縺吶ｋ蟇ｾ雎｡繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 迥ｶ諷狗ｵゆｺ・凾縺ｮ繧ｯ繝ｪ繝ｼ繝ｳ繧｢繝・・蜃ｦ逅・
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
