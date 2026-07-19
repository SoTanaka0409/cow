#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 各種オブジェクトリストのメモリ事前確保
 */
ObjectManager::ObjectManager()
{
	// ゲーム進行中の動的メモリ確保に伴うスパイク（処理落ち）を防ぐため、想定される最大数を事前確保する
	object3d_list_.reserve(1000);
	object2d_list_.reserve(100);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: unique_ptrによるオブジェクトの自動破棄
 */
ObjectManager::~ObjectManager()
{
	// 手動deleteによるメモリリークや二重解放バグを防ぐため、解放処理はスマートポインタ(unique_ptr)に委譲する
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 全オブジェクトの更新、カメラ距離の計算、および不要オブジェクトの削除
 */
void ObjectManager::Update()
{
	for (auto& obj : object3d_list_)
	{
		obj->Update();
	}

	for (auto& obj : object2d_list_)
	{
		obj->Update();
	}

	for (auto& obj : object3d_list_)
	{
		// 半透明オブジェクトのZソート描画や、距離ベースのLOD(負荷軽減)を後続処理で行うためカメラ距離を事前計算する
		VECTOR cameraPos = Master::camera_->GetPosition();
		VECTOR objPos = obj->GetPosition();
		obj->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// ループ処理中に要素を直接削除してイテレータが無効化されクラッシュするのを防ぐため、フレーム末尾で遅延処理する
	DeleteAll3DIfNeeded();
	DeleteAll2DIfNeeded();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 全オブジェクトの描画
 */
void ObjectManager::Draw()
{
	for (auto& obj : object3d_list_)
	{
		obj->Draw();
	}

	for (auto& obj : object2d_list_)
	{
		obj->Draw();
	}

	if (Master::is_debug_mode_)
	{
		for (auto& obj : object3d_list_)
		{
			obj->DrawDebug();
		}
	}
}

/*
 * 入力: object3D (追加する3Dオブジェクトのポインタ)
 * 出力: なし
 * 副作用: 管理リストへの追加およびタグキャッシュへの登録
 */
void ObjectManager::AddObject(Object3D* object3D)
{
	// AIや衝突判定などで特定タグの全検索(O(N))が毎フレーム発生して処理落ちするのを防ぐため、追加時にキャッシュしておく
	object3d_list_.emplace_back(object3D);
	tag_cache_3d_[object3D->GetTag()].push_back(object3D);
}

/*
 * 入力: object3D (管理から除外する3Dオブジェクトのポインタ)
 * 出力: なし
 * 副作用: unique_ptrの所有権放棄、リストからの除外、キャッシュの再構築
 */
void ObjectManager::RemoveObjectNoDelete(Object3D* object3D)
{
	auto itr = std::find_if(object3d_list_.begin(), object3d_list_.end(), [object3D](const std::unique_ptr<Object3D>& ptr) {
		return ptr.get() == object3D;
		});
	if (itr != object3d_list_.end())
	{
		// シーン間移動やプールへの返却など、所有権を他へ移譲する際にオブジェクトごと破棄されてしまうバグを防ぐ
		itr->release();
		object3d_list_.erase(itr);
	}
	RebuildTagCache3D();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 全3Dオブジェクトの即時破棄とキャッシュクリア
 */
void ObjectManager::DeleteAll3D()
{
	object3d_list_.clear();
	tag_cache_3d_.clear();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 削除フラグの立った3Dオブジェクトの破棄とキャッシュ再構築
 */
void ObjectManager::DeleteAll3DIfNeeded()
{
	bool isDeleted = false;
	if (object3d_list_.empty()) return;

	// 中途削除による配列要素のメモリ詰め直し(O(N))が多発してパフォーマンスが低下するのを防ぐため、erase-removeで一括削除する
	auto newEnd = std::remove_if(object3d_list_.begin(), object3d_list_.end(), [](const std::unique_ptr<Object3D>& obj) {
		return obj->IsDeleteFlag();
		});

	if (newEnd != object3d_list_.end())
	{
		object3d_list_.erase(newEnd, object3d_list_.end());
		isDeleted = true;
	}

	if (isDeleted) {
		RebuildTagCache3D();
	}
}

/*
 * 入力: tag (検索対象の3Dタグ)
 * 出力: 条件に合致した最初のObject3Dポインタ（無ければnullptr）
 * 副作用: なし
 */
Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		object3d_list_.begin(),
		object3d_list_.end(),
		[&](const std::unique_ptr<Object3D>& obj) { return obj->GetTag() == tag; }
	);
	if (itr != object3d_list_.end())
	{
		return itr->get();
	}
	return nullptr;
}

/*
 * 入力: tag (検索対象の3Dタグ)
 * 出力: 条件に合致したObject3Dポインタのリスト参照
 * 副作用: なし
 */
const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	return tag_cache_3d_[tag];
}

/*
 * 入力: object2D (追加する2Dオブジェクトのポインタ)
 * 出力: なし
 * 副作用: 管理リストへの追加およびタグキャッシュへの登録
 */
void ObjectManager::AddObject(Object2D* object2D)
{
	object2d_list_.emplace_back(object2D);
	tag_cache_2d_[object2D->GetTag()].push_back(object2D);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 全2Dオブジェクトの即時破棄とキャッシュクリア
 */
void ObjectManager::DeleteAll2D()
{
	object2d_list_.clear();
	tag_cache_2d_.clear();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 削除フラグの立った2Dオブジェクトの破棄とキャッシュ再構築
 */
void ObjectManager::DeleteAll2DIfNeeded()
{
	bool isDeleted = false;
	auto newEnd = std::remove_if(object2d_list_.begin(), object2d_list_.end(), [](const std::unique_ptr<Object2D>& obj) {
		return obj->IsDeleteFlag();
		});

	if (newEnd != object2d_list_.end())
	{
		object2d_list_.erase(newEnd, object2d_list_.end());
		isDeleted = true;
	}

	if (isDeleted) {
		RebuildTagCache2D();
	}
}

/*
 * 入力: tag (検索対象の2Dタグ)
 * 出力: 条件に合致した最初のObject2Dポインタ（無ければnullptr）
 * 副作用: なし
 */
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	auto itr = std::find_if(
		object2d_list_.begin(),
		object2d_list_.end(),
		[&](const std::unique_ptr<Object2D>& obj) { return obj->GetTag() == tag; }
	);

	if (itr != object2d_list_.end())
	{
		return itr->get();
	}
	return nullptr;
}

/*
 * 入力: tag (検索対象の2Dタグ)
 * 出力: 条件に合致したObject2Dポインタのリスト参照
 * 副作用: なし
 */
const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	return tag_cache_2d_[tag];
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 3Dタグキャッシュの完全な再生成
 */
void ObjectManager::RebuildTagCache3D()
{
	// 要素削除後にキャッシュ内にダングリングポインタ(無効なアドレス)が残留し、他クラス参照時にクラッシュするのを防ぐ
	tag_cache_3d_.clear();
	for (auto& obj : object3d_list_)
	{
		tag_cache_3d_[obj->GetTag()].push_back(obj.get());
	}
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 2Dタグキャッシュの完全な再生成
 */
void ObjectManager::RebuildTagCache2D()
{
	tag_cache_2d_.clear();
	for (auto& obj : object2d_list_)
	{
		tag_cache_2d_[obj->GetTag()].push_back(obj.get());
	}
}
