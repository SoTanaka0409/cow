#pragma once

#include<list>
#include<vector>
#include<map>
#include"Object3D.h"
#include"Object2D.h"


// シーン上の全ゲームオブジェクト（2D/3D）の一元管理と更新・描画・クリーンアップを担うクラス
class ObjectManager
{
public:
	ObjectManager();

	~ObjectManager();

	/*
	 * @brief 登録されているすべての2D/3Dオブジェクトを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各オブジェクトのDraw関数が実行され、画面にレンダリングされる
	 */
	void Draw();

	/*
	 * @brief 登録されているすべての2D/3Dオブジェクトの更新処理を行い、不要なオブジェクトの破棄を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各オブジェクトのUpdate実行、不要オブジェクトのメモリ解放
	 */
	void Update();

public:

	/*
	 * @brief 管理リストに3Dオブジェクトを追加する
	 * [入力] object3D: 追加する3Dオブジェクトのポインタ
	 * [出力] なし
	 * [副作用] mObject3DListへの追加
	 */
	void AddObject(Object3D* object3D);

	/*
	 * @brief 管理リストから3Dオブジェクトを除外する（メモリ解放はしない）
	 */
	void RemoveObjectNoDelete(Object3D* object3D);

	/*
	 * @brief すべての3Dオブジェクトを強制的に破棄・メモリ解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 管理している全3Dオブジェクトのdelete、リストおよびキャッシュのクリア
	 */
	void DeleteAll3D();

	/*
	 * @brief 削除フラグが立った3Dオブジェクトを安全に破棄し、メモリを解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 該当オブジェクトのdelete、リストからの除外
	 */
	void DeleteAll3DIfNeeded();

	/*
	 * @brief 指定タグを持つ3Dオブジェクトを1つ取得する
	 * [入力] tag: 検索対象の3Dタグ
	 * [出力] 見つかった3Dオブジェクトのポインタ（存在しない場合はnullptr）
	 * [副作用] なし
	 */
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	/*
	 * @brief 指定タグを持つすべての3Dオブジェクトのリスト（参照）を取得する
	 * [入力] tag: 検索対象の3Dタグ
	 * [出力] 指定タグを持つ3Dオブジェクトポインタの配列
	 * [副作用] なし
	 */
	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);

	////////////////////////////////////////////////////////////////////////

	/*
	 * @brief 管理リストに2Dオブジェクトを追加する
	 * [入力] object2D: 追加する2Dオブジェクトのポインタ
	 * [出力] なし
	 * [副作用] mObject2DListへの追加
	 */
	void AddObject(Object2D* object2D);

	/*
	 * @brief すべての2Dオブジェクトを強制的に破棄・メモリ解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 管理している全2Dオブジェクトのdelete、リストおよびキャッシュのクリア
	 */
	void DeleteAll2D();

	/*
	 * @brief 削除フラグが立った2Dオブジェクトを安全に破棄し、メモリを解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 該当オブジェクトのdelete、リストからの除外
	 */
	void DeleteAll2DIfNeeded();

	/*
	 * @brief 指定タグを持つ2Dオブジェクトを1つ取得する
	 * [入力] tag: 検索対象の2Dタグ
	 * [出力] 見つかった2Dオブジェクトのポインタ（存在しない場合はnullptr）
	 * [副作用] なし
	 */
	Object2D* GetObject2DByTag(Object2D::Tag2D tag);

	/*
	 * @brief 指定タグを持つすべての2Dオブジェクトのリスト（参照）を取得する
	 * [入力] tag: 検索対象の2Dタグ
	 * [出力] 指定タグを持つ2Dオブジェクトポインタの配列
	 * [副作用] なし
	 */
	const std::vector<Object2D*>& GetObject2DListByTag(Object2D::Tag2D tag);

	/*
	 * @brief 3Dオブジェクトのタグキャッシュを再構築する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mTagCache3Dの更新
	 */
	void RebuildTagCache3D();

	/*
	 * @brief 2Dオブジェクトのタグキャッシュを再構築する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mTagCache2Dの更新
	 */
	void RebuildTagCache2D();

private:
	
	std::list<Object3D*> mObject3DList;                           // 3Dオブジェクトの管理リスト
	std::map<Object3D::Tag3D, std::vector<Object3D*>> mTagCache3D; // タグごとの3Dオブジェクトキャッシュ
	
	std::list<Object2D*> mObject2DList;                           // 2Dオブジェクトの管理リスト
	std::map<Object2D::Tag2D, std::vector<Object2D*>> mTagCache2D; // タグごとの2Dオブジェクトキャッシュ
};
