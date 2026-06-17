#pragma once
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

class Tatumaki : public Object3D
{
public:
	/*
	 * @brief 竜巻オブジェクトの初期化
	 * [入力] pos: 竜巻の初期スポーン座標
	 * [出力] なし
	 * [副作用] 3Dエフェクト(EffekseerEffect)のリソースをメモリにロードし、初期スケールを設定する
	 */
	Tatumaki(VECTOR pos);

	/*
	 * @brief リソースの解放
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 動的確保したEffekseerEffectの破棄によるメモリ解放を行う
	 */
	virtual ~Tatumaki() override;

	/*
	 * @brief フレーム毎の追従・演出更新
	 * [入力] なし
	 * [出力] なし
	 * [副作用] プレイヤーへのホーミング移動、段階的なサイズ変更(Lerp)、及び一定間隔でのSE再生を行う
	 */
	void Update() override;

	/*
	 * @brief 描画処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 本クラス自体の描画は行わない（描画はEffekseer側で処理されるため）
	 */
	void Draw() override;

	/*
	 * @brief 当たり判定への侵入検知
	 * [入力] collider: 自身のコライダー, check: 接触した相手のコライダー
	 * [出力] なし
	 * [副作用] プレイヤー接触時、仕様に基づきプレイヤー座標を強制的にランダムワープさせ、カメラを振動させる
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;
	
	/*
	 * @brief 当たり判定内の滞在検知
	 * [入力] collider: 自身のコライダー, check: 接触した相手のコライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	
	/*
	 * @brief 当たり判定からの退出検知
	 * [入力] collider: 自身のコライダー, check: 接触した相手のコライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * @brief 竜巻の巨大化モード設定
	 * [入力] isCrisis: 巨大化状態にする場合はtrue
	 * [出力] なし
	 * [副作用] 状態フラグの更新。次回のUpdate時にスケールと当たり判定のサイズが拡張される
	 */
	void SetCrisisMode(bool isCrisis) { mIsCrisis = isCrisis; }

private:
	VECTOR mPos;
	VECTOR mVelocity;
	int mMoveTimer;
	float mSpeed;
	EffekseerEffect* Tatu;

	bool mIsCrisis;
	float mCurrentScaleRatio;
	float mCurrentRadius;

	int mEffectTimer;
	int mTatumakiSpawnTimer;
};
