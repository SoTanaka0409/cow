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
	for (size_t i = 0; i < mObject3DList.size(); ++i)
	{
		mObject3DList[i]->Update();
	}

	for (size_t i = 0; i < mObject2DList.size(); ++i)
	{
		mObject2DList[i]->Update();
	}

	// 半透明描画時のZソートで必要となるため、カメラとの距離を算出する
	for (size_t i = 0; i < mObject3DList.size(); ++i)
	{
		VECTOR cameraPos = Master::mpCamera->GetPosition();
		VECTOR objPos = mObject3DList[i]->GetPosition();
		mObject3DList[i]->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
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
	for (size_t i = 0; i < mObject3DList.size(); ++i)
	{
		if (mObject3DList[i]->IsDrawFlag())
		{
			mObject3DList[i]->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (size_t i = 0; i < mObject2DList.size(); ++i)
	{
		if (mObject2DList[i]->IsDrawFlag())
		{
			mObject2DList[i]->Draw();
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
	mObject3DList.push_back(object3D);
	mTagCache3D[object3D->GetTag()].push_back(object3D);
}

/*
 * @brief 3Dオブジェクトをリストから削除する（メモリ解放はしない）
 * [入力] object3D: 削除対象の3Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject3DListからの要素削除とキャッシュの再構築
 */
void ObjectManager::RemoveObjectNoDelete(Object3D* object3D)
{
	auto itr = std::find(mObject3DList.begin(), mObject3DList.end(), object3D);
	if (itr != mObject3DList.end())
	{
		mObject3DList.erase(itr);
	}
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
	if (mObject3DList.empty()) return;
	for (size_t i = 0; i < mObject3DList.size(); ++i)
	{
		mObject3DList[i]->SetDeleteFlag(true);
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
		mObject3DList.begin(),
		mObject3DList.end(),
		[&](Object3D* obj) { return obj->GetTag() == tag; }
	);
	if (itr != mObject3DList.end())
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
	return mTagCache3D[tag];
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
	if (mObject3DList.empty()) return;

	auto newEnd = std::remove_if(mObject3DList.begin(), mObject3DList.end(), [](Object3D* obj) {
		if (obj->IsDeleteFlag())
		{
			delete obj;
			return true;
		}
		return false;
	});

	if (newEnd != mObject3DList.end())
	{
		mObject3DList.erase(newEnd, mObject3DList.end());
		isDeleted = true;
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
	mObject2DList.push_back(object2D);
	mTagCache2D[object2D->GetTag()].push_back(object2D);
}

/*
 * @brief 全ての2Dオブジェクトをリストから削除し、メモリ解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 全2Dオブジェクトの削除とキャッシュのクリア
 */
void ObjectManager::DeleteAll2D()
{
	for (size_t i = 0; i < mObject2DList.size(); ++i)
	{
		delete mObject2DList[i];
	}
	mObject2DList.clear();
	mTagCache2D.clear();
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
	auto newEnd = std::remove_if(mObject2DList.begin(), mObject2DList.end(), [](Object2D* obj) {
		if (obj->IsDeleteFlag())
		{
			delete obj;
			return true;
		}
		return false;
	});

	if (newEnd != mObject2DList.end())
	{
		mObject2DList.erase(newEnd, mObject2DList.end());
		isDeleted = true;
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
		mObject2DList.begin(),
		mObject2DList.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }
	);

	if (itr != mObject2DList.end())
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
	return mTagCache2D[tag];
}

/*
 * @brief 3Dオブジェクト検索用のタグキャッシュを全構築する
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache3Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache3D()
{
	mTagCache3D.clear();
	for (auto obj : mObject3DList) {
		mTagCache3D[obj->GetTag()].push_back(obj);
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
	mTagCache2D.clear();
	for (auto obj : mObject2DList) {
		mTagCache2D[obj->GetTag()].push_back(obj);
	}
}
