#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

// ステージ上にランダムに落雷を発生させ、プレイヤーに一時的なスタンを与える環境トラップクラス
class Thunder : public Object3D
{
public:
	// 落雷のライフサイクル状態
	enum State
	{
		IDLE,      // 落雷が発生していない待機時間状態
		WARNING,   // 地面に警告エフェクトを表示している予兆状態
		STRIKE,    // 実際に落雷エフェクトと判定を発生させている落雷状態
		END        // 終了（未使用）
	};

public:
	/*
	 * @brief 落雷トラップを初期位置で生成し、各種エフェクトのインスタンスを作成する
	 * [入力] pos: 生成座標（初期状態は非アクティブ）
	 * [出力] なし
	 * [副作用] 各種EffekseerEffectインスタンスのnew
	 */
	Thunder(VECTOR pos);

	/*
	 * @brief ロードしたエフェクト資源を安全に解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] エフェクト資源の破棄
	 */
	virtual ~Thunder() override;

	/*
	 * @brief 状態マシンの更新、座標移動、プレイヤーの衝突検知、およびスタン用エフェクトの進行更新を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 落雷座標のランダム更新、状態変更、エフェクト再生
	 */
	void Update() override;

	/*
	 * @brief 落雷コライダーの可視化デバッグ描画（現在はコメントアウト）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	/*
	 * @brief 落雷の稼働状態を取得する
	 * [入力] なし
	 * [出力] 稼働中ならtrue
	 * [副作用] なし
	 */
	bool IsActive() const;

	/*
	 * @brief 指定位置に対する当たり判定を行う（現在は未使用、OnEnterで処理）
	 * [入力] playerPos: プレイヤー座標, range: 判定半径
	 * [出力] ヒットした場合はtrue
	 * [副作用] なし
	 */
	bool CheckHit(VECTOR playerPos, float range);

	/*
	 * @brief 落雷判定が発生している瞬間にプレイヤーと接触した場合、プレイヤーを一定時間スタンさせる
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] プレイヤーのスタン適用、スタンエフェクトの再生、カメラシェイク
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	VECTOR mPos;                // 落雷のターゲット（地面）中心座標
	int mWarningTimer;          // 警告エフェクトの表示残りフレーム
	int mStrikeTimer;           // 落雷エフェクトと判定の残り継続フレーム
	State mState;               // 現在の落雷状態
	int mIntervalTimer;         // 次の落雷発生までの待機残りフレーム
	bool mActive;               // アクティブフラグ

public:
	EffekseerEffect* mpThunder; // 落雷の瞬間エフェクト
	EffekseerEffect* mpWarning; // 地面の警告サークルエフェクト
	EffekseerEffect* mpStun;    // プレイヤーの頭上で回転するスタンエフェクト

	bool mHasStunned;           // 同一落雷フェーズ内でプレイヤーをスタンさせたかどうかのフラグ
	int mStunEffectTimer;       // スタンスターエフェクトの再生残り時間タイマー
};
