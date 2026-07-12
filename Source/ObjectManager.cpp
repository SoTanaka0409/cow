#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

ObjectManager::ObjectManager()
{
	object3d_list_.reserve(1000);
	object2d_list_.reserve(100);
}

ObjectManager::~ObjectManager()
{
	// unique_ptr will automatically clean up memory
}

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
		VECTOR cameraPos = Master::camera_->GetPosition();
		VECTOR objPos = obj->GetPosition();
		obj->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	DeleteAll3DIfNeeded();
	DeleteAll2DIfNeeded();
}

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
}

void ObjectManager::AddObject(Object3D* object3D)
{
	object3d_list_.emplace_back(object3D);
	tag_cache_3d_[object3D->GetTag()].push_back(object3D);
}

void ObjectManager::RemoveObjectNoDelete(Object3D* object3D)
{
	auto itr = std::find_if(object3d_list_.begin(), object3d_list_.end(), [object3D](const std::unique_ptr<Object3D>& ptr) {
		return ptr.get() == object3D;
	});
	if (itr != object3d_list_.end())
	{
		// To remove without deleting, we would release the pointer, but here it's expected to delete or maybe keep it?
		// Actually, RemoveObjectNoDelete implies someone else will delete it.
		itr->release(); 
		object3d_list_.erase(itr);
	}
	RebuildTagCache3D();
}

void ObjectManager::DeleteAll3D()
{
	object3d_list_.clear();
	tag_cache_3d_.clear();
}

void ObjectManager::DeleteAll3DIfNeeded()
{
	bool isDeleted = false;
	if (object3d_list_.empty()) return;

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

const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	return tag_cache_3d_[tag];
}

void ObjectManager::AddObject(Object2D* object2D)
{
	object2d_list_.emplace_back(object2D);
	tag_cache_2d_[object2D->GetTag()].push_back(object2D);
}

void ObjectManager::DeleteAll2D()
{
	object2d_list_.clear();
	tag_cache_2d_.clear();
}

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

const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	return tag_cache_2d_[tag];
}

void ObjectManager::RebuildTagCache3D()
{
	tag_cache_3d_.clear();
	for (auto& obj : object3d_list_)
	{
		tag_cache_3d_[obj->GetTag()].push_back(obj.get());
	}
}

void ObjectManager::RebuildTagCache2D()
{
	tag_cache_2d_.clear();
	for (auto& obj : object2d_list_)
	{
		tag_cache_2d_[obj->GetTag()].push_back(obj.get());
	}
}