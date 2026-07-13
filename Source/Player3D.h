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
class Player3D : public Object3D
{
private:
	// 吸い込み機能の有効状態を管理するため
	bool mbVacuumFlag = true;
public:
	// 各種ステータス計算を共通化するため
	enum StatusID
	{
		Status_Hp,
		Status_AttackS,
		kStatusSpeed,
	};
	/*
	 * システム連携のため吸い込み有効状態を外部へ公開する
	 * [入力] なし
	 * [出力] 吸い込みフラグの真偽値
	 * [副作用] なし
	 */
	bool GetVacuumFlag() const
	{
		return mbVacuumFlag;
	}
public:
	/*
	 * ゲーム開始時にプレイヤーと周辺システムを準備するため
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] 各種マネージャーやコライダーの初期化
	 */
	Player3D(std::string filename, VECTOR initPos);
	/*
	 * 終了時に不要なメモリを解放するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] メモリ解放
	 */
	virtual ~Player3D();
	/*
	 * 画面にプレイヤーを表示するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] グラフィック描画
	 */
	void Draw() override;
	/*
	 * 毎フレームの各種状態を最新に保つため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 状態更新
	 */
	void Update() override;
	/*
	 * ユーザー入力に応じて位置を移動させるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標の更新
	 */
	void MoveEx();
	/*
	 * プレイヤーによる吸い込みアクションを実現するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 吸い込みゲージとアクション状態の変更
	 */
	void Play();
	/*
	 * 移動後の位置に合わせて当たり判定を追従させるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コライダーの座標更新
	 */
	void ColliderUpdate();
	/*
	 * プレイヤーが画面外へ出ないように制限するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標の補正
	 */
	void ScreenOutCheck();
	/*
	 * 移動方向に合わせて自然にモデルを向けるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 回転角度の更新
	 */
	void RotationByMove();
	/*
	 * スキルやスコア等の関連システムを一括で更新するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] サブマネージャー群の更新
	 */
	void ManagerUpdate();
	/*
	 * プレイヤー周辺のUI情報を可視化するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] UI描画
	 */
	void bar();
	/*
	 * 開発中の動作確認を行うため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] デバッグ情報の描画
	 */
	void test();
	/*
	 * スキル補正等を加味した最終的なステータスを計算するため
	 * [入力] id: 対象のステータスID
	 * [出力] 計算後のステータス値
	 * [副作用] なし
	 */
	float Status(StatusID id);
	/*
	 * アイテム等で攻撃速度を変動させるため
	 * [入力] f: 設定する速度
	 * [出力] なし
	 * [副作用] 攻撃速度の更新
	 */
	void SetStatusAttack(float f) { mfAttack_Speed = f; }
	/*
	 * 他システムから攻撃速度を参照するため
	 * [入力] なし
	 * [出力] 現在の攻撃速度
	 * [副作用] なし
	 */
	float GetStatusAttack() { return mfAttack_Speed; }
	/*
	 * 状態に応じたモーションを準備するため
	 * [入力] state: アニメーション状態, filename: モデルパス
	 * [出力] なし
	 * [副作用] アニメーションの追加
	 */
	void AddAnimation(AnimationState state, std::string filename);
	/*
	 * 他の物体との衝突開始を検知するため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 衝突イベントの発火
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;
	/*
	 * 他の物体との接触継続を処理するため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 接触イベントの発火
	 */
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	/*
	 * 他の物体との離脱を検知するため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 離脱イベントの発火
	 */
	virtual void OnExit(Collider* collider, Collider* check) override;
	/*
	 * 当たり判定情報を外部から参照するため
	 * [入力] なし
	 * [出力] カプセルコライダーのポインタ
	 * [副作用] なし
	 */
	CapsuleCollider* GetCollisionCollider() { return capsule_collider_; }
	/*
	 * プレイヤーの描画サイズを変更するため
	 * [入力] scale: 倍率
	 * [出力] なし
	 * [副作用] モデルスケールの変更
	 */
	void SetScale(float scale);
	/*
	 * 敵の攻撃等による行動不能状態を付与するため
	 * [入力] stunTime: スタン時間(フレーム)
	 * [出力] なし
	 * [副作用] スタン状態のフラグ設定
	 */
	void ApplyStun(int stunTime);
	/*
	 * スキル発動時の視覚演出を行うため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] エフェクトの生成と再生
	 */
	void PlaySkillEffect();
public:
	Model* model_;
	Skill* mpSkill;
	Level* mpLevel;
	Combo* combo_;
	Score* mpScore;
private:
	float mfSpeed;
	float mfHp;
	float mfAttack_Speed;
	float target_angle_;
	float angle_;
	const float kRotateSpeed = 0.2f;
	const float JUMP_POWER = 30.0f;
	bool mIsOutOfBounds = false;
	int mnLighGraph;
	int mnGaugeFrameGraph;
	bool mIsCowInVacuumRange;
	const float VACUUM_RADIUS = 300.0f;
	const int VACUUM_REQUIRE_TIME = 120;
	float mVacuumGauge = 100.0f;
	const float VACUUM_GAUGE_MAX = 100.0f;
	const float VACUUM_COST_PER_FRAME = 0.2f;
	const float VACUUM_RECOVER_PER_FRAME = 0.4f;
	bool mIsVacuumActive = false;
	int CatchNowCount;
	float currentSpeed;
	VECTOR UpMoveVector;
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);
	VECTOR oldmoveVec;
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	float horizontal_angle_;
	float vertical_angle_;
	const int SIZE_RAND_MAX = 800;
	const int SIZE_RAND_MIN = 400;
	const int VISIBLE_TIME_RAND_MAX = 30;
	const int VISIBLE_TIME_RAND_MIN = 5;
	EffekseerEffect* mpSpeed;
	EffekseerEffect* mpBeam;
	int effect_timer_;
	bool mIsStunned;
	int mStunTimer;
};