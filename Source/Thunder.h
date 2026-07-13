#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

// 業務ルール：プレイヤーの移動エリアを制限し、ランダムな位置への落雷によってステージに緊張感を与える環境ギミッククラス
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
	// 入力：pos=発生中心座標
	// 副作用：落雷・予兆・スタン用の各EffekseerEffectインスタンスの動的確保
	Thunder(VECTOR pos);
	virtual ~Thunder() override;

	// 副作用：落雷ステートマシンの更新、ランダムな落雷座標の再計算、SEおよびエフェクトの再生
	void Update() override;

	void Draw() override;

	// 出力：オブジェクトの稼働状態（生存フラグ）
	bool IsActive() const;

	// 入力：playerPos=判定対象座標, range=判定半径
	// 出力：衝突しているかどうかの真偽値
	// 一時対応：現在は物理エンジンのOnEnterによる自動判定に移行したため、旧来の球判定ロジックとして未使用保持
	bool CheckHit(VECTOR playerPos, float range);

	// 入力：collider=自身の衝突判定, check=相手の衝突判定
	// 副作用：プレイヤーへのスタン状態（120f）適用、ヒットエフェクトの追従再生、カメラシェイクの発動
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	VECTOR pos_;
	int warning_timer_;
	int strike_timer_;
	State state_;
	int interval_timer_;
	bool active_;

public:
	EffekseerEffect* thunder_;
	EffekseerEffect* warning_;
	EffekseerEffect* stun_;

	// バグ回避：1回の落雷フレーム中に多段ヒットして、プレイヤーのスタン受付時間が意図せず延長されるのを防ぐためのフラグ
	bool has_stunned_;
	int stun_effect_timer_;
};