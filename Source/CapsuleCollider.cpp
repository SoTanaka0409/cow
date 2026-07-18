#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Object3D.h"

/*
 * カプセル状の衝突判定領域を初期化するため
 * [入力] parent: 親オブジェクト, pos1: 始点, pos2: 終点, radius: 半径
 * [出力] なし
 * [副作用] メンバ変数(position_, position2_, radius_)を初期化する
 */
CapsuleCollider::CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius)
	: Collider(parent)
{
	position_ = pos1;
	position2_ = pos2;
	radius_ = radius;
}

/*
 * リソース解放のため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
CapsuleCollider::~CapsuleCollider()
{
}

/*
 * デバッグ用の形状を描画するため
 * [入力] なし
 * [出力] なし
 * [副作用] 画面に描画を行う
 */
void CapsuleCollider::Draw()
{
}

/*
 * 衝突開始時の振る舞いを定義するため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void CapsuleCollider::OnEnter()
{
}

/*
 * 衝突中の振る舞いを定義するため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void CapsuleCollider::OnTrigger()
{
}

/*
 * 衝突終了時の振る舞いを定義するため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void CapsuleCollider::OnExit()
{
}
