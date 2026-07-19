#pragma once
#include "Collider.h"

// 線分ベースの衝突判定を利用するため
class CapsuleCollider : public Collider
{
public:
	/*
	 * オブジェクトにカプセル形状の判定を付与するため
	 * [入力] parent: 親オブジェクト, pos1: 始点, pos2: 終点, radius: 半径
	 * [出力] なし
	 * [副作用] 座標と半径を初期化する
	 */
	CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius);

	/*
	 * リソース破棄を保証するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~CapsuleCollider();

	/*
	 * 衝突範囲のデバッグ可視化を行うため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	/*
	 * 衝突開始時の振る舞いを定義するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void OnEnter() override;

	/*
	 * 衝突中の継続的な振る舞いを定義するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void OnTrigger() override;

	/*
	 * 衝突終了時の振る舞いを定義するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void OnExit() override;
};
