#include "SphereCollider.h"
#include "CapsuleCollider.h"

/// @brief コライダーの判定領域となる球体を定義する
/// @param parent 追従する親オブジェクト
/// @param center ローカル座標での中心点
/// @param radius 判定の半径
/// @details Colliderの内部状態を初期化する
SphereCollider::SphereCollider(Object3D* parent, VECTOR center, float radius)
	: Collider(parent)
{
	position_ = center;
	radius_ = radius;
}

/// @brief 継承先でのクリーンアップを保証するため、仮想デストラクタを定義する
SphereCollider::~SphereCollider()
{
}

/// @brief 判定領域の目視確認を容易にするため、デバッグ用ワイヤーフレームを描画する
/// @details 画面に描画コマンドが発行される
void SphereCollider::Draw()
{
	DrawSphere3D(
		position_,
		radius_,
		8,
		GetColor(255, 0, 0),
		GetColor(255, 0, 0),
		false
	);
}

/// @brief 衝突開始時の処理を委譲するため、イベントを発火する
/// @details 親オブジェクトのコールバックを呼び出す
void SphereCollider::OnEnter()
{
}

/// @brief 衝突中の処理を継続して委譲するため、イベントを発火する
/// @details 親オブジェクトのコールバックを呼び出す
void SphereCollider::OnTrigger()
{
}

/// @brief 衝突終了時の処理を委譲するため、イベントを発火する
/// @details 親オブジェクトのコールバックを呼び出す
void SphereCollider::OnExit()
{
}
