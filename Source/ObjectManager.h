#pragma once

#include<vector>
#include<memory>
#include<map>
#include"Object3D.h"
#include"Object2D.h"

// unique_ptrによる所有権管理でメモリリークを防ぎ、シーン内オブジェクトの更新・描画・破棄を安全な順序で一元実行するマネージャー
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	// [入力] なし [出力] なし [副作用] 3Dおよび2Dオブジェクトの描画命令発行
	// Zバッファの仕様上、半透明描画が破綻しないよう必ず「3D描画の完了後に2Dを描画する」という順序を厳守している
	void Draw();

	// [入力] なし [出力] なし [副作用] 全オブジェクトの更新と遅延削除の実行
	// イテレータ無効化（走査中のリスト書き換え）によるクラッシュを防ぐため、Update呼び出しと削除処理をこの関数内で安全に同期する
	void Update();

public:

	// [入力] 追加する3Dオブジェクト [出力] なし [副作用] unique_ptrへの所有権の移行とリスト登録
	// メモリ二重解放バグを防ぐため、引数として渡したポインタの所有権・破棄責任はマネージャー側に完全に移譲される点に注意すること
	void AddObject(Object3D* object3D);

	// [入力] 除外する3Dオブジェクト [出力] なし [副作用] リストからの除外と所有権の放棄
	// シーン遷移などでオブジェクトを生存させたままマネージャー管理から切り離す用途。呼び出し側が手動でdeleteする責任を負う
	void RemoveObjectNoDelete(Object3D* object3D);

	// [入力] なし [出力] なし [副作用] 3D管理リストのクリアと全オブジェクトの即時破棄
	// シーン切り替え時に古いオブジェクトが残存してクラッシュや不正な当たり判定を起こすバグを防ぐため、確実に呼び出すこと
	void DeleteAll3D();

	// [入力] なし [出力] なし [副作用] 削除フラグが有効な3Dオブジェクトの破棄とリスト除外
	// 毎フレームのUpdate処理直後に呼ばれ、安全なタイミングで一括してガベージコレクションを行うための内部処理的メソッド
	void DeleteAll3DIfNeeded();

	// [入力] 検索対象の3Dタグ [出力] 見つかったオブジェクトへのポインタ(見つからなければnullptr) [副作用] なし
	// 毎フレームのリスト全探索によるO(N)の処理落ちを防ぐため、動的構築されたタグキャッシュからO(1)〜O(logN)で高速取得する
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	// [入力] 検索対象の3Dタグ [出力] 対象タグを持つ全オブジェクトのポインタ配列 [副作用] なし
	// 同上。戻り値はキャッシュの参照を返すため、受け取り側でリストそのものを改ざんしないこと
	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);

	// [入力] 追加する2Dオブジェクト [出力] なし [副作用] unique_ptrへの所有権の移行とリスト登録
	// UIの重なり（Zオーダー）は追加された順番に依存するため、奥に描画したいものから順にAddすること
	void AddObject(Object2D* object2D);

	// [入力] なし [出力] なし [副作用] 2D管理リストのクリアと全オブジェクトの即時破棄
	void DeleteAll2D();

	// [入力] なし [出力] なし [副作用] 削除フラグが有効な2Dオブジェクトの破棄とリスト除外
	void DeleteAll2DIfNeeded();

	// [入力] 検索対象の2Dタグ [出力] 見つかったオブジェクトへのポインタ [副作用] なし
	Object2D* GetObject2DByTag(Object2D::Tag2D tag);

	// [入力] 検索対象の2Dタグ [出力] 対象タグを持つ全オブジェクトのポインタ配列 [副作用] なし
	const std::vector<Object2D*>& GetObject2DListByTag(Object2D::Tag2D tag);

	// [入力] なし [出力] なし [副作用] tag_cache_3d_の再構築
	// オブジェクト生成後、動的にタグを変更した際に検索結果の不整合バグを防ぐため、明示的に呼び出してキャッシュを同期させること
	void RebuildTagCache3D();

	// [入力] なし [出力] なし [副作用] tag_cache_2d_の再構築
	void RebuildTagCache2D();

private:
	std::vector<std::unique_ptr<Object3D>> object3d_list_;            // 3Dオブジェクトの管理リスト
	std::map<Object3D::Tag3D, std::vector<Object3D*>>tag_cache_3d_;  // 高速検索用の3Dタグキャッシュ

	std::vector<std::unique_ptr<Object2D>> object2d_list_;            // 2Dオブジェクトの管理リスト
	std::map<Object2D::Tag2D, std::vector<Object2D*>> tag_cache_2d_;  // 高速検索用の2Dタグキャッシュ
};