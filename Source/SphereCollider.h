#pragma once
#include "Collider.h"

// 逅・ｽ灘ｽ｢迥ｶ・井ｸｭ必要せ縺ｨ蜊雁ｾ・ｼ峨・蠖薙◆繧雁愛螳壹さ繝ｩ繧､繝繝ｼ繧ｯ繝ｩ繧ｹ
class SphereCollider : public Collider
{
public:
	/*
	 * @brief 逅・ｽ薙さ繝ｩ繧､繝繝ｼ縺ｮ蛻晄悄險ｭ螳壹ｒ陦後≧
	 * [蜈･蜉嫋 parent: 隕ｪ繧ｪ繝悶ず繧ｧ繧ｯ繝・ center: 荳ｭ必要ｺｧ讓・ radius: 蜊雁ｾ・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	SphereCollider(Object3D* parent, VECTOR center, float radius);
	virtual ~SphereCollider();

	/*
	 * @brief 逅・・3D繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝繧呈緒逕ｻ縺吶ｋ・医ョ繝舌ャ繧ｰ陦ｨ遉ｺ逕ｨ・・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Draw() override;

	void OnEnter() override;
	void OnTrigger() override;
	void OnExit() override;

};

