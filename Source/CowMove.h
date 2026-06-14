#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "EffekseerEffect.h"
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// ステージ上をAI自律移動する牛キャラクターの基底クラス
class CowMove : public CharacterMove
{
public:
	// 牛の種類識別用タグ
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
	 * @brief 牛キャラクターをロードし、初期状態をセットする
	 * [入力] filename: モデルのファイルパス, initPos: 初期スポーン座標
	 * [出力] なし
	 * [副作用] モデルおよび吸引演出用Effekseerエフェクトのインスタンス生成
	 */
	CowMove(std::string filename, VECTOR initPos);
	virtual ~CowMove();

	void Update() override;
	void Draw() override;

	/*
	 * @brief AIによる自律徘徊移動や、吸引された際の上昇同期処理などを実行する
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
	 * @brief 吸引され限界高度に達した際、スコア・経験値をプレイヤーに加算して回収（消滅）する
	 */
	virtual void CharacterDied() override;

	/*
	 * @brief 餌の効果によってアクターに倒された時の回収（消滅）処理
	 */
	virtual void KilledByBait();

	/*
	 * @brief 牛の死亡処理を統合
	 */
	virtual void Die(DeathReason reason) override;

	void SetTag_cow(Tag_cow tag) { mntag_cow = tag; }
	Tag_cow GetTag_cow() { return mntag_cow; }

	void SetColliderRadius(float radius) { mColliderRadius = radius; }
	bool GetCowDelete() { return mCowtDelete; }

protected:
	Tag_cow mntag_cow;                  // 牛のタグ種類
	float mColliderRadius = 50.0f;      // コライダーの判定半径

	bool mCowtDelete;                   // 吸引消滅エフェクト終了後の完全削除フラグ
	EffekseerEffect* mpCowVm;           // 吸引時に発生するエフェクト
	int mEffectTimer;                   // 吸引消滅エフェクトの再生残り時間タイマー
	bool TutrialVacumFlag;              // チュートリアル判定フラグ
};



