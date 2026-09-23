#pragma once

class CharacterMove;

/// @brief 設計ルール：キャラクターの複雑な状態遷移（アイドル・移動・吸引等）のバグを防ぎ、追加・保守を容易にするStateパターンの基底クラス
class CharacterState
{
public:
	virtual ~CharacterState() = default;

	/// @param character 状態を適用する対象キャラクターのポインタ
	/// @brief アニメーションの切り替え、タイマーリセットなど状態遷移時の初期化
	virtual void Enter(CharacterMove* character) {}

	/// @param character 更新対象のキャラクターのポインタ
	/// @brief キャラクター座標、移動ベクトル、アニメーションフレームの毎フレーム更新
	virtual void Update(CharacterMove* character) = 0;

	/// @param character 状態を解除する対象キャラクターのポインタ
	/// @brief エフェクトの停止、一時的な移動バフや制限フラグのクリーンアップ
	virtual void Exit(CharacterMove* character) {}
};

/// @brief 入力がない時、または現在の移動速度がゼロの時に遷移する待機（アイドル）状態
class StateIdle : public CharacterState
{
public:
	/// @brief 処理の実行
	void Enter(CharacterMove* character) override;
	/// @brief 毎フレームの更新処理
	void Update(CharacterMove* character) override;
};

/// @brief コントローラーやキーボードによる移動入力中に遷移する通常の歩行状態
class StateWalk : public CharacterState
{
public:
	/// @brief 処理の実行
	void Enter(CharacterMove* character) override;
	/// @brief 毎フレームの更新処理
	void Update(CharacterMove* character) override;
};

/// @brief UFOからのアブダクションビームに囚われ、空中へ吸い上げられている最中の特殊被ダメージ状態
class StateVacuum : public CharacterState
{
public:
	/// @brief 処理の実行
	void Enter(CharacterMove* character) override;
	/// @brief 毎フレームの更新処理
	void Update(CharacterMove* character) override;
};
