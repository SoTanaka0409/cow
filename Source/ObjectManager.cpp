#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

/*
 * @brief 繧ｪ繝悶ず繧ｧ繧ｯ繝育ｮ｡逅・け繝ｩ繧ｹ縺ｮ蛻晄悄蛹・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
ObjectManager::ObjectManager()
{
	mObject3DList.reserve(1000);
	mObject2DList.reserve(100);
}

/*
 * @brief 繧ｪ繝悶ず繧ｧ繧ｯ繝育ｮ｡逅・け繝ｩ繧ｹ縺ｮ邨ゆｺ・・逅・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
ObjectManager::~ObjectManager()
{
}

/*
 * @brief 逋ｻ骭ｲ縺輔ｌ縺溷・繧ｪ繝悶ず繧ｧ繧ｯ繝医・譖ｴ譁ｰ縺ｨ繧ｫ繝｡繝ｩ霍晞屬縺ｮ邂怜・繧定｡後≧
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] Z繧ｽ繝ｼ繝育畑繧ｫ繝｡繝ｩ霍晞屬縺ｮ譖ｴ譁ｰ縲√さ繝ｩ繧､繝繝ｼ縺ｮ荳諡ｬ蜃ｦ逅・ｮ溯｡・
 */
void ObjectManager::Update()
{
	for (auto obj : mObject3DList)
	{
		obj->Update();
	}

	for (auto obj : mObject2DList)
	{
		obj->Update();
	}

	// 蜊企乗・謠冗判譎ゅ・Z繧ｽ繝ｼ繝医〒蠢・ｦ√→縺ｪ繧九◆繧√√き繝｡繝ｩ縺ｨ縺ｮ霍晞屬繧堤ｮ怜・縺吶ｋ
	for (auto obj : mObject3DList)
	{
		VECTOR cameraPos = Master::mpCamera->GetPosition();
		VECTOR objPos = obj->GetPosition();
		obj->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// 蠎ｧ讓呎峩譁ｰ縺悟・縺ｦ螳御ｺ・＠縺溷ｾ後↓陦晉ｪ∝愛螳壹ｒ陦後≧縺溘ａ縺薙％縺ｧ荳諡ｬ蜃ｦ逅・☆繧・
	ColliderManager::GetInstance()->Update();
}

/*
 * @brief 逋ｻ骭ｲ縺輔ｌ縺溷・繧ｪ繝悶ず繧ｧ繧ｯ繝医・謠冗判繧定｡後≧
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 謠冗判繝輔Λ繧ｰ縺梧怏蜉ｹ縺ｪ繧ｪ繝悶ず繧ｧ繧ｯ繝医→繧ｳ繝ｩ繧､繝繝ｼ縺ｮ謠冗判蜻ｽ莉､逋ｺ陦・
 */
void ObjectManager::Draw()
{
	for (auto obj : mObject3DList)
	{
		if (obj->IsDrawFlag())
		{
			obj->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto obj : mObject2DList)
	{
		if (obj->IsDrawFlag())
		{
			obj->Draw();
		}
	}
}

/*
 * @brief 3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繝ｪ繧ｹ繝医→繧ｭ繝｣繝・す繝･縺ｫ霑ｽ蜉縺吶ｋ
 * [蜈･蜉嫋 object3D: 霑ｽ蜉縺吶ｋ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mObject3DList縺翫ｈ縺ｳmTagCache3D縺ｸ縺ｮ隕∫ｴ霑ｽ蜉
 */
void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);
	mTagCache3D[object3D->GetTag()].push_back(object3D);
}

/*
 * @brief 3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繝ｪ繧ｹ繝医°繧牙炎髯､縺吶ｋ・医Γ繝｢繝ｪ隗｣謾ｾ縺ｯ縺励↑縺・ｼ・
 * [蜈･蜉嫋 object3D: 蜑企勁蟇ｾ雎｡縺ｮ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mObject3DList縺九ｉ縺ｮ隕∫ｴ蜑企勁縺ｨ繧ｭ繝｣繝・す繝･縺ｮ蜀肴ｧ狗ｯ・
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
 * @brief 蜈ｨ縺ｦ縺ｮ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医↓蜑企勁繝輔Λ繧ｰ繧堤ｫ九※縲∝ｮ滄圀縺ｮ蜑企勁蜃ｦ逅・ｒ蜻ｼ縺ｶ
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 蜈ｨ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・蜑企勁縺ｨ繝｡繝｢繝ｪ隗｣謾ｾ
 */
void ObjectManager::DeleteAll3D()
{
	if (mObject3DList.empty()) return;
	for (auto obj : mObject3DList)
	{
		obj->SetDeleteFlag(true);
	}

	DeleteAll3DIfNeeded();
}

