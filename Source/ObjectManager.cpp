#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::Update()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		(*itr)->Update();
	}

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Update();
	}

	// 描画順（Zソート）のための基準値として、各オブジェクトとカメラの距離を計算しておく
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		VECTOR cameraPos = Master::mpCamera->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// コライダーの交差・イベント通知を一括処理する
	ColliderManager::GetInstance()->Update();
}

void ObjectManager::Draw()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		if ((*itr)->IsDrawFlag())
		{
			(*itr)->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		if ((*itr)->IsDrawFlag())
		{
			(*itr)->Draw();
		}
	}
}

void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);
	mTagCache3D[object3D->GetTag()].push_back(object3D);
}

void ObjectManager::DeleteAll3D()
{
	if (mObject3DList.empty()) return;
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		(*itr)->SetDeleteFlag(true);
		itr++;
	}

	DeleteAll3DIfNeeded();
}

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

const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	return mTagCache3D[tag];
}

void ObjectManager::DeleteAll3DIfNeeded()
{
	bool isDeleted = false;
	if (mObject3DList.empty()) return;
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object3D* temp = *itr;
			itr = mObject3DList.erase(itr);
			delete temp;
			isDeleted = true;
		}
		else
		{
			itr++;
		}
	}
	// リスト要素に変更があった場合のみ、検索高速化用キャッシュを再構築する
	if (isDeleted) {
		RebuildTagCache3D();
	}
}

void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
	mTagCache2D[object2D->GetTag()].push_back(object2D);
}

void ObjectManager::DeleteAll2D()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		Object2D* temp = *itr;
		itr = mObject2DList.erase(itr);
		delete temp;
	}
	mTagCache2D.clear();
}

void ObjectManager::DeleteAll2DIfNeeded()
{
	bool isDeleted = false;
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object2D* temp = *itr;
			itr = mObject2DList.erase(itr);
			delete temp;
			isDeleted = true;
		}
		else
		{
			itr++;
		}
	}
	// リスト要素に変更があった場合のみ、検索高速化用キャッシュを再構築する
	if (isDeleted) {
		RebuildTagCache2D();
	}
}

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

const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	return mTagCache2D[tag];
}

void ObjectManager::RebuildTagCache3D()
{
	mTagCache3D.clear();
	for (auto obj : mObject3DList) {
		mTagCache3D[obj->GetTag()].push_back(obj);
	}
}

void ObjectManager::RebuildTagCache2D()
{
	mTagCache2D.clear();
	for (auto obj : mObject2DList) {
		mTagCache2D[obj->GetTag()].push_back(obj);
	}
}
