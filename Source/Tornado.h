#pragma once
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

// 業務ルール：ステージ上を自律走行してプレイヤーを妨害し、接触時にペナルティを与える環境トラップ（竜巻）クラス
class Tornado : public Object3D
{
public:
	// 入力：pos=発生位置座標
	// 副作用：EffekseerEffect インスタンスの動的確保
	Tornado(VECTOR pos);
	virtual ~Tornado() override;

	// 副作用：最も近いプレイヤーへの緩やかな追追尾計算、竜巻サイズのLerp（線形補間）接近更新、SE再生
	void Update() override;

	// 副作用：天球や他3Dモデルと干渉しないよう、DxLibとEffekseerの描画順を同期してエフェクトを描画
	void Draw() override;

	// 入力：collider=自身のカプセル衝突判定, check=相手の衝突判定
	// 副作用：接触したプレイヤーを上空のランダム座標へ強制ワープ、カメラシェイク演出の発動
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	// 入力：isCrisis=危機的状況（後半戦やイベント等）フラグ
	// 業務ルール：ゲーム後半の難易度上昇（クライシスモード）に伴い、竜巻の当たり判定と見た目を2倍に巨大化させるトリガー
	void SetCrisisMode(bool isCrisis) { is_crisis_ = isCrisis; }

private:
	VECTOR pos_;                     // 竜巻の中心位置座標
	VECTOR velocity_;                 // ステージ内をランダムに巡回するための移動方向ベクトル
	int move_timer_;                 // 移動方向を変更するまでの残フレーム数カウンター
	float speed_;                    // プレイヤーがギリギリ走って逃げ切れる速度基準値
	EffekseerEffect* tatu_;          // 3Dモデルの代わりに竜巻の見た目を表現するエフェクトポインタ

	bool is_crisis_;                 // ゲーム後半戦（難易度上昇モード）へ移行したかどうかの管理フラグ
	float current_scale_ratio_;      // 巨大化時の見た目の急変を防ぐための現在のエフェクト拡縮比率
	float current_radius_;           // 見た目の拡縮と当たり判定を同期させるための現在の判定半径

	int effect_timer_;               // 定期的なエフェクト再再生とSE暴発を防ぐインターバルタイマー
	int tatumaki_spawn_timer_; // 前回の竜巻消失から次に出現するまでの湧き待ち猶予カウンタ
};