#pragma once

#include<vector>
#include<memory>
#include<map>
#include"Object3D.h"
#include"Object2D.h"

// 繧ｷ繝ｼ繝ｳ荳翫・蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ荳蜈・ｮ｡逅・＠縲√Λ繧､繝輔し繧､繧ｯ繝ｫ繧貞宛蠕｡縺吶ｋ繧ｯ繝ｩ繧ｹ
class ObjectManager
{
public:
	ObjectManager();

	~ObjectManager();

	/*
	 * 逋ｻ骭ｲ縺輔ｌ縺ｦ縺・ｋ縺吶∋縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ謠冗判縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢縺ｸ縺ｮ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ
	 */
	void Draw();

	/*
	 * 繧ｪ繝悶ず繧ｧ繧ｯ繝医・譖ｴ譁ｰ縺ｨ荳崎ｦ√↑繧ｪ繝悶ず繧ｧ繧ｯ繝医・遐ｴ譽・ｒ陦後≧
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｪ繝悶ず繧ｧ繧ｯ繝医・譖ｴ譁ｰ蜃ｦ逅・√Γ繝｢繝ｪ隗｣謾ｾ
	 */
	void Update();

public:

	/*
	 * 邂｡逅・Μ繧ｹ繝医↓3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑ｽ蜉縺吶ｋ
	 * [蜈･蜉嫋 object3D: 霑ｽ蜉縺吶ｋ3D繧ｪ繝悶ず繧ｧ繧ｯ繝・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mObject3DList縺ｸ縺ｮ霑ｽ蜉
	 */
	void AddObject(Object3D* object3D);

	/*
	 * 邂｡逅・Μ繧ｹ繝医°繧・D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ髯､螟悶☆繧具ｼ医Γ繝｢繝ｪ隗｣謾ｾ縺ｯ縺励↑縺・ｼ・
	 * [蜈･蜉嫋 object3D: 髯､螟悶☆繧・D繧ｪ繝悶ず繧ｧ繧ｯ繝・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mObject3DList縺九ｉ縺ｮ蜑企勁
	 */
	void RemoveObjectNoDelete(Object3D* object3D);

	/*
	 * 縺吶∋縺ｦ縺ｮ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蠑ｷ蛻ｶ逧・↓遐ｴ譽・・繝｡繝｢繝ｪ隗｣謾ｾ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜈ｨ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・delete縺ｨ繝ｪ繧ｹ繝医け繝ｪ繧｢
	 */
	void DeleteAll3D();

	/*
	 * 蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺・D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ螳牙・縺ｫ遐ｴ譽・＠縲√Γ繝｢繝ｪ繧定ｧ｣謾ｾ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 隧ｲ蠖薙が繝悶ず繧ｧ繧ｯ繝医・delete縲√Μ繧ｹ繝医°繧峨・髯､螟・
	 */
	void DeleteAll3DIfNeeded();

	/*
	 * 謖・ｮ壹ち繧ｰ繧呈戟縺､3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ1縺､蜿門ｾ励☆繧・
	 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ3D繧ｿ繧ｰ
	 * [蜃ｺ蜉嫋 隕九▽縺九▲縺・D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ
	 * [蜑ｯ菴懃畑] なし
	 */
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	/*
	 * 謖・ｮ壹ち繧ｰ繧呈戟縺､縺吶∋縺ｦ縺ｮ3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝医ｒ蜿門ｾ励☆繧・
	 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ3D繧ｿ繧ｰ
	 * [蜃ｺ蜉嫋 謖・ｮ壹ち繧ｰ繧呈戟縺､3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・驟榊・
	 * [蜑ｯ菴懃畑] なし
	 */
	const std::vector<Object3D*>& GetObject3DListByTag(Object3D::Tag3D tag);

	/*
	 * 邂｡逅・Μ繧ｹ繝医↓2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑ｽ蜉縺吶ｋ
	 * [蜈･蜉嫋 object2D: 霑ｽ蜉縺吶ｋ2D繧ｪ繝悶ず繧ｧ繧ｯ繝・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mObject2DList縺ｸ縺ｮ霑ｽ蜉
	 */
	void AddObject(Object2D* object2D);

	/*
	 * 縺吶∋縺ｦ縺ｮ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蠑ｷ蛻ｶ逧・↓遐ｴ譽・・繝｡繝｢繝ｪ隗｣謾ｾ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜈ｨ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・delete縺ｨ繝ｪ繧ｹ繝医け繝ｪ繧｢
	 */
	void DeleteAll2D();

	/*
	 * 蜑企勁繝輔Λ繧ｰ縺檎ｫ九▲縺・D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ螳牙・縺ｫ遐ｴ譽・＠縲√Γ繝｢繝ｪ繧定ｧ｣謾ｾ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 隧ｲ蠖薙が繝悶ず繧ｧ繧ｯ繝医・delete縲√Μ繧ｹ繝医°繧峨・髯､螟・
	 */
	void DeleteAll2DIfNeeded();

	/*
	 * 謖・ｮ壹ち繧ｰ繧呈戟縺､2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ1縺､蜿門ｾ励☆繧・
	 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ2D繧ｿ繧ｰ
	 * [蜃ｺ蜉嫋 隕九▽縺九▲縺・D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝昴う繝ｳ繧ｿ
	 * [蜑ｯ菴懃畑] なし
	 */
	Object2D* GetObject2DByTag(Object2D::Tag2D tag);

	/*
	 * 謖・ｮ壹ち繧ｰ繧呈戟縺､縺吶∋縺ｦ縺ｮ2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝医ｒ蜿門ｾ励☆繧・
	 * [蜈･蜉嫋 tag: 讀懃ｴ｢蟇ｾ雎｡縺ｮ2D繧ｿ繧ｰ
	 * [蜃ｺ蜉嫋 謖・ｮ壹ち繧ｰ繧呈戟縺､2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・驟榊・
	 * [蜑ｯ菴懃畑] なし
	 */
	const std::vector<Object2D*>& GetObject2DListByTag(Object2D::Tag2D tag);

	/*
	 * 3D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繧ｿ繧ｰ繧ｭ繝｣繝・す繝･繧貞・讒狗ｯ峨☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mTagCache3D縺ｮ譖ｴ譁ｰ
	 */
	void RebuildTagCache3D();

	/*
	 * 2D繧ｪ繝悶ず繧ｧ繧ｯ繝医・繧ｿ繧ｰ繧ｭ繝｣繝・す繝･繧貞・讒狗ｯ峨☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mTagCache2D縺ｮ譖ｴ譁ｰ
	 */
	void RebuildTagCache2D();

private:
	std::vector<std::unique_ptr<Object3D>> object3d_list_;
	std::map<Object3D::Tag3D, std::vector<Object3D*>> tag_cache_3d_;
	
	std::vector<std::unique_ptr<Object2D>> object2d_list_;
	std::map<Object2D::Tag2D, std::vector<Object2D*>> tag_cache_2d_;
};
