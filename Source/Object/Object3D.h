#pragma once
#include "DxLib.h"
#include <string>

class Collider;
class CapsuleCollider;

class Object3D
{
public:
	enum Tag3D
	{
		kNone3d,
		kTag3dStage,
		kTag3dPlayer,
		kTag3dEnemy,
		kTag3dCow,
		kTag3dWall,
		kTag3dBait,
		kTag3dAnimal
	};

public:
	/// @brief 半透明描画などのZソートに利用するため
	/// @param distance カメラからの距離
	/// @details current_camera_distance_を更新する
	void SetCameraDistance(float distance) { current_camera_distance_ = distance; }

	struct CompareZOrder {
		/// @brief 半透明オブジェクトの破綻を防ぐため、奥から手前に描画する用途で使用する
		/// @param b 比較する2つのオブジェクト
		/// @return aがbよりカメラから遠い場合はtrue
		/// @details a
		bool operator()(Object3D* a, Object3D* b) const {
			return a->current_camera_distance_ > b->current_camera_distance_;
		}
	};

public:
	/// @brief 生成と同時にシーンへの自動登録を行い、管理漏れを防ぐ
	/// @param init_pos 初期座標
	/// @details シーンのObjectManagerに自身が登録され、カプセルコライダーが生成される
	Object3D(VECTOR init_pos);

	virtual ~Object3D();

	/// @brief オブジェクトの毎フレームの振る舞いを実行するため
	/// @details オブジェクトの座標や状態が更新される
	virtual void Update();

	/// @brief オブジェクトの現在の状態を画面に反映するため
	/// @details 画面にモデルや画像が描画される
	virtual void Draw();
	virtual void DrawShadowCaster();
	virtual void DrawDebug();

	/// @brief 衝突判定の開始イベントを処理し、ダメージや効果音のトリガーとするため
	/// @param collider 自身のコライダー
	/// @param check 相手のコライダー
	/// @details 派生クラスでの実装に依存して状態が変更される
	virtual void OnEnter(Collider* collider, Collider* check);

	/// @brief 継続的な接触状態（押し出しや継続ダメージなど）を処理するため
	/// @param collider 自身のコライダー
	/// @param check 相手のコライダー
	/// @details 派生クラスでの実装に依存して状態が変更される
	virtual void OnTrigger(Collider* collider, Collider* check);

	/// @brief 衝突終了イベントを処理し、接触状態のリセットなどを行うため
	/// @param collider 自身のコライダー
	/// @param check 相手のコライダー
	/// @details 派生クラスでの実装に依存して状態が変更される
	virtual void OnExit(Collider* collider, Collider* check);

public:
	/// @brief Positionの設定
	void SetPosition(VECTOR pos) { position_ = pos; };
	/// @brief Positionの取得
	VECTOR GetPosition() const { return position_; }

	/// @brief OldPositionの設定
	void SetOldPosition(VECTOR pos) { old_position_ = pos; }
	/// @brief OldPositionの取得
	VECTOR GetOldPosition() const { return old_position_; }

	/// @brief Rotationの設定
	void SetRotation(VECTOR rot) { rotation_ = rot; }
	/// @brief Rotationの取得
	VECTOR GetRotation() const { return rotation_; }

	/// @brief DeleteFlagの設定
	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	/// @brief DeleteFlagの判定
	bool IsDeleteFlag() const { return delete_flag_; }

	/// @brief DrawFlagの設定
	void SetDrawFlag(bool flag) { draw_flag_ = flag; }
	/// @brief DrawFlagの判定
	bool IsDrawFlag() const { return draw_flag_; }

	/// @brief Tagの設定
	void SetTag(Tag3D tag) { tag_ = tag; }
	/// @brief Tagの取得
	Tag3D GetTag() { return tag_; }

protected:
	VECTOR position_;                   ///< 座標や位置情報を管理する値
	VECTOR rotation_;                   ///< 移動や回転の計算に使用する値
	VECTOR old_position_;               ///< 座標や位置情報を管理する値
	CapsuleCollider* capsule_collider_; ///< 当たり判定の管理に使用する情報
	float radius_;                      ///< サイズや範囲の計算に使用する値

private:
	bool delete_flag_;              ///< 状態の有効・無効を管理するフラグ
	Tag3D tag_;                     ///< 現在の状態や種別を管理する値
	bool draw_flag_;                ///< 状態の有効・無効を管理するフラグ
	float current_camera_distance_; ///< 内部状態を管理する値
};
