#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

/// @brief プレイヤーの移動エリアを制限し、ランダムな位置への落雷によってステージに緊張感を与える環境ギミッククラス
class Thunder : public Object3D
{
public:
	enum State
	{
		kIdle,
		kWarning,
		kStrike,
		kEnd
	};

public:
	/// @param pos 発生中心座標
	/// @brief 落雷・予兆・スタン用の各EffekseerEffectインスタンスの動的確保
	Thunder(VECTOR pos);
	virtual ~Thunder() override;

	/// @brief 落雷ステートマシンの更新、ランダムな落雷座標の再計算、SEおよびエフェクトの再生
	void Update() override;

	void Draw() override;

	/// @brief 値を取得する
	/// @return オブジェクトの稼働状態（生存フラグ）
	bool IsActive() const;

	/// @brief 一時対応：現在は物理エンジンのOnEnterによる自動判定に移行したため、旧来の球判定ロジックとして未使用保持
	/// @param playerPos 判定対象座標
	/// @param range 判定半径
	/// @return 衝突しているかどうかの真偽値
	bool CheckHit(VECTOR playerPos, float range);

	/// @param collider 自身の衝突判定
	/// @param check 相手の衝突判定
	/// @brief プレイヤーへのスタン状態（120f）適用、ヒットエフェクトの追従再生、カメラシェイクの発動
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	void UpdateEffects();
	void UpdateCollider();
	void UpdateState();
	void UpdateStunEffect();

	VECTOR pos_;         ///< 座標や位置情報を管理する値
	int warning_timer_;  ///< 時間経過や処理間隔を管理するカウンター
	int strike_timer_;   ///< 時間経過や処理間隔を管理するカウンター
	State state_;        ///< 現在の状態や種別を管理する値
	int interval_timer_; ///< 時間経過や処理間隔を管理するカウンター
	bool active_;        ///< 状態の有効・無効を管理するフラグ

public:
	EffekseerEffect* thunder_; ///< 内部状態を管理する値
	EffekseerEffect* warning_; ///< 内部状態を管理する値
	EffekseerEffect* stun_;    ///< 内部状態を管理する値

	/// @brief 1回の落雷フレーム中に多段ヒットして、プレイヤーのスタン受付時間が意図せず延長されるのを防ぐためのフラグ
	bool has_stunned_;      ///< 状態の有効・無効を管理するフラグ
	int stun_effect_timer_; ///< 時間経過や処理間隔を管理するカウンター
};
