#pragma once
#include "Collider.h"

// 複雑な形状の衝突判定負荷を軽減するため、球体による簡易的な判定領域を提供する
class SphereCollider : public Collider
{
public:
	/*
	 * コライダーの判定領域となる球体を定義する
	 * [入力] parent: 追従する親オブジェクト, center: ローカル座標での中心点, radius: 判定の半径
	 * [出力] なし
	 * [副作用] Colliderの内部状態を初期化する
	 */
	SphereCollider(Object3D* parent, VECTOR center, float radius);
	virtual ~SphereCollider();

	/*
	 * 判定領域の目視確認を容易にするため、デバッグ用ワイヤーフレームを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面に描画コマンドが発行される
	 */
	void Draw() override;

	/*
	 * 衝突開始時の処理を委譲するため、イベントを発火する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 親オブジェクトのコールバックを呼び出す
	 */
	void OnEnter() override;

	/*
	 * 衝突中の処理を継続して委譲するため、イベントを発火する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 親オブジェクトのコールバックを呼び出す
	 */
	void OnTrigger() override;

	/*
	 * 衝突終了時の処理を委譲するため、イベントを発火する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 親オブジェクトのコールバックを呼び出す
	 */
	void OnExit() override;

};
