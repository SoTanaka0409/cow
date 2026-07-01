#pragma once

#include<vector>
#include<map>
#include"Object3D.h"
#include"Object2D.h"

// シーン上の全オブジェクトを一元管理し、ライフサイクルを制御するクラス
class ObjectManager
{
public:
	ObjectManager();

	~ObjectManager();

	/*
	 * 登録されているすべてのオブジェクトを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面へのレンダリング
	 */
	void Draw();

	/*
	 * オブジェクトの更新と不要なオブジェクトの破棄を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] オブジェクトの更新処理、メモリ解放
	 */
	void Update();

public:

	/*
	 * 管理リストに3Dオブジェクトを追加する
	 * [入力] object3D: 追加する3Dオブジェクト
	 * [出力] なし
	 * [副作用] mObject3DListへの追加
	 */
	void AddObject(Object3D* object3D);

	/*
	 * 管理リストから3Dオブジェクトを除外する（メモリ解放はしない）
	 * [入力] object3D: 除外する3Dオブジェクト
	 * [出力] なし
	 * [副作用] mObject3DListからの削除
	 */
	void RemoveObjectNoDelete(Object3D* object3D);

	/*
	 * すべての3Dオブジェクトを強制的に破棄・メモリ解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 全3Dオブジェクトのdeleteとリストクリア
	 */
	void DeleteAll3D();

	/*
	 * 削除フラグが立った3Dオブジェクトを安全に破棄し、メモリを解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 該当オブジェクトのdelete、リストからの除外
	 */
	void DeleteAll3DIfNeeded();

	/*
	 * 指定タグを持つ3Dオブジェクトを1つ取得する
	 * [入力] tag: 検索対象の3Dタグ
	 * [出力] 見つかった3Dオブジェクトのポインタ
	 * [副作用] なし
	 */
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	/*
	 * 指定タグを持つすべての3Dオブジェクトのリストを取得する
	 * [入力] tag: 検索対象の3Dタグ
	 * [出力] 指定タグを持つ3Dオブジェクトの配列
	 * [副作用] なし
	 */
	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);

	/*
	 * 管理リストに2Dオブジェクトを追加する
	 * [入力] object2D: 追加する2Dオブジェクト
	 * [出力] なし
	 * [副作用] mObject2DListへの追加
	 */
	void AddObject(Object2D* object2D);

	/*
	 * すべての2Dオブジェクトを強制的に破棄・メモリ解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 全2Dオブジェクトのdeleteとリストクリア
	 */
	void DeleteAll2D();

	/*
	 * 削除フラグが立った2Dオブジェクトを安全に破棄し、メモリを解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 該当オブジェクトのdelete、リストからの除外
	 */
	void DeleteAll2DIfNeeded();

	/*
	 * 指定タグを持つ2Dオブジェクトを1つ取得する
	 * [入力] tag: 検索対象の2Dタグ
	 * [出力] 見つかった2Dオブジェクトのポインタ
	 * [副作用] なし
	 */
	Object2D* GetObject2DByTag(Object2D::Tag2D tag);

	/*
	 * 指定タグを持つすべての2Dオブジェクトのリストを取得する
	 * [入力] tag: 検索対象の2Dタグ
	 * [出力] 指定タグを持つ2Dオブジェクトの配列
	 * [副作用] なし
	 */
	const std::vector<Object2D*>& GetObject2DListByTag(Object2D::Tag2D tag);

	/*
	 * 3Dオブジェクトのタグキャッシュを再構築する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mTagCache3Dの更新
	 */
	void RebuildTagCache3D();

	/*
	 * 2Dオブジェクトのタグキャッシュを再構築する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mTagCache2Dの更新
	 */
	void RebuildTagCache2D();

private:
	std::vector<Object3D*> mObject3DList;
	std::map<Object3D::Tag3D, std::vector<Object3D*>> mTagCache3D;
	
	std::vector<Object2D*> mObject2DList;
	std::map<Object2D::Tag2D, std::vector<Object2D*>> mTagCache2D;
};
