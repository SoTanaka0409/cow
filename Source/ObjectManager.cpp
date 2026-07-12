#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

/*
 * @brief オブジェクト管?E??ラスの初期匁E
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ObjectManager::ObjectManager()
{
	object3d_list_.reserve(1000);
	object2d_list_.reserve(100);
}

/*
 * @brief オブジェクト管?E??ラスの終?E?E?E
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ObjectManager::~ObjectManager()
{
}

/*
 * @brief 登録された?Eオブジェクト?E更新とカメラ距離の算?Eを行う
 * [入力] なし
 * [出力] なし
 * [副作用] Zソート用カメラ距離の更新、コライダーの一括処?E???E
 */
void ObjectManager::Update()
{
	for (auto obj : object3d_list_)
	{
		obj->Update();
	}

	for (auto obj : object2d_list_)
	{
		obj->Update();
	}

	// 半透?E描画時?EZソートで?E??となるため、カメラとの距離を算?Eする
	for (auto obj : object3d_list_)
	{
		VECTOR cameraPos = Master::camera_->GetPosition();
		VECTOR objPos = obj->GetPosition();
		obj->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// 座標更新が?Eて完?E??た後に衝突判定を行うためここで一括処?E??めE
	ColliderManager::GetInstance()->Update();
}

/*
 * @brief 登録された?Eオブジェクト?E描画を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 描画フラグが有効なオブジェクトとコライダーの描画命令発?E
 */
void ObjectManager::Draw()
{
	for (auto obj : object3d_list_)
	{
		if (obj->IsDrawFlag())
		{
			obj->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto obj : object2d_list_)
	{
		if (obj->IsDrawFlag())
		{
			obj->Draw();
		}
	}
}

/*
 * @brief 3DオブジェクトをリストとキャチE??ュに追加する
 * [入力] object3D: 追加する3Dオブジェクト?Eポインタ
 * [出力] なし
 * [副作用] mObject3DListおよびmTagCache3Dへの要素追加
 */
void ObjectManager::AddObject(Object3D* object3D)
{
	object3d_list_.push_back(object3D);
	tag_cache_3d_[object3D->GetTag()].push_back(object3D);
}

/*
 * @brief 3Dオブジェクトをリストから削除する?E?メモリ解放はしなし??E
 * [入力] object3D: 削除対象の3Dオブジェクト?Eポインタ
 * [出力] なし
 * [副作用] mObject3DListからの要素削除とキャチE??ュの再構篁E
 */
void ObjectManager::RemoveObjectNoDelete(Object3D* object3D)
{
	auto itr = std::find(object3d_list_.begin(), object3d_list_.end(), object3D);
	if (itr != object3d_list_.end())
	{
		object3d_list_.erase(itr);
	}
	RebuildTagCache3D();
}

/*
 * @brief 全ての3Dオブジェクトに削除フラグを立て、実際の削除処?E??呼ぶ
 * [入力] なし
 * [出力] なし
 * [副作用] 全3Dオブジェクト?E削除とメモリ解放
 */
void ObjectManager::DeleteAll3D()
{
	if (object3d_list_.empty()) return;
	for (auto obj : object3d_list_)
	{
		obj->SetDeleteFlag(true);
	}

	DeleteAll3DIfNeeded();
}

/*
 * @brief 持E??されたタグを持つ3Dオブジェクトを1つ取得すめE
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合?Eする最初?EObject3Dポインタ?E?見つからなし??合?Enullptr?E?E
 * [副作用] なし
 */
Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		object3d_list_.begin(),
		object3d_list_.end(),
		[&](Object3D* obj) { return obj->GetTag() == tag; }
	);
	if (itr != object3d_list_.end())
	{
		return (*itr);
	}
	return nullptr;
}

/*
 * @brief 持E??されたタグを持つ3Dオブジェクト?Eリストを取得すめE
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合?EするObject3Dポインタのベクタ?E?キャチE??ュから参?E?E?E
 * [副作用] なし
 */
const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	return tag_cache_3d_[tag];
}

