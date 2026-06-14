#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

// ゲーム内の3D当たり判定オブジェクトの基底コライダークラス
class Collider
{
public:
	Collider(Object3D* parent);
	virtual ~Collider();

	/*
	 * @brief 別のコライダーとの衝突状態を判定し、OnEnter/OnTrigger/OnExitイベントを発行する
	 * [入力] check: 判定対象の別コライダー, isHit: 衝突したかどうかの結果
	 * [出力] なし
	 * [副作用] mCollisionListの更新、親ゲームオブジェクトへの衝突コールバック通知
	 */
	void HitCheck(Collider* check, bool isHit);

	virtual void Update(Collider* check);
	virtual void Draw();

	virtual void OnEnter();
	virtual void OnTrigger();
	virtual void OnExit();

	// ゲッター・セッター群
	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }
	Object3D* GetParentObject() { return mpParentObject; }
	void InvalidateParent() { mpParentObject = nullptr; }

public:
	Object3D* mpParentObject;      // このコライダーを所有する親3Dオブジェクト

	VECTOR mvPosition;             // コライダーの基準位置座標1（球の中心、またはカプセルの始点）
	VECTOR mvPosition2;            // コライダーの基準位置座標2（カプセルの終点）
	float mfRadius;                // コライダーの半径

	bool mbDeleteFlag;             // コライダーの削除フラグ（trueでマネージャーから除外）

protected:
	std::vector<Collider*> mCollisionList; // 現在このコライダーに衝突し続けている他のコライダーのリスト
};

