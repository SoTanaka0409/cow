#pragma once
#include "Collider.h"

// 繧ｫ繝励そ繝ｫ蠖｢迥ｶ・育ｷ壼・・句濠蠕・ｼ峨・蠖薙◆繧雁愛螳壹さ繝ｩ繧､繝繝ｼ繧ｯ繝ｩ繧ｹ
class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(Object3D* parent, VECTOR pos1, VECTOR pos2, float radius);
	virtual ~CapsuleCollider();

	/*
	 * @brief ・医ョ繝舌ャ繧ｰ陦ｨ遉ｺ逕ｨ・峨き繝励そ繝ｫ縺ｮ3D繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝繧呈緒逕ｻ縺吶ｋ・育樟蝨ｨ蜃ｦ逅・↑縺暦ｼ・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	void Draw() override;

	void OnEnter() override;
	void OnTrigger() override;
	void OnExit() override;
};

