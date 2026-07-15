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

// プレイヤーの入力処理、移動、状態遷移、ステータス計算を一元管理するクラス
// 各種サブマネージャーを内包するため、ライフサイクルの管理（メモリリーク）に注意すること
class Player3D : public Object3D
{
private:
	bool mbVacuumFlag = true; // 吸い込みの有効状態

public:
	// ステータスID
	enum StatusID
	{
		Status_Hp,
		Status_AttackS,
		kStatusSpeed,
	};

	// [入力] なし [出力] 吸い込みフラグの真偽値 [副作用] なし
	// チュートリアル中など、特定イベント進行前に吸い込みが暴発して進行不能になるのを防ぐための状態参照
	bool GetVacuumFlag() const
	{
		return mbVacuumFlag;
	}

public:
	// [入力] filename: モデルパス, initPos: 初期座標 [出力] なし [副作用] 各種リソースの動的確保
	// 動的確保するポインタ群（サブシステムや判定）のメモリリークを防ぐため、破棄時のdeleteと対にすること
	Player3D(std::string filename, VECTOR initPos);

	// [入力] なし [出力] なし [副作用] 確保したポインタの解放
	// EffekseerやModelなどのリソース解放順序を間違えるとアクセス違反でクラッシュするため注意
	virtual ~Player3D();

	// [入力] なし [出力] なし [副作用] 3Dモデルの描画コール
	// Zバッファの仕様上、半透明オブジェクトより先に描画されるようマネージャ側で制御すること
	void Draw() override;
	void DrawShadowCaster() override;

	// [入力] なし [出力] なし [副作用] 各種状態の更新処理
	void Update() override;

	// [入力] なし [出力] なし [副作用] プレイヤー座標の更新
	// カメラの向きに依存するため、必ずカメラのUpdateより後に実行して進行方向のズレを防ぐこと
	void MoveEx();

	// [入力] なし [出力] なし [副作用] ゲージ消費と吸い込み状態の変更
	// 多重呼び出しによるゲージの異常消費を防ぐため、アクション制限状態（スタン等）では必ずガードをかけること
	void Play();

	// [入力] なし [出力] なし [副作用] コライダーの座標同期
	// モデル移動の後に呼ばないと、当たり判定が1フレーム取り残されて壁抜けバグの原因になるため順序厳守
	void ColliderUpdate();

	// [入力] なし [出力] なし [副作用] 座標のクランプ処理
	// 敵との押し出し判定等で画面外（奈落）へ落ちて進行不能になるバグを防ぐ最終防波堤
	void ScreenOutCheck();

	// [入力] なし [出力] なし [副作用] モデル回転角の更新
	// ジンバルロックや逆方向への急旋回時のカクつきを防ぐため、現在角から目標角へLerp補間して向かせること
	void RotationByMove();

	
	// [入力] なし [出力] なし [副作用] 内部保持する各サブマネージャーの更新
	// プレイヤーのライフサイクルと紐づく機能（Skill等）の更新を一元化し、外部マネージャからの呼び出し忘れを防ぐ
	void ManagerUpdate();

	// [入力] なし [出力] なし [副作用] HUD（UI）の描画
	// 3D空間描画の完了後に呼ばないとZテストでUIが裏に隠れるため、2D描画フェーズで呼び出すこと
	void bar();

	// [入力] なし [出力] なし [副作用] デバッグ用テキスト等の描画
	// リリースビルドでの意図せぬ情報露出を防ぐため、本番環境ではコンパイルスイッチ等で確実に除外すること
	void test();

	// [入力] id: 対象のステータスID [出力] 最終計算値 [副作用] なし
	// スキルバフやレベル補正が乗算されるため、計算漏れバグを防ぐべく元ステータスは直接参照せず必ず本関数を通すこと
	float Status(StatusID id);

	// [入力] f: 設定する速度 [出力] なし [副作用] 攻撃速度の更新
	void SetStatusAttack(float f) { mfAttack_Speed = f; }

	// [入力] なし [出力] 現在の攻撃速度 [副作用] なし
	float GetStatusAttack() { return mfAttack_Speed; }

	// [入力] state: アニメーション状態, filename: モデルパス [出力] なし [副作用] アニメーションのロードと追加
	// 事前ロード機構を持たないため、ゲーム中の動的追加はスパイク（処理落ち）を招く点に留意して使用すること
	void AddAnimation(AnimationState state, std::string filename);

