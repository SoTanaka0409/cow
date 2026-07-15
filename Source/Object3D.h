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
	/*
	 * 半透明描画などのZソートに利用するため
	 * [入力] distance: カメラからの距離
	 * [出力] なし
	 * [副作用] current_camera_distance_を更新する
	 */
	void SetCameraDistance(float distance) { current_camera_distance_ = distance; }

	struct CompareZOrder {
		/*
		 * 半透明オブジェクトの破綻を防ぐため、奥から手前に描画する用途で使用する
		 * [入力] a, b: 比較する2つのオブジェクト
		 * [出力] aがbよりカメラから遠い場合はtrue
		 * [副作用] なし
		 */
		bool operator()(Object3D* a, Object3D* b) const {
			return a->current_camera_distance_ > b->current_camera_distance_;
		}
	};

public:
	/*
	 * 生成と同時にシーンへの自動登録を行い、管理漏れを防ぐ
	 * [入力] initPos: 初期座標
	 * [出力] なし
	 * [副作用] シーンのObjectManagerに自身が登録され、カプセルコライダーが生成される
	 */
	Object3D(VECTOR initPos);

	virtual ~Object3D();

	/*
	 * オブジェクトの毎フレームの振る舞いを実行するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] オブジェクトの座標や状態が更新される
	 */
	virtual void Update();

	/*
	 * オブジェクトの現在の状態を画面に反映するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面にモデルや画像が描画される
	 */
	virtual void Draw();
	virtual void DrawShadowCaster();

	/*
	 * 衝突判定の開始イベントを処理し、ダメージや効果音のトリガーとするため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 派生クラスでの実装に依存して状態が変更される
	 */
	virtual void OnEnter(Collider* collider, Collider* check);

	/*
	 * 継続的な接触状態（押し出しや継続ダメージなど）を処理するため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 派生クラスでの実装に依存して状態が変更される
	 */
	virtual void OnTrigger(Collider* collider, Collider* check);

	/*
	 * 衝突終了イベントを処理し、接触状態のリセットなどを行うため
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 派生クラスでの実装に依存して状態が変更される
	 */
	virtual void OnExit(Collider* collider, Collider* check);

public:
	void SetPosition(VECTOR pos) { position_ = pos; };
	VECTOR GetPosition() { return position_; }

	void SetOldPosition(VECTOR pos) { old_position_ = pos; }
	VECTOR GetOldPosition() { return old_position_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GETRotation() { return rotation_; }

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

	void SetDrawFlag(bool flag) { draw_flag_ = flag; }
	bool IsDrawFlag() { return draw_flag_; }

	void SetTag(Tag3D tag) { tag_ = tag; }
	Tag3D GetTag() { return tag_; }

protected:
	VECTOR position_;
	VECTOR rotation_;
	VECTOR old_position_;
	CapsuleCollider* capsule_collider_;
	float radius_;

private:
	bool delete_flag_;
	Tag3D tag_;
	bool draw_flag_;
	float current_camera_distance_;
};
