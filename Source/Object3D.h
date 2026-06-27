#pragma once
#include "DxLib.h"
#include <string>

class Collider;
class CapsuleCollider;

// 3D空間に配置されるオブジェクトの基底クラス
class Object3D
{
public:
	// オブジェクトの識別用タグ
	enum Tag3D
	{
		None3D,
		Tag3D_Stage,
		Tag3D_player,
		Tag3D_Enemy,
		Tag3D_Cow,
		Tag3D_Wall,
		Tag3D_Bait,
		Tag3D_Animal
	};

public:
	// カメラからの距離を設定する
	void SetCameraDistance(float distance) { currentCameraDistance = distance; }

	// カメラからの距離を基準に遠い順（Z値の降順）でソートするための比較関数
	struct CompareZOrder {
		bool operator()(Object3D* a, Object3D* b) const {
			return a->currentCameraDistance > b->currentCameraDistance;
		}
	};

public:
	/*
	 * @brief 3Dオブジェクトを初期座標で生成し、現在アクティブなシーンのマネージャーへ自動登録する
	 * [入力] initPos: 初期座標
	 * [出力] なし
	 * [副作用] シーンのObjectManagerに自身が登録される、デフォルトのカプセルコライダーが生成される
	 */
	Object3D(VECTOR initPos);

	virtual ~Object3D();

	virtual void Update();

	virtual void Draw();

	/*
	 * @brief コライダー同士の交差が開始した瞬間に呼ばれるコールバック関数
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void OnEnter(Collider* collider, Collider* check);

	/*
	 * @brief コライダー同士が交差している間、毎フレーム呼ばれるコールバック関数
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void OnTrigger(Collider* collider, Collider* check);

	/*
	 * @brief コライダー同士の交差が終了した瞬間に呼ばれるコールバック関数
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void OnExit(Collider* collider, Collider* check);

public:
	// ゲッター・セッター群
	void SetPosition(VECTOR pos) { mvPosition = pos; };
	VECTOR GetPosition() { return mvPosition; }

	void SetOldPosition(VECTOR pos) { mvOldPosition = pos; }
	VECTOR GetOldPosition() { return mvOldPosition; }

	void SetRotation(VECTOR rot) { mvRotation = rot; }
	VECTOR GETRotation() { return mvRotation; }

	void SetDeleteFlag(bool flag) { deleteFlag = flag; }
	bool IsDeleteFlag() { return deleteFlag; }

	void SetDrawFlag(bool flag) { drawFlag = flag; }
	bool IsDrawFlag() { return drawFlag; }

	void SetTag(Tag3D tag) { tag = tag; }
	Tag3D GetTag() { return tag; }

protected:
	VECTOR mvPosition;                  // 現在の座標
	VECTOR mvRotation;                  // 回転角度
	VECTOR mvOldPosition;               // 1フレーム前の座標
	CapsuleCollider* capsuleCollider; // 所有するカプセルコライダー
	float radius;                     // 簡易的な衝突半径

private:
	bool deleteFlag;                  // 削除フラグ（trueでマネージャーから破棄される）
	Tag3D tag;                        // オブジェクト識別タグ
	bool drawFlag;                    // 描画フラグ
	float currentCameraDistance;      // カメラからこのオブジェクトまでの距離
};
