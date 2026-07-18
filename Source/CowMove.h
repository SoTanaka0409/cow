#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "EffekseerEffect.h"
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// プレイヤーの吸い込みアクションに対する挙動と、非干渉時のAI徘徊を管理する基底クラス
// 派生クラスでの固有処理（金牛の逃走など）を前提とするため、仮想関数のオーバーライドと呼び出し順に注意すること
class CowMove : public CharacterMove
{
public:
	// RTTI(dynamic_cast)の多用によるパフォーマンス低下を防ぐための識別用列挙型
	// 種類ごとのスコア倍率計算や、専用アニメーションの分岐判定において使用する
	enum TagCow
	{
		kNone,
		kCow1,
		kCow2,
		kCow3,
		kCowGold,
		kCowT,
	};

public:
	// [入力] filename: モデルパス, initPos: 初期配置座標 [出力] なし [副作用] エフェクトのインスタンス確保
	// 非同期読み込み未対応のため、大量生成によるメインスレッドのブロック（処理落ち）に注意すること
	CowMove(std::string filename, VECTOR initPos);
	virtual ~CowMove();

	void Update() override;
	void Draw() override;

	// [入力] なし [出力] なし [副作用] AI状態に基づく座標計算、および吸い込み時の強制浮上
	// 物理演算（重力など）との競合による座標の暴れを防ぐため、吸い込み中は通常の移動処理をバイパスする
	virtual void MoveCharacter() override;

	// モデルの移動後に当たり判定が取り残され、壁抜けが発生するバグを防ぐための座標同期処理
	virtual void ColliderMove() override;

	// [入力] pos: 復帰座標 [出力] なし [副作用] 速度・AI状態などの初期化
	// オブジェクトプールでの再利用時に、前回の状態（吸い込み中など）が残存するバグを防ぐため確実に呼ぶこと
	virtual void Reset(VECTOR pos) override;

	// 密集状態による複数モデルのめり込みや、進行不能（スタック）バグを防ぐための反発ベクトル計算処理
	void AvoidOtherCows();

	// 徘徊AIに優先して実行される誘導処理。毎フレームの厳密な経路探索は重いため簡易的な直線追従に留める
	bool SeekBait();

	virtual void UpdateWanderAI() override;

	// 画面外への脱走や、地形裏への落下バグを防ぐためのワールド境界クランプ処理
	virtual void CheckWallCollision() override;

	virtual void RotationCharacter() override;
	virtual void CharacterRotate() override;

	void AddAnimation(AnimationState state, std::string filename);

	// 多重発火による複数回ダメージや連続吸い込みを防ぐため、内部で状態フラグによるガードを必ず行うこと
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	// [入力] なし [出力] なし [副作用] スコア・経験値加算およびエフェクト再生
	// UFO到達（吸い込み完了）時の専用処理。二重加算バグを防ぐため、実行後は直ちに判定を無効化すること
	virtual void CharacterDied() override;

	// [入力] なし [出力] なし [副作用] 削除フラグの有効化
	// 餌によるキルはプレイヤーの吸い込みとはスコア計算の業務ルールが異なるため、専用の処理経路として分離
	virtual void KilledByBait();

	// [入力] reason: 死亡要因 [出力] なし [副作用] 要因別スコア計算と破棄フラグ有効化
	// 将来的な死因（ギミック死など）の追加に備え、スコア加算の共通処理と個別演出の分岐をここに集約する
	virtual void Die(DeathReason reason) override;

	void SetTagCow(TagCow tag) { tag_cow_ = tag; }
	TagCow GetTagCow() { return tag_cow_; }

	void SetColliderRadius(float radius) { collider_radius_ = radius; }

protected:
	// O(1)で派生型を判定するためのキャッシュ。初期化時の設定漏れによるスコア未加算バグに注意
	TagCow tag_cow_;

	// 吸い込み範囲や壁とのめり込み補正の基準値。モデルスケールを変更する際は必ずこれも同期させること
	float collider_radius_ = 50.0f;

	// 毎回の動的生成によるメモリ断片化とスパイクを防ぐため、初期化時に確保して使い回すエフェクトポインタ
	EffekseerEffect* cow_vm_;

	// 消滅エフェクトの再生完了を待ってからオブジェクトを完全破棄するための猶予時間管理
	int effect_timer_;

	// チュートリアル中の進行不能バグを防ぐため、特定の手順を踏むまで吸い込み状態への遷移をロックする制約フラグ
	bool tutorial_vacum_flag_;
};
