#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

/// @brief 3Dオブジェクト間の衝突判定を共通化するため。
class Collider
{
public:
	/// @brief オブジェクト生成時に親を設定するため。
	/// @param parent 所属する親オブジェクト
	/// @details parent_object_を初期化する
	Collider(Object3D* parent);

	/// @brief 派生クラスで正しくリソースを解放するため。
	virtual ~Collider();

	/// @brief 衝突状態の遷移を検知し、適切な処理を呼ぶため。
	/// @param check 判定対象の別コライダー
	/// @param is_hit 衝突したかどうかの結果
	/// @details collision_list_の更新、親オブジェクトへの通知
	void HitCheck(Collider* check, bool is_hit);

	/// @brief デバッグ用にコライダーの形状を可視化するため。
	/// @details 画面への描画命令の追加
	virtual void Draw();

	/// @brief 衝突開始時の初期化やエフェクト生成を行うため。
	/// @details 派生クラスによる
	virtual void OnEnter();

	/// @brief 衝突継続中のダメージや押し出し処理を行うため。
	/// @details 派生クラスによる
	virtual void OnTrigger();

	/// @brief 衝突終了時の後処理やエフェクト停止を行うため。
	/// @details 派生クラスによる
	virtual void OnExit();

	/// @brief マネージャーから除外するタイミングを制御するため。
	/// @param flag 削除フラグ
	/// @details delete_flag_の変更
	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }

	/// @brief オブジェクトの生存状態を確認するため。
	/// @return 削除フラグ
	bool IsDeleteFlag() const { return delete_flag_; }

	/// @brief 親のステータスにアクセスするため。
	/// @return 親オブジェクトのポインタ
	Object3D* GetParentObject() { return parent_object_; }

	/// @brief 親オブジェクト破棄時のダングリングポインタを防ぐため。
	/// @details parent_object_の無効化
	void InvalidateParent() { parent_object_ = nullptr; }

public:
	Object3D* parent_object_; ///< 参照対象のオブジェクトを保持するポインタ

	VECTOR position_;  ///< 座標や位置情報を管理する値
	VECTOR position2_; ///< 座標や位置情報を管理する値
	float radius_;     ///< サイズや範囲の計算に使用する値

	bool delete_flag_; ///< 状態の有効・無効を管理するフラグ

protected:
	std::vector<Collider*> collision_list_; ///< 当たり判定の管理に使用する情報
};
