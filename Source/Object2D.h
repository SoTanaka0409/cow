#pragma once
#include <string>
#include <vector>
#include "Dxlib.h"

// 2D描画を行うオブジェクトの基底クラス
class Object2D
{
public:
	// オブジェクトの識別用タグ
	enum Tag2D
	{
		None2D = 0,      // 設定なし
		Tag2D_Dino1 = 1,
		Tag2D_Dino2 = 100,
	};

public:
	/*
	 * @brief 2Dオブジェクトを初期座標で生成し、現在のアクティブなオブジェクトマネージャーに自動登録する
	 * [入力] initPos: 初期座標
	 * [出力] なし
	 * [副作用] シーンのObjectManagerに自身が登録される
	 */
	Object2D(VECTOR initPos);

	virtual ~Object2D();

	virtual void Update();

	virtual void Draw();

public:
	// ゲッター・セッター群
	void SetPosition(VECTOR pos) { mvPosition = pos; };
	VECTOR GetPosition() { return mvPosition; }

	void SetRotation(VECTOR rot) { mvRotation = rot; }
	VECTOR GETRotation() { return mvRotation; }

	void SetDeleteFlag(bool flag) { deleteFlag = flag; }
	bool IsDeleteFlag() { return deleteFlag; }

	void SetDrawFlag(bool flag) { drawFlag = flag; }
	bool IsDrawFlag() { return drawFlag; }

	void SetTag(Tag2D tag) { tag = tag; }
	Tag2D GetTag() { return tag; }

protected:
	VECTOR mvPosition;  // 画面上の配置座標
	VECTOR mvRotation;  // 回転角度

private:
	bool deleteFlag;  // 削除フラグ（trueでマネージャーから破棄される）
	Tag2D tag;        // オブジェクト識別タグ
	bool drawFlag;    // 描画を実行するかどうかのフラグ
};

