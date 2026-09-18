#pragma once
#include "Object3D.h"
#include "EffekseerEffect.h"

#include <vector>

class SphereCollider;
class CapsuleCollider;
class Player3D;

/// @brief ステージ上を自律走行してプレイヤーを妨害し、接触時にペナルティを与える環境トラップ（竜巻）クラス
class Tornado : public Object3D
{
public:
	/// @param pos 発生位置座標
	/// @brief EffekseerEffect インスタンスの動的確保
	Tornado(VECTOR pos);
	virtual ~Tornado() override;

	/// @brief 最も近いプレイヤーへの緩やかな追追尾計算、竜巻サイズのLerp（線形補間）接近更新、SE再生
	void Update() override;

	/// @brief 天球や他3Dモデルと干渉しないよう、DxLibとEffekseerの描画順を同期してエフェクトを描画
	void Draw() override;

	/// @param collider 自身のカプセル衝突判定
	/// @param check 相手の衝突判定
	/// @brief 接触したプレイヤーを上空のランダム座標へ強制ワープ、カメラシェイク演出の発動
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	/// @param isCrisis 危機的状況（後半戦やイベント等）フラグ
	/// @brief ゲーム後半の難易度上昇（クライシスモード）に伴い、竜巻の当たり判定と見た目を2倍に巨大化させるトリガー
	void SetCrisisMode(bool isCrisis) { is_crisis_ = isCrisis; }

private:
	VECTOR velocity_;       ///< ステージ内をランダムに巡回するための移動方向ベクトル
	int move_timer_;        ///< 移動方向を変更するまでの残フレーム数カウンター
	float speed_;           ///< プレイヤーがギリギリ走って逃げ切れる速度基準値
	EffekseerEffect* tatu_; ///< 3Dモデルの代わりに竜巻の見た目を表現するエフェクトポインタ

	bool is_crisis_;            ///< ゲーム後半戦（難易度上昇モード）へ移行したかどうかの管理フラグ
	float current_scale_ratio_; ///< 巨大化時の見た目の急変を防ぐための現在のエフェクト拡縮比率
	float current_radius_;      ///< 見た目の拡縮と当たり判定を同期させるための現在の判定半径

	int effect_timer_;         ///< 定期的なエフェクト再再生とSE暴発を防ぐインターバルタイマー
	int tornado_spawn_timer_; ///< 前回の竜巻消失から次に出現するまでの湧き待ち猶予カウンタ

	/// @brief ノックバック中のプレイヤー情報
	struct KnockbackInfo {
		Player3D* player; ///< 参照対象のオブジェクトを保持するポインタ
		VECTOR velocity;  ///< 移動や回転の計算に使用する値
	};
	std::vector<KnockbackInfo> knockbacks_; ///< 内部状態を管理する値

	/// @brief 成長弾などオブジェクトのサイズ変化に伴い、物理的な衝突範囲を即座に同期する。
	/// @details オブジェクトのスケール増減およびコライダー半径の再計算を行う。
	void UpdateScaleAndRadius();

	/// @brief プレイヤーの現在座標を常に追従させ、回避行動を強要するゲームプレイを補強する。
	/// @details プレイヤーへ向けた移動ベクトルの補間および速度の更新を行う。
	void UpdateHomingPlayer();

	/// @brief 壁などの地形を活用した反射攻撃を実現し、閉所での戦闘に緊張感を与える。
	/// @details 地形との衝突検知、および法線に基づく反射ベクトルの算出・適用を行う。
	void UpdateWallBounce();

	/// @brief オブジェクトの状態に応じた視覚・聴覚フィードバックを生成し、操作の手応えを提示する。
	/// @details パーティクルエフェクトの発生要求、および環境音・効果音の再生を行う。
	void UpdateEffectAndSound();

	/// @brief 被弾時の吹き飛びを計算し、攻撃の重み（Weight）とキャラクターの硬直状態を表現する。
	/// @details 速度ベクトルの強制加算、および摩擦による速度の減衰処理を行う。
	void UpdateKnockback();
};
