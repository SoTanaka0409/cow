#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"

/// @brief カプセル状の衝突判定領域を初期化するため
/// @param parent 親オブジェクト
/// @param pos1 始点
/// @param pos2 終点
/// @param radius 半径
/// @details メンバ変数(position_, position2_, radius_)を初期化する
CapsuleCollider::CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius)
	: Collider(parent)
{
	position_ = pos1;
	position2_ = pos2;
	radius_ = radius;
}

/// @brief リソース解放のため
CapsuleCollider::~CapsuleCollider()
{
}

/// @brief デバッグ用の形状を描画するため
/// @details 画面に描画を行う
void CapsuleCollider::Draw()
{
	DrawCapsule3D(
		position_,
		position2_,
		radius_,
		8,
		GetColor(255, 0, 0),
		GetColor(255, 0, 0),
		false
	);
}

/// @brief 衝突開始時の振る舞いを定義するため
void CapsuleCollider::OnEnter()
{
}

/// @brief 衝突中の振る舞いを定義するため
void CapsuleCollider::OnTrigger()
{
}

/// @brief 衝突終了時の振る舞いを定義するため
void CapsuleCollider::OnExit()
{
}
