#pragma once
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

// ステージ上を自律移動し、プレイヤーを巻き込んでランダム位置へワープさせる竜巻オブジェクトクラス
class Tatumaki : public Object3D
{
public:
	/*
	 * @brief 竜巻を生成し、コライダーの初期化とエフェクトのロードを行う
	 * [入力] pos: 生成時の初期座標
	 * [出力] なし
	 * [副作用] EffekseerEffectのインスタンス確保
	 */
	Tatumaki(VECTOR pos);

	/*
	 * @brief エフェクト等の動的メモリ資源を解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] エフェクトオブジェクトの破棄
	 */
	virtual ~Tatumaki() override;

	/*
	 * @brief プレイヤー方向への追尾移動、巨大化Lerp計算、コライダー位置の追従を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コライダーおよびエフェクトの座標更新、一定間隔でのSE再生
	 */
	void Update() override;

	/*
	 * @brief ワイヤーフレームなどのコライダー補助描画を行う（現在は非アクティブ）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	/*
	 * @brief プレイヤーとの衝突時に、プレイヤーをステージ上のランダム位置に吹き飛ばしワープさせる
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] プレイヤー座標の書き換え、カメラシェイクの発生
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * @brief 竜巻のクライシスモード（後半戦などの巨大化状態）を切り替える
	 * [入力] isCrisis: 巨大化する場合はtrue
	 * [出力] なし
	 * [副作用] なし
	 */
	void SetCrisisMode(bool isCrisis) { mIsCrisis = isCrisis; }

private:
	VECTOR mPos;              // 竜巻の現在座標
	VECTOR mVelocity;         // 移動方向の単位ベクトル
	int mMoveTimer;           // 方向転換用のタイマー（未使用）
	float mSpeed;             // 移動速度
	EffekseerEffect* Tatu;    // 竜巻表現用の3Dエフェクト

	bool mIsCrisis;           // 巨大化状態であるかどうかのフラグ
	float mCurrentScaleRatio; // 巨大化演出用のLerpスケール補間率
	float mCurrentRadius;     // コライダー半径のLerp補間用現在値

	int mEffectTimer;         // 次回のエフェクト・SE再生までのタイマー
	int mTatumakiSpawnTimer;  // スポーン制御用のタイマー（未使用）
};
