#include "SphereCollider.h"
#include "CapsuleCollider.h"

/*
 * コライダーの判定領域となる球体を定義する
 * [入力] parent: 追従する親オブジェクト, center: ローカル座標での中心点, radius: 判定の半径
 * [出力] なし
 * [副作用] Colliderの内部状態を初期化する
 */
SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	position_ = center;
	radius_ = radius;
}

/*
 * 継承先でのクリーンアップを保証するため、仮想デストラクタを定義する
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
SphereCollider::~SphereCollider()
{
}

/*
 * 判定領域の目視確認を容易にするため、デバッグ用ワイヤーフレームを描画する
 * [入力] なし
 * [出力] なし
 * [副作用] 画面に描画コマンドが発行される
 */
void SphereCollider::Draw()
{
	DrawSphere3D(
		position_,
		radius_,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

/*
 * 衝突開始時の処理を委譲するため、イベントを発火する
 * [入力] なし
 * [出力] なし
 * [副作用] 親オブジェクトのコールバックを呼び出す
 */
void SphereCollider::OnEnter()
{
}

/*
 * 衝突中の処理を継続して委譲するため、イベントを発火する
 * [入力] なし
 * [出力] なし
 * [副作用] 親オブジェクトのコールバックを呼び出す
 */
void SphereCollider::OnTrigger()
{
}

/*
 * 衝突終了時の処理を委譲するため、イベントを発火する
 * [入力] なし
 * [出力] なし
 * [副作用] 親オブジェクトのコールバックを呼び出す
 */
void SphereCollider::OnExit()
{
}
