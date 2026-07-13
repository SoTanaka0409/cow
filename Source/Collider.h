#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

// 3Dオブジェクト間の衝突判定を共通化するため。
class Collider
{
public:
	/*
	 * オブジェクト生成時に親を設定するため。
	 * [入力] parent: 所属する親オブジェクト
	 * [出力] なし
	 * [副作用] parent_object_を初期化する
	 */
	Collider(Object3D* parent);

	/*
	 * 派生クラスで正しくリソースを解放するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Collider();

	/*
	 * 衝突状態の遷移を検知し、適切な処理を呼ぶため。
	 * [入力] check: 判定対象の別コライダー, isHit: 衝突したかどうかの結果
	 * [出力] なし
	 * [副作用] collision_list_の更新、親オブジェクトへの通知
	 */
	void HitCheck(Collider* check, bool isHit);

	/*
	 * デバッグ用にコライダーの形状を可視化するため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面への描画命令の追加
	 */
	virtual void Draw();

	/*
	 * 衝突開始時の初期化やエフェクト生成を行うため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 派生クラスによる
	 */
	virtual void OnEnter();

	/*
	 * 衝突継続中のダメージや押し出し処理を行うため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 派生クラスによる
	 */
	virtual void OnTrigger();

	/*
	 * 衝突終了時の後処理やエフェクト停止を行うため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 派生クラスによる
	 */
	virtual void OnExit();

	/*
	 * マネージャーから除外するタイミングを制御するため。
	 * [入力] flag: 削除フラグ
	 * [出力] なし
	 * [副作用] delete_flag_の変更
	 */
	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }

	/*
	 * オブジェクトの生存状態を確認するため。
	 * [入力] なし
	 * [出力] 削除フラグ
	 * [副作用] なし
	 */
	bool IsDeleteFlag() { return delete_flag_; }

	/*
	 * 親のステータスにアクセスするため。
	 * [入力] なし
	 * [出力] 親オブジェクトのポインタ
	 * [副作用] なし
	 */
	Object3D* GetParentObject() { return parent_object_; }

	/*
	 * 親オブジェクト破棄時のダングリングポインタを防ぐため。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] parent_object_の無効化
	 */
	void InvalidateParent() { parent_object_ = nullptr; }

public:
	Object3D* parent_object_;

	VECTOR position_;
	VECTOR position2_;
	float radius_;

	bool delete_flag_;

protected:
	std::vector<Collider*> collision_list_;
};