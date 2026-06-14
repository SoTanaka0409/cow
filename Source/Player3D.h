#pragma once
#include "DxLib.h"
#include <string>
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "Skill.h"
#include "Level.h"
#include "camera.h"
#include "Combo.h"
#include "Score.h"
#include "EffekseerEffect.h"

class CapsuleCollider;

// 3D空間上のプレイヤーキャラクターを制御するクラス
class Player3D : public Object3D
{
private:
	bool mbVacuumFlag = true;       // 吸引ビームを使用可能かどうかのフラグ
public:
	// プレイヤーの強化ステータスID定義
	enum StatusID
	{
		Status_Hp,                  // HPステータス
		Status_AttackS,             // 攻撃（吸引）速度ステータス
		Status_Speed,               // 移動速度ステータス
	};

	bool GetVacuumFlag() const
	{
		return mbVacuumFlag;
	}

public:
	/*
	 * @brief プレイヤーオブジェクトをロードし、初期ステータスとサブシステムを初期化する
	 * [入力] filename: モデルのファイルパス, initPos: 初期スポーン座標
	 * [出力] なし
	 * [副作用] 各マネージャー（Score, Level, Skill, Combo）やコライダーのロード
	 */
	Player3D(std::string filename, VECTOR initPos);
	virtual ~Player3D();

	void Draw() override;
	void Update() override;

	/*
	 * @brief 入力情報を元にプレイヤーの移動およびカメラ方向基準の移動ベクトルを計算する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標(mvPosition)の更新
	 */
	void MoveEx();

	/*
	 * @brief マウス入力などをチェックして、吸引中状態のフラグやゲージ消費量を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mIsVacuumActiveフラグおよびmVacuumGauge値の更新
	 */
	void Play();

	/*
	 * @brief 吸引中かどうかに応じてプレイヤーのコライダーのサイズと位置を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mpCapsuleColliderの位置、半径、およびビームエフェクトの更新
	 */
	void ColliderUpdate();

	/*
	 * @brief プレイヤーがステージ外に出てしまった場合の判定と復帰処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] キー入力による座標リセット
	 */
	void ScreenOutCheck();

	/*
	 * @brief 移動ベクトルの向きに合わせてプレイヤーモデルを滑らかに回転させる
	 * [入力] なし
	 * [出力] なし
	 * [副作用] モデル回転角度の更新
	 */
	void RotationByMove();

	/*
	 * @brief プレイヤーが保持するサブモジュール（レベル、スコア、モデルなど）の更新を描画と共に行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] サブモジュールのUpdate/Draw呼び出し
	 */
	void ManagerUpdate();

	/*
	 * @brief 画面上にプレイヤーの吸引ゲージUIなどを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void bar();

	/*
	 * @brief デバッグ用の入力処理（XP強制追加など）を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] デバッグ時のみレベル上昇などの状態変化
	 */
	void test();

	/*
	 * @brief ステータスIDに基づき、プレイヤーの現在の強化数値を計算して返す
	 * [入力] id: ステータスID
	 * [出力] 該当ステータスの最終的な数値
	 * [副作用] なし
	 */
	float Status(StatusID id);

	void SetStatusAttack(float f) { mfAttack_Speed = f; }
	float GetStatusAttack() { return mfAttack_Speed; }

	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	CapsuleCollider* GetCollisionCollider() { return mpCapsuleCollider; }

	void SetScale(float scale);

	/*
	 * @brief プレイヤーに気絶（スタン）状態を適用し、タイマーを開始する
	 * [入力] stunTime: 気絶時間（フレーム数）
	 * [出力] なし
	 * [副作用] mIsStunnedフラグを真にし、操作不能時間が発生する
	 */
	void ApplyStun(int stunTime);

	void PlaySkillEffect();

public:
	Model* mpModel;                 // 3Dモデル制御クラス
	Skill* mpSkill;                 // スキル制御クラス
	Level* mpLevel;                 // レベル・XP管理クラス
	Combo* mpCombo;                 // コンボ数制御クラス
	Score* mpScore;                 // スコア表示・加算クラス

private:
	float mfSpeed;                  // 基本移動速度
	float mfHp;                     // プレイヤーの体力
	float mfAttack_Speed;           // 吸い込みの吸引速度
	float mfTargetAngle;            // 移動方向による目標回転角度
	float mfAngle;                  // 現在のプレイヤーモデルの向き角度
	const float ROTATE_SPEED = 0.2f; // 滑らかなモデル回転用補間速度
	const float JUMP_POWER = 30.0f;  // ジャンプ時の初速（未使用）
	bool mIsOutOfBounds = false;    // 画面外に出てしまっているかのフラグ
	int mnLighGraph;                // 吸引ビーム用テクスチャハンドル

	bool mIsCowInVacuumRange;       // 吸引範囲内に牛が存在するかのフラグ
	const float VACUUM_RADIUS = 300.0f;  // 通常の吸引判定範囲の半径
	const int VACUUM_REQUIRE_TIME = 120; // 吸引開始までの待機フレーム

	float mVacuumGauge = 100.0f;    // 吸引ゲージの現在値
	const float VACUUM_GAUGE_MAX = 100.0f; // 吸引ゲージの最大値
	const float VACUUM_COST_PER_FRAME = 0.2f; // 1フレームあたりのゲージ消費量
	const float VACUUM_RECOVER_PER_FRAME = 0.4f; // 1フレームあたりのゲージ自然回復量
	bool mIsVacuumActive = false;   // 吸い込みビームを放射しているかのフラグ

	int CatchNowCount;              // 現在のセッションで吸引した牛の総数
	float currentSpeed;             // 現在の移動速度実値

	VECTOR UpMoveVector;            // 上方向移動用ベクトル
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f); // 移動方向入力ベクトル
	VECTOR oldmoveVec;              // 前フレームの移動ベクトル
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f); // ステージ交差座標の一時格納用

	float mfHorizontalAngle;        // カメラ水平旋回角度
	float mfVerticalAngle;          // カメラ垂直旋回角度

	const int SIZE_RAND_MAX = 800;  // エフェクトランダムサイズの最大
	const int SIZE_RAND_MIN = 400;  // エフェクトランダムサイズの最小
	const int VISIBLE_TIME_RAND_MAX = 30; // エフェクト表示フレーム最大
	const int VISIBLE_TIME_RAND_MIN = 5;  // エフェクト表示フレーム最小

	EffekseerEffect* mpSpeed;       // 移動速度上昇時のエフェクト
	EffekseerEffect* mpBeam;        // 吸い込みビーム用Effekseerエフェクト
	int mEffectTimer;               // エフェクト定期再生用カウンタ

	bool mIsStunned;                // スタン中かどうかのフラグ
	int mStunTimer;                 // スタンの残りフレーム時間
};

