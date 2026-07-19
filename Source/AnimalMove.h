#pragma once
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// プレイヤーによる捕獲と自律移動の状態遷移を管理するため
class AnimalMove : public CharacterMove
{
public:
	// コンボボーナスを動物の種類ごとに計算するため
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
	 * 動物の3Dモデルと初期設定を行うため
	 * [入力] filename: モデルのファイルパス, initPos: 出現座標
	 * [出力] なし
	 * [副作用] コライダーやステータスの初期化
	 */
	AnimalMove(std::string filename, VECTOR initPos);
	virtual ~AnimalMove();

	/*
	 * 動物ごとのAIや移動ロジックを実行するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標と向きの更新
	 */
	virtual void MoveCharacter() override;

	/*
	 * 特定の座標からリスポーンさせるため
	 * [入力] pos: 復帰先の座標
	 * [出力] なし
	 * [副作用] 座標の変更および状態のリセット
	 */
	virtual void Reset(VECTOR pos) override;

	/*
	 * 状態に応じたアニメーションを割り当てるため
	 * [入力] state: アニメーション状態, filename: アニメーションのファイルパス
	 * [出力] なし
	 * [副作用] アニメーションリストへの追加
	 */
	void AddAnimation(AnimationState state, std::string filename);

	/*
	 * 衝突判定の開始を処理するため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 衝突フラグの設定
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;

	/*
	 * 衝突中の継続的な処理を行うため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] ダメージや押し出しの適用
	 */
	virtual void OnTrigger(Collider* collider, Collider* check) override;

	/*
	 * 衝突判定の終了を処理するため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 衝突フラグの解除
	 */
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * 死亡時のエフェクトやスコア処理を行うため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] スコア加算とオブジェクト削除要求
	 */
	virtual void CharacterDied() override;

	/*
	 * 特定の死因に応じて死亡処理を分岐させるため
	 * [入力] reason: 死因
	 * [出力] なし
	 * [副作用] 死亡状態の設定とHP減少
	 */
	virtual void Die(DeathReason reason) override;

	/*
	 * コンボ判定用に動物の種類を設定するため
	 * [入力] tag: 設定するタグ
	 * [出力] なし
	 * [副作用] tag_animal_の変更
	 */
	void SetTagAnimal(TagAnimal tag) { tag_animal_ = tag; }

	/*
	 * 現在の動物の種類を取得するため
	 * [入力] なし
	 * [出力] 設定されている動物タグ
	 * [副作用] なし
	 */
	TagAnimal GetTagAnimal() { return tag_animal_; }

protected:
	TagAnimal tag_animal_;
};
