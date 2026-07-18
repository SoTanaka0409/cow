#pragma once

class CharacterMove;

// 設計ルール：キャラクターの複雑な状態遷移（アイドル・移動・吸引等）のバグを防ぎ、追加・保守を容易にするStateパターンの基底クラス
class CharacterState
{
public:
	virtual ~CharacterState() = default;

	// 入力：character=状態を適用する対象キャラクターのポインタ
	// 副作用：アニメーションの切り替え、タイマーリセットなど状態遷移時の初期化
	virtual void Enter(CharacterMove* character) {}

	// 入力：character=更新対象のキャラクターのポインタ
	// 副作用：キャラクター座標、移動ベクトル、アニメーションフレームの毎フレーム更新
	virtual void Update(CharacterMove* character) = 0;

	// 入力：character=状態を解除する対象キャラクターのポインタ
	// 副作用：エフェクトの停止、一時的な移動バフや制限フラグのクリーンアップ
	virtual void Exit(CharacterMove* character) {}
};

// 業務ルール：入力がない時、または現在の移動速度がゼロの時に遷移する待機（アイドル）状態
class StateIdle : public CharacterState
{
public:
	void Enter(CharacterMove* character) override;
	void Update(CharacterMove* character) override;
};

// 業務ルール：コントローラーやキーボードによる移動入力中に遷移する通常の歩行状態
class StateWalk : public CharacterState
{
public:
	void Enter(CharacterMove* character) override;
	void Update(CharacterMove* character) override;
};

// 業務ルール：UFOからのアブダクションビームに囚われ、空中へ吸い上げられている最中の特殊被ダメージ状態
class StateVacuum : public CharacterState
{
public:
	void Enter(CharacterMove* character) override;
	void Update(CharacterMove* character) override;
};
