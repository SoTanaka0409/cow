#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"

class SphereCollider;
class CapsuleCollider;

// ステージ上を自律移動する一般動物（AI搭載アクター）の基底クラス
class AnimalMove : public Object3D
{
public:
	// 動物の行動状態の定義
	enum AnimalState
	{
		STATE_IDLE,    // 立ち止まり状態
		STATE_WALK,    // 徘徊移動状態
		STATE_VACUUM   // プレイヤーに吸引されている状態
	};

	// 動物の種類識別用タグ
	enum Tag_animal
	{
		none,
		Animal_1,
		Animal_2,
		Animal_3,
		Animal_gold,
		Animal_T,
	};

public:
	/*
	 * @brief 動物キャラクターオブジェクトを初期化し、初期座標にモデルを配置する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] なし
	 */
	AnimalMove(std::string filename, VECTOR initPos);
	virtual ~AnimalMove();

	void Update() override;
	void Draw() override;

	/*
	 * @brief AIに基づいて動物の移動処理を行う（吸引中・徘徊・壁衝突補正など）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標(mvPosition)の更新
	 */
	virtual void MoveAnimal();

	/*
	 * @brief 動物の衝突コライダーの座標情報をアクター座標に合わせて更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コライダー座標の同期
	 */
	virtual void ColliderMove();

	/*
	 * @brief 移動ベクトルに合わせて滑らかにモデルを回転させる
	 * [入力] なし
	 * [出力] なし
	 * [副作用] モデル角度の更新
	 */
	virtual void RotationAnimal();

	/*
	 * @brief 吸引時に動物自身をグルグルと自転させる演出処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 回転角度(mvRotation.y)の加算更新
	 */
	virtual void AnimalRotate();

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	void SetScale(float scale);

	AnimalState GetCurrentState() { return mCurrentState; }
	void ChangeStateToVacuum() { mCurrentState = STATE_VACUUM; }
	void SetCurrentState(AnimalState state) { mCurrentState = state; }
	void IncreaseVacuumTimer() { mVacuumTimer++; }
	void ResetVacuumTimer() { mVacuumTimer = 0; }
	int  GetVacuumTimer() { return mVacuumTimer; }

	bool IsDead() { return mIsDead; }
	void SetDeadFlag(bool flag) { mIsDead = flag; }

	void SetTag_animal(Tag_animal tag) { mntag_animal = tag; }
	Tag_animal GetTag_animal() { return mntag_animal; }
	bool GetBaitFlag() const { return mbBaitFlag; }

protected:
	Model* mpModel;                     // 3Dグラフィックモデル制御インスタンスへのポインタ
	AnimalState mCurrentState;          // 現在のAI状態
	int mActionTimer;                   // 次の意思決定（静止/徘徊）までの残りフレーム数
	float mfSpeed;                      // 徘徊時の移動速度
	float mfTargetAngle;                // 回転目標角度
	float mfAngle;                      // 現在の表示用回転角度

	const float ROTATE_SPEED = 0.2f;    // 旋回時の滑らかさ補正値
	VECTOR UpMoveVector;                // 上方向上昇ベクトル
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f); // 徘徊時の進行ベクトル
	VECTOR oldmoveVec;                  // 前フレームの進行ベクトル
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f); // 地形交差座標バッファ
	Tag_animal mntag_animal;            // タグ

	float mfHorizontalAngle;            // カメラ水平角（未使用）
	float mfVerticalAngle;              // カメラ垂直角（未使用）
	int mnActionTimer;                  // 行動タイマーカウンタ（未使用）

	int mVacuumTimer;                   // 吸引され始めてからの経過時間カウンタ
	bool mIsDead;                       // 死亡（回収完了）したかどうかのフラグ
	bool mDelete;                       // 削除完了フラグ

	float mfdeathTime;                  // 回収とみなす限界高度（Y座標値）
	float mfScore;                      // 回収された時の減点スコア（一般動物はマイナス点）
	float mfXp;                         // 回収された時に獲得できるXP（通常は0）
	bool mbBaitFlag;                    // 餌に惹かれているかどうかのフラグ
};