/*
 * @brief 謖・ｮ壹＆繧後◆繧ｿ繧ｰ繧呈戟縺､3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ1縺､蜿門ｾ励☆繧・
 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ繧ｿ繧ｰ
 * [蜃ｺ蜉嫋 譚｡莉ｶ縺ｫ蜷郁・縺吶ｋ譛蛻昴・Object3D繝昴う繝ｳ繧ｿ・郁ｦ九▽縺九ｉ縺ｪ縺・ｴ蜷医・nullptr・・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
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
 * @brief 謖・ｮ壹＆繧後◆繧ｿ繧ｰ繧呈戟縺､3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝医ｒ蜿門ｾ励☆繧・
 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ繧ｿ繧ｰ
 * [蜃ｺ蜉嫋 譚｡莉ｶ縺ｫ蜷郁・縺吶ｋObject3D繝昴う繝ｳ繧ｿ縺ｮ繝吶け繧ｿ・医く繝｣繝・す繝･縺九ｉ蜿ら・・・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	return mTagCache3D[tag];
}

/*
 * @brief 蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺ｦ縺・ｋ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繝｡繝｢繝ｪ隗｣謾ｾ縺励√Μ繧ｹ繝医°繧蛾勁螟悶☆繧・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 荳崎ｦ√が繝悶ず繧ｧ繧ｯ繝医・繝｡繝｢繝ｪ隗｣謾ｾ縺ｨ繧ｭ繝｣繝・す繝･縺ｮ蜀肴ｧ狗ｯ・
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

	// 豈弱ヵ繝ｬ繝ｼ繝縺ｮ繧ｭ繝｣繝・す繝･讒狗ｯ芽ｲ闕ｷ繧帝∩縺代ｋ縺溘ａ縲∬ｦ∫ｴ螟画峩譎ゅ・縺ｿ蜀肴ｧ狗ｯ峨☆繧・
	if (isDeleted) {
		RebuildTagCache3D();
	}
}

/*
 * @brief 2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繝ｪ繧ｹ繝医→繧ｭ繝｣繝・す繝･縺ｫ霑ｽ蜉縺吶ｋ
 * [蜈･蜉嫋 object2D: 霑ｽ蜉縺吶ｋ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mObject2DList縺翫ｈ縺ｳmTagCache2D縺ｸ縺ｮ隕∫ｴ霑ｽ蜉
 */
void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
	mTagCache2D[object2D->GetTag()].push_back(object2D);
}

/*
 * @brief 蜈ｨ縺ｦ縺ｮ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繝ｪ繧ｹ繝医°繧牙炎髯､縺励√Γ繝｢繝ｪ隗｣謾ｾ縺吶ｋ
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 蜈ｨ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・蜑企勁縺ｨ繧ｭ繝｣繝・す繝･縺ｮ繧ｯ繝ｪ繧｢
 */
void ObjectManager::DeleteAll2D()
{
	for (auto obj : mObject2DList)
	{
		delete obj;
	}
	mObject2DList.clear();
	mTagCache2D.clear();
}

/*
 * @brief 蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺ｦ縺・ｋ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繝｡繝｢繝ｪ隗｣謾ｾ縺励√Μ繧ｹ繝医°繧蛾勁螟悶☆繧・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 荳崎ｦ√が繝悶ず繧ｧ繧ｯ繝医・繝｡繝｢繝ｪ隗｣謾ｾ縺ｨ繧ｭ繝｣繝・す繝･縺ｮ蜀肴ｧ狗ｯ・
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

	// 豈弱ヵ繝ｬ繝ｼ繝縺ｮ繧ｭ繝｣繝・す繝･讒狗ｯ芽ｲ闕ｷ繧帝∩縺代ｋ縺溘ａ縲∬ｦ∫ｴ螟画峩譎ゅ・縺ｿ蜀肴ｧ狗ｯ峨☆繧・
	if (isDeleted) {
		RebuildTagCache2D();
	}
}

/*
 * @brief 謖・ｮ壹＆繧後◆繧ｿ繧ｰ繧呈戟縺､2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ1縺､蜿門ｾ励☆繧・
 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ繧ｿ繧ｰ
 * [蜃ｺ蜉嫋 譚｡莉ｶ縺ｫ蜷郁・縺吶ｋ譛蛻昴・Object2D繝昴う繝ｳ繧ｿ・郁ｦ九▽縺九ｉ縺ｪ縺・ｴ蜷医・nullptr・・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
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
 * @brief 謖・ｮ壹＆繧後◆繧ｿ繧ｰ繧呈戟縺､2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝医ｒ蜿門ｾ励☆繧・
 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ繧ｿ繧ｰ
 * [蜃ｺ蜉嫋 譚｡莉ｶ縺ｫ蜷郁・縺吶ｋObject2D繝昴う繝ｳ繧ｿ縺ｮ繝吶け繧ｿ・医く繝｣繝・す繝･縺九ｉ蜿ら・・・
 * [蜑ｯ菴懃畑] 縺ｪ縺・
 */
const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	return mTagCache2D[tag];
}

/*
 * @brief 3D繧ｪ繝悶ず繧ｧ繧ｯ繝域､懃ｴ｢逕ｨ縺ｮ繧ｿ繧ｰ繧ｭ繝｣繝・す繝･繧貞・讒狗ｯ峨☆繧・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mTagCache3D縺ｮ繧ｯ繝ｪ繧｢縺ｨ隕∫ｴ縺ｮ蜀咲匳骭ｲ
 */
void ObjectManager::RebuildTagCache3D()
{
	mTagCache3D.clear();
	for (auto obj : mObject3DList) {
		mTagCache3D[obj->GetTag()].push_back(obj);
	}
}

/*
 * @brief 2D繧ｪ繝悶ず繧ｧ繧ｯ繝域､懃ｴ｢逕ｨ縺ｮ繧ｿ繧ｰ繧ｭ繝｣繝・す繝･繧貞・讒狗ｯ峨☆繧・
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mTagCache2D縺ｮ繧ｯ繝ｪ繧｢縺ｨ隕∫ｴ縺ｮ蜀咲匳骭ｲ
 */
void ObjectManager::RebuildTagCache2D()
{
	mTagCache2D.clear();
	for (auto obj : mObject2DList) {
		mTagCache2D[obj->GetTag()].push_back(obj);
	}
}
