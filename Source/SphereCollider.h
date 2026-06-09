#pragma once
#include "Collider.h"

// 球体形状（中心点と半径）の当たり判定コライダークラス
class SphereCollider : public Collider
{
public:
	/*
	 * @brief 球体コライダーの初期設定を行う
	 * [入力] parent: 親オブジェクト, center: 中心座標, radius: 半径
	 * [出力] なし
	 * [副作用] なし
	 */
	SphereCollider(Object3D* parent, VECTOR center, float radius);
	virtual ~SphereCollider();

	/*
	 * @brief 交差対象コライダーの形状(球・カプセル)に応じた当たり判定計算を行い、交差イベントを呼び出す
	 * [入力] check: 衝突判定を行う相手コライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update(Collider* check) override;

	/*
	 * @brief 球の3Dワイヤーフレームを描画する（デバッグ表示用）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	void OnEnter() override;
	void OnTrigger() override;
	void OnExit() override;

};
