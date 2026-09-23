#pragma once
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

/// @brief プレイヤーによる捕獲と自律移動の状態遷移を管理するため
class AnimalMove : public CharacterMove
{
public:
	/// @brief コンボボーナスを動物の種類ごとに計算するため
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
	/// @brief 動物の3Dモデルと初期設定を行うため
	/// @param filename モデルのファイルパス
	/// @param init_pos 出現座標
	/// @details コライダーやステータスの初期化
	AnimalMove(const std::string& filename, VECTOR init_pos);
	virtual ~AnimalMove();

	/// @brief 動物ごとのAIや移動ロジックを実行するため
	/// @details 座標と向きの更新
	virtual void MoveCharacter() override;

	/// @brief 特定の座標からリスポーンさせるため
	/// @param pos 復帰先の座標
	/// @details 座標の変更および状態のリセット
	virtual void Reset(VECTOR pos) override;

	/// @brief 状態に応じたアニメーションを割り当てるため
	/// @param state アニメーション状態
	/// @param filename アニメーションのファイルパス
	/// @details アニメーションリストへの追加
	void AddAnimation(AnimationState state, const std::string& filename);

	/// @brief 衝突判定の開始を処理するため
	/// @param collider 自身のコライダー
	/// @param check 相手のコライダー
	/// @details 衝突フラグの設定
	virtual void OnEnter(Collider* collider, Collider* check) override;

	/// @brief 衝突中の継続的な処理を行うため
	/// @param collider 自身のコライダー
	/// @param check 相手のコライダー
	/// @details ダメージや押し出しの適用
	virtual void OnTrigger(Collider* collider, Collider* check) override;

	/// @brief 衝突判定の終了を処理するため
	/// @param collider 自身のコライダー
	/// @param check 相手のコライダー
	/// @details 衝突フラグの解除
	virtual void OnExit(Collider* collider, Collider* check) override;

	/// @brief 死亡時のエフェクトやスコア処理を行うため
	/// @details スコア加算とオブジェクト削除要求
	virtual void CharacterDied() override;

	/// @brief 特定の死因に応じて死亡処理を分岐させるため
	/// @param reason 死因
	/// @details 死亡状態の設定とHP減少
	virtual void Die(DeathReason reason) override;

	/// @brief コンボ判定用に動物の種類を設定するため
	/// @param tag 設定するタグ
	/// @details tag_animal_の変更
	void SetTagAnimal(TagAnimal tag) { tag_animal_ = tag; }

	/// @brief 現在の動物の種類を取得するため
	/// @return 設定されている動物タグ
	TagAnimal GetTagAnimal() { return tag_animal_; }

protected:
	TagAnimal tag_animal_; ///< 現在の状態や種別を管理する値
};
