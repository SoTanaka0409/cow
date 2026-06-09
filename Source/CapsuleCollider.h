#pragma once
#include "Collider.h"

// カプセル形状（線分＋半径）の当たり判定コライダークラス
class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius);
	virtual ~CapsuleCollider();

	/*
	 * @brief 交差対象コライダーの形状(カプセル型等)に応じた当たり判定計算を行い、交差イベントを呼び出す
	 * [入力] check: 衝突判定を行う相手コライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update(Collider* check) override;

	/*
	 * @brief （デバッグ表示用）カプセルの3Dワイヤーフレームを描画する（現在処理なし）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	void OnEnter() override;
	void OnTrigger() override;
	void OnExit() override;
};
