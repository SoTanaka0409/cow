#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

// ステージ上をAI自律移動する牛キャラクターの基底クラス
class CowMove : public Object3D
{
public:
	// 牛の行動状態の定義
	enum CowState
	{
		STATE_IDLE,    // 立ち止まり状態
		STATE_WALK,    // 徘徊移動状態
		STATE_VACUUM   // プレイヤーに吸引されている状態
	};

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

	enum DeathReason
	{
		DEATH_VACUUM, // 吸い込まれて回収
		DEATH_BAIT,   // エサで回収
		DEATH_LIMIT   // 生成上限による消去
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
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標(mvPosition)の更新
	 */
	virtual void MoveCow();

	/*
	 * @brief 衝突コライダー座標を現在の位置に合わせて更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void ColliderMove();

	/*
	 * @brief 他の牛と重ならないように、近接時に押し返す力を適用する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 重複時の座標修正
	 */
	void AvoidOtherCows();

	/*
	 * @brief 周囲に餌(Bait)があれば、その餌に向けて引き寄せられる移動を行う
	 * [入力] なし
	 * [出力] true（餌へ移動したため標準移動をスキップ）, false（餌がない）
	 * [副作用] 座標の更新
	 */
	bool SeekBait();

	/*
	 * @brief 徘徊AIタイマーを減らし、立ち止まりと歩き状態を定期的に選択する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] AI状態および進行ベクトルの更新
	 */
	void UpdateWanderAI();

	/*
	 * @brief ステージ境界や壁面との衝突を判定し、壁沿いに滑り移動するように座標を補正する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 衝突時の滑り座標補正
	 */
	void CheckWallCollision();

	/*
	 * @brief 歩き移動時に、進行方向を向くように滑らかにモデルを回転させる
	 * [入力] なし
	 * [出力] なし
	 * [副作用] モデル角度の更新
	 */
	virtual void RotationCow();

	/*
	 * @brief 吸引時に、牛が空中で回転しながら吸い込まれていく演出処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 回転角度(mvRotation.y)の加算更新
	 */
	virtual void CowRotate();

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	void SetScale(float scale);

	/*
	 * @brief 吸引され限界高度に達した際、スコア・経験値をプレイヤーに加算して回収（消滅）する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 削除フラグ(DeleteFlag)の有効化、XP・スコア加算、SE再生
	 */
	void CowDied();

	CowState GetCurrentState() { return mCurrentState; }
	void ChangeStateToVacuum() { mCurrentState = STATE_VACUUM; }
	void SetCurrentState(CowState state) { mCurrentState = state; }
	void IncreaseVacuumTimer() { mVacuumTimer++; }
	void ResetVacuumTimer() { mVacuumTimer = 0; }
	int GetVacuumTimer() { return mVacuumTimer; }

	/*
	 * @brief 餌の効果によってアクターに倒された時の回収（消滅）処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 削除フラグの有効化、XP・スコア加算、SE再生
	 */
	virtual void KilledByBait();

	/*
	 * @brief 牛の死亡処理を統合
	 */
	virtual void Die(DeathReason reason);

	bool IsDead() { return mIsDead; }

	void SetTag_cow(Tag_cow tag) { mntag_cow = tag; }
	Tag_cow GetTag_cow() { return mntag_cow; }

	void SetColliderRadius(float radius) { mColliderRadius = radius; }
	bool GetCowDelete() { return mCowtDelete; }
	bool IsJudged() const { return mbIsJudged; }
	void SetJudged(bool flag) { mbIsJudged = flag; }
	bool GetBaitFlag() const { return mbBaitFlag; }

protected:
	Model* mpModel;                     // 3Dモデルクラスポインタ
	CowState mCurrentState;              // 現在のAI行動状態
	int mActionTimer;                   // 行動変更タイマー残り時間
	float mfSpeed;                      // 移動速度
	float mfTargetAngle;                // 目標回転角
	float mfAngle;                      // 現在の回転角

	const float ROTATE_SPEED = 0.2f;    // 回転滑らかさ係数
	VECTOR UpMoveVector;                // 上昇用ベクトル（未使用）
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f); // 徘徊方向ベクトル
	VECTOR oldmoveVec;                  // 前フレームの進行ベクトル
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f); // 壁衝突一時座標
	Tag_cow mntag_cow;                  // 牛のタグ種類

	float mfHorizontalAngle;            // カメラ水平旋回（未使用）
	float mfVerticalAngle;              // カメラ垂直旋回（未使用）
	int mnActionTimer;                  // アクションタイマー（未使用）

	int mVacuumTimer;                   // 吸引され始めてからのフレームカウント
	bool mIsDead;                       // 死亡（回収可能状態）したかどうかのフラグ
	float mColliderRadius = 50.0f;      // コライダーの判定半径
	float mfdeathTime;                  // 回収となる吸引限界高度Y座標
	float mfScore;                      // 回収時に獲得できるスコア
	float mfXp;                         // 回収時に獲得できる経験値

	bool mbBaitFlag;                    // 餌に引き寄せられているかのフラグ
	bool mCowtDelete;                   // 吸引消滅エフェクト終了後の完全削除フラグ
	bool mbIsJudged = false;            // スコアコンボ判定チェック済みフラグ
	EffekseerEffect* mpCowVm;           // 吸引時に発生するエフェクト
	bool mbIsVisible;                   // 牛のモデル描画フラグ（吸引消滅時は非表示）
	int mEffectTimer;                   // 吸引消滅エフェクトの再生残り時間タイマー
	bool TutrialVacumFlag;              // チュートリアル判定フラグ
};



