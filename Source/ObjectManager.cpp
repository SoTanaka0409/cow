#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

/*
 * @brief オブジェクト管理クラスの初期化
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ObjectManager::ObjectManager()
{
}

/*
 * @brief オブジェクト管理クラスの終了処理
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ObjectManager::~ObjectManager()
{
}

/*
 * @brief 登録された全オブジェクトの更新とカメラ距離の算出を行う
 * [入力] なし
 * [出力] なし
 * [副作用] Zソート用カメラ距離の更新、コライダーの一括処理実行
 */
void ObjectManager::Update()
{
	for (auto itr = object3DList.begin(); itr != object3DList.end(); itr++)
	{
		(*itr)->Update();
	}

	for (auto itr = object2DList.begin(); itr != object2DList.end(); itr++)
	{
		(*itr)->Update();
	}

	// 半透明描画時のZソートで必要となるため、カメラとの距離を算出する
	for (auto itr = object3DList.begin(); itr != object3DList.end(); itr++)
	{
		VECTOR cameraPos = Master::camera->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// 座標更新が全て完了した後に衝突判定を行うためここで一括処理する
	ColliderManager::GetInstance()->Update();
}

/*
 * @brief 登録された全オブジェクトの描画を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 描画フラグが有効なオブジェクトとコライダーの描画命令発行
 */
void ObjectManager::Draw()
{
	for (auto itr = object3DList.begin(); itr != object3DList.end(); itr++)
	{
		if ((*itr)->IsDrawFlag())
		{
			(*itr)->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto itr = object2DList.begin(); itr != object2DList.end(); itr++)
	{
		if ((*itr)->IsDrawFlag())
		{
			(*itr)->Draw();
		}
	}
}

/*
 * @brief 3Dオブジェクトをリストとキャッシュに追加する
 * [入力] object3D: 追加する3Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject3DListおよびmTagCache3Dへの要素追加
 */
void ObjectManager::AddObject(Object3D* object3D)
{
	object3DList.push_back(object3D);
	tagCache3D[object3D->GetTag()].push_back(object3D);
}

/*
 * @brief 3Dオブジェクトをリストから削除する（メモリ解放はしない）
 * [入力] object3D: 削除対象の3Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject3DListからの要素削除とキャッシュの再構築
 */
void ObjectManager::RemoveObjectNoDelete(Object3D* object3D)
{
	object3DList.remove(object3D);
	RebuildTagCache3D();
}

/*
 * @brief 全ての3Dオブジェクトに削除フラグを立て、実際の削除処理を呼ぶ
 * [入力] なし
 * [出力] なし
 * [副作用] 全3Dオブジェクトの削除とメモリ解放
 */
void ObjectManager::DeleteAll3D()
{
	if (object3DList.empty()) return;
	for (auto itr = object3DList.begin(); itr != object3DList.end();)
	{
		(*itr)->SetDeleteFlag(true);
		itr++;
	}

	DeleteAll3DIfNeeded();
}

/*
 * @brief 指定されたタグを持つ3Dオブジェクトを1つ取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致する最初のObject3Dポインタ（見つからない場合はnullptr）
 * [副作用] なし
 */
Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		object3DList.begin(),
		object3DList.end(),
		[&](Object3D* obj) { return obj->GetTag() == tag; }
	);
	if (itr != object3DList.end())
	{
		return (*itr);
	}
	return nullptr;
}

/*
 * @brief 指定されたタグを持つ3Dオブジェクトのリストを取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致するObject3Dポインタのベクタ（キャッシュから参照）
 * [副作用] なし
 */
const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	return tagCache3D[tag];
}

/*
 * @brief 削除フラグが立っている3Dオブジェクトをメモリ解放し、リストから除外する
 * [入力] なし
 * [出力] なし
 * [副作用] 不要オブジェクトのメモリ解放とキャッシュの再構築
 */
void ObjectManager::DeleteAll3DIfNeeded()
{
	bool isDeleted = false;
	if (object3DList.empty()) return;
	for (auto itr = object3DList.begin(); itr != object3DList.end();)
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object3D* temp = *itr;
			itr = object3DList.erase(itr);
			delete temp;
			isDeleted = true;
		}
		else
		{
			itr++;
		}
	}
	// 毎フレームのキャッシュ構築負荷を避けるため、要素変更時のみ再構築する
	if (isDeleted) {
		RebuildTagCache3D();
	}
}

/*
 * @brief 2Dオブジェクトをリストとキャッシュに追加する
 * [入力] object2D: 追加する2Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject2DListおよびmTagCache2Dへの要素追加
 */
void ObjectManager::AddObject(Object2D* object2D)
{
	object2DList.push_back(object2D);
	tagCache2D[object2D->GetTag()].push_back(object2D);
}

/*
 * @brief 全ての2Dオブジェクトをリストから削除し、メモリ解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 全2Dオブジェクトの削除とキャッシュのクリア
 */
void ObjectManager::DeleteAll2D()
{
	for (auto itr = object2DList.begin(); itr != object2DList.end();)
	{
		Object2D* temp = *itr;
		itr = object2DList.erase(itr);
		delete temp;
	}
	tagCache2D.clear();
}

/*
 * @brief 削除フラグが立っている2Dオブジェクトをメモリ解放し、リストから除外する
 * [入力] なし
 * [出力] なし
 * [副作用] 不要オブジェクトのメモリ解放とキャッシュの再構築
 */
void ObjectManager::DeleteAll2DIfNeeded()
{
	bool isDeleted = false;
	for (auto itr = object2DList.begin(); itr != object2DList.end();)
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object2D* temp = *itr;
			itr = object2DList.erase(itr);
			delete temp;
			isDeleted = true;
		}
		else
		{
			itr++;
		}
	}
	// 毎フレームのキャッシュ構築負荷を避けるため、要素変更時のみ再構築する
	if (isDeleted) {
		RebuildTagCache2D();
	}
}

/*
 * @brief 指定されたタグを持つ2Dオブジェクトを1つ取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致する最初のObject2Dポインタ（見つからない場合はnullptr）
 * [副作用] なし
 */
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	auto itr = std::find_if(
		object2DList.begin(),
		object2DList.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }
	);

	if (itr != object2DList.end())
	{
		return (*itr);
	}
	return nullptr;
}

/*
 * @brief 指定されたタグを持つ2Dオブジェクトのリストを取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致するObject2Dポインタのベクタ（キャッシュから参照）
 * [副作用] なし
 */
const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	return tagCache2D[tag];
}

/*
 * @brief 3Dオブジェクト検索用のタグキャッシュを全構築する
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache3Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache3D()
{
	tagCache3D.clear();
	for (auto obj : object3DList) {
		tagCache3D[obj->GetTag()].push_back(obj);
	}
}

/*
 * @brief 2Dオブジェクト検索用のタグキャッシュを全構築する
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache2Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache2D()
{
	tagCache2D.clear();
	for (auto obj : object2DList) {
		tagCache2D[obj->GetTag()].push_back(obj);
	}
}
