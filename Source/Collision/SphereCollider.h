#pragma once
#include "Collider.h"

/// @brief 複雑な形状の衝突判定負荷を軽減するため、球体による簡易的な判定領域を提供する
class SphereCollider : public Collider
{
public:
	/// @brief コライダーの判定領域となる球体を定義する
	/// @param parent 追従する親オブジェクト
	/// @param center ローカル座標での中心点
	/// @param radius 判定の半径
	/// @details Colliderの内部状態を初期化する
	SphereCollider(Object3D* parent, VECTOR center, float radius);
	virtual ~SphereCollider();

	/// @brief 判定領域の目視確認を容易にするため、デバッグ用ワイヤーフレームを描画する
	/// @details 画面に描画コマンドが発行される
	void Draw() override;

	/// @brief 衝突開始時の処理を委譲するため、イベントを発火する
	/// @details 親オブジェクトのコールバックを呼び出す
	void OnEnter() override;

	/// @brief 衝突中の処理を継続して委譲するため、イベントを発火する
	/// @details 親オブジェクトのコールバックを呼び出す
	void OnTrigger() override;

	/// @brief 衝突終了時の処理を委譲するため、イベントを発火する
	/// @details 親オブジェクトのコールバックを呼び出す
	void OnExit() override;

};
