#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "EffekseerEffect.h"
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// AIによる自律移動と吸い込み判定を管理する基底クラス
class CowMove : public CharacterMove
{
public:
	// 個別仕様分岐のための牛種類識別用タグ
	enum Tag_cow
	{
		none,
		Cow_1,
		Cow_2,
		Cow_3,
		Cow_gold,
		Cow_T,
	};

public:
	/*
	 * @brief 初期化
	 * [入力] filename: モデルパス, initPos: 初期配置座標
	 * [出力] なし
	 * [副作用] Effekseerエフェクトのインスタンス生成
	 */
	CowMove(std::string filename, VECTOR initPos);
	virtual ~CowMove();

	void Update() override;
	void Draw() override;

	/*
	 * @brief 移動処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標の更新、AI自律移動および吸引時上昇同期
	 */
	virtual void MoveCharacter() override;

	virtual void ColliderMove() override;

	virtual void Reset(VECTOR pos) override;

	void AvoidOtherCows();

	bool SeekBait();

	virtual void UpdateWanderAI() override;

	virtual void CheckWallCollision() override;

	virtual void RotationCharacter() override;

	virtual void CharacterRotate() override;

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * @brief 限界高度到達時の消滅処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] スコア・経験値の加算、エフェクト再生
	 */
	virtual void CharacterDied() override;

	/*
	 * @brief 餌によるキル処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 削除フラグの有効化
	 */
	virtual void KilledByBait();

	/*
	 * @brief 死亡時の共通処理
	 * [入力] reason: 死亡理由
	 * [出力] なし
	 * [副作用] コンボ・スコアの計算、削除フラグの有効化
	 */
	virtual void Die(DeathReason reason) override;

	void SetTag_cow(Tag_cow tag) { mntag_cow = tag; }
	Tag_cow GetTag_cow() { return mntag_cow; }

	void SetColliderRadius(float radius) { mColliderRadius = radius; }

protected:
	Tag_cow mntag_cow;                  // 個別仕様分岐のためのタグ
	float mColliderRadius = 50.0f;      // 衝突判定用の半径制約

	EffekseerEffect* mpCowVm;           // 吸引演出用エフェクト
	int mEffectTimer;                   // エフェクト再生完了までの待機フレーム
	bool TutrialVacumFlag;              // チュートリアル用の特別な判定フラグ
};



