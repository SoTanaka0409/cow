#pragma once
#include "Collider.h"

/// @brief 線分ベースの衝突判定を利用するため
class CapsuleCollider : public Collider
{
public:
	/// @brief オブジェクトにカプセル形状の判定を付与するため
	/// @param parent 親オブジェクト
	/// @param pos1 始点
	/// @param pos2 終点
	/// @param radius 半径
	/// @details 座標と半径を初期化する
	CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius);

	/// @brief リソース破棄を保証するため
	virtual ~CapsuleCollider();

	/// @brief 衝突範囲のデバッグ可視化を行うため
	void Draw() override;

	/// @brief 衝突開始時の振る舞いを定義するため
	void OnEnter() override;

	/// @brief 衝突中の継続的な振る舞いを定義するため
	void OnTrigger() override;

	/// @brief 衝突終了時の振る舞いを定義するため
	void OnExit() override;
};