	// [入力] collider: 自身のコライダー, check: 相手のコライダー [出力] なし [副作用] 衝突イベントの発火
	// 多段ヒットによる即死バグを防ぐため、ダメージ処理を行う場合は必ず無敵時間（インビンシブル）のフラグガードを噛ませること
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	// [入力] なし [出力] カプセルコライダーのポインタ [副作用] なし
	CapsuleCollider* GetCollisionCollider() { return capsule_collider_; }

	// [入力] scale: 倍率 [出力] なし [副作用] モデルスケールと判定半径の変更
	// 当たり判定と見た目の乖離を防ぐため、必ずコライダーの半径も同期して変更する実装にすること
	void SetScale(float scale);

	// [入力] stunTime: スタン時間(フレーム) [出力] なし [副作用] スタン状態のフラグ設定
	// 行動不能時に吸い込みや移動が暴発しないよう、Update内の各アクション処理でこのフラグを監視すること
	void ApplyStun(int stunTime);

	// [入力] なし [出力] なし [副作用] エフェクトの生成と再生
	// 毎フレーム呼ばれるとVRAM枯渇と処理落ちを引き起こすため、スキル発動の瞬間（エッジ）のみ発火させること
	void PlaySkillEffect();

public:
	Model* model_;               // プレイヤーの3Dモデル
	Skill* mpSkill;              // スキル管理
	Level* mpLevel;              // レベル・経験値管理
	Combo* combo_;               // コンボ管理
	Score* mpScore;              // スコア管理

private:
	float mfSpeed;                       // ベース移動速度
	float mfHp;                          // 現在体力
	float mfAttack_Speed;                // ベース攻撃速度
	float target_angle_;                 // 旋回目標の角度
	float angle_;                        // 現在の旋回角度
	const float kRotateSpeed = 0.2f;     // 旋回時の補間係数
	const float JUMP_POWER = 30.0f;      // ジャンプ力
	bool mIsOutOfBounds = false;         // 画面外判定フラグ
	int mnLighGraph;                     // ライティング用画像ハンドル
	int mnGaugeFrameGraph;               // ゲージ枠の画像ハンドル
	bool mIsCowInVacuumRange;            // 吸引対象が範囲内にいるかのフラグ
	const float VACUUM_RADIUS = 300.0f;  // 吸引有効半径
	const int VACUUM_REQUIRE_TIME = 120; // 吸引完了に必要なフレーム数
	float mVacuumGauge = 100.0f;         // 現在の吸引ゲージ残量
	const float VACUUM_GAUGE_MAX = 100.0f; // 吸引ゲージの最大値
	const float VACUUM_COST_PER_FRAME = 0.2f; // 吸引中の毎フレーム消費量
	const float VACUUM_RECOVER_PER_FRAME = 0.4f; // 非吸引時の毎フレーム回復量
	bool mIsVacuumActive = false;        // 吸引アクション実行中フラグ
	int CatchNowCount;                   // 現在の同時捕獲数
	float currentSpeed;                  // 最終計算された実際の移動速度
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f); // 最終的な移動ベクトル
	VECTOR oldmoveVec;                   // 前フレームの移動ベクトル
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);  // 当たり判定の衝突検知座標
	float horizontal_angle_;             // カメラ基準の水平回転角
	float vertical_angle_;               // カメラ基準の垂直回転角
	const int SIZE_RAND_MAX = 800;       // エフェクト等のランダムサイズ最大値
	const int SIZE_RAND_MIN = 400;       // エフェクト等のランダムサイズ最小値
	const int VISIBLE_TIME_RAND_MAX = 30; // エフェクト等のランダム表示時間最大値
	const int VISIBLE_TIME_RAND_MIN = 5;  // エフェクト等のランダム表示時間最小値
	EffekseerEffect* mpSpeed;            // スピードバフ用エフェクト
	EffekseerEffect* mpBeam;             // ビーム攻撃用エフェクト
	int effect_timer_;                   // エフェクトの再生時間管理タイマー
	bool mIsStunned;                     // スタン(行動不能)状態フラグ
	int mStunTimer;                      // スタン解除までの残りフレーム数

};