/*
 * @brief 削除フラグが立ってぁE??3Dオブジェクトをメモリ解放し、リストから除外すめE
 * [入力] なし
 * [出力] なし
 * [副作用] 不要オブジェクト?Eメモリ解放とキャチE??ュの再構篁E
 */
void ObjectManager::DeleteAll3DIfNeeded()
{
	bool isDeleted = false;
	if (object3d_list_.empty()) return;

	auto newEnd = std::remove_if(object3d_list_.begin(), object3d_list_.end(), [](Object3D* obj) {
		if (obj->IsDeleteFlag())
		{
			delete obj;
			return true;
		}
		return false;
	});

	if (newEnd != object3d_list_.end())
	{
		object3d_list_.erase(newEnd, object3d_list_.end());
		isDeleted = true;
	}

	// 毎フレームのキャチE??ュ構築負荷を避けるため、要素変更時?Eみ再構築すめE
	if (isDeleted) {
		RebuildTagCache3D();
	}
}

/*
 * @brief 2DオブジェクトをリストとキャチE??ュに追加する
 * [入力] object2D: 追加する2Dオブジェクト?Eポインタ
 * [出力] なし
 * [副作用] mObject2DListおよびmTagCache2Dへの要素追加
 */
void ObjectManager::AddObject(Object2D* object2D)
{
	object2d_list_.push_back(object2D);
	tag_cache_2d_[object2D->GetTag()].push_back(object2D);
}

/*
 * @brief 全ての2Dオブジェクトをリストから削除し、メモリ解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 全2Dオブジェクト?E削除とキャチE??ュのクリア
 */
void ObjectManager::DeleteAll2D()
{
	for (auto obj : object2d_list_)
	{
		delete obj;
	}
	object2d_list_.clear();
	tag_cache_2d_.clear();
}

/*
 * @brief 削除フラグが立ってぁE??2Dオブジェクトをメモリ解放し、リストから除外すめE
 * [入力] なし
 * [出力] なし
 * [副作用] 不要オブジェクト?Eメモリ解放とキャチE??ュの再構篁E
 */
void ObjectManager::DeleteAll2DIfNeeded()
{
	bool isDeleted = false;
	auto newEnd = std::remove_if(object2d_list_.begin(), object2d_list_.end(), [](Object2D* obj) {
		if (obj->IsDeleteFlag())
		{
			delete obj;
			return true;
		}
		return false;
	});

	if (newEnd != object2d_list_.end())
	{
		object2d_list_.erase(newEnd, object2d_list_.end());
		isDeleted = true;
	}

	// 毎フレームのキャチE??ュ構築負荷を避けるため、要素変更時?Eみ再構築すめE
	if (isDeleted) {
		RebuildTagCache2D();
	}
}

/*
 * @brief 持E??されたタグを持つ2Dオブジェクトを1つ取得すめE
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合?Eする最初?EObject2Dポインタ?E?見つからなし??合?Enullptr?E?E
 * [副作用] なし
 */
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	auto itr = std::find_if(
		object2d_list_.begin(),
		object2d_list_.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }
	);

	if (itr != object2d_list_.end())
	{
		return (*itr);
	}
	return nullptr;
}

/*
 * @brief 持E??されたタグを持つ2Dオブジェクト?Eリストを取得すめE
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合?EするObject2Dポインタのベクタ?E?キャチE??ュから参?E?E?E
 * [副作用] なし
 */
const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	return tag_cache_2d_[tag];
}

/*
 * @brief 3Dオブジェクト検索用のタグキャチE??ュを?E構築すめE
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache3Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache3D()
{
	tag_cache_3d_.clear();
	for (auto obj : object3d_list_) {
		tag_cache_3d_[obj->GetTag()].push_back(obj);
	}
}

/*
 * @brief 2Dオブジェクト検索用のタグキャチE??ュを?E構築すめE
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache2Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache2D()
{
	tag_cache_2d_.clear();
	for (auto obj : object2d_list_) {
		tag_cache_2d_[obj->GetTag()].push_back(obj);
	}
}
