#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

// 繧ｲ繝ｼ繝蜀・・3D蠖薙◆繧雁愛螳壹が繝悶ず繧ｧ繧ｯ繝医・蝓ｺ蠎輔さ繝ｩ繧､繝繝ｼ繧ｯ繝ｩ繧ｹ
class Collider
{
public:
	Collider(Object3D* parent);
	virtual ~Collider();

	/*
	 * @brief 蛻･縺ｮ繧ｳ繝ｩ繧､繝繝ｼ縺ｨ縺ｮ陦晉ｪ∫憾諷九ｒ蛻､螳壹＠縲＾nEnter/OnTrigger/OnExit繧､繝吶Φ繝医ｒ逋ｺ陦後☆繧・
	 * [蜈･蜉嫋 check: 蛻､螳壼ｯｾ雎｡縺ｮ蛻･繧ｳ繝ｩ繧､繝繝ｼ, isHit: 陦晉ｪ√＠縺溘°縺ｩ縺・°縺ｮ邨先棡
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mCollisionList縺ｮ譖ｴ譁ｰ縲∬ｦｪ繧ｲ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ陦晉ｪ√さ繝ｼ繝ｫ繝舌ャ繧ｯ騾夂衍
	 */
	void HitCheck(Collider* check, bool isHit);

	virtual void Draw();

	virtual void OnEnter();
	virtual void OnTrigger();
	virtual void OnExit();

	// 繧ｲ繝・ち繝ｼ繝ｻ繧ｻ繝・ち繝ｼ鄒､
	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }
	Object3D* GetParentObject() { return parent_object_; }
	void InvalidateParent() { parent_object_ = nullptr; }

public:
	Object3D* parent_object_;      // 縺薙・繧ｳ繝ｩ繧､繝繝ｼ繧呈園譛峨☆繧玖ｦｪ3D繧ｪ繝悶ず繧ｧ繧ｯ繝・

	VECTOR position_;             // 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蝓ｺ貅紋ｽ咲ｽｮ蠎ｧ讓・・育帥縺ｮ荳ｭ蠢・√∪縺溘・繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ・・
	VECTOR position2_;            // 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蝓ｺ貅紋ｽ咲ｽｮ蠎ｧ讓・・医き繝励そ繝ｫ縺ｮ邨らせ・・
	float radius_;                // 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蜊雁ｾ・

	bool delete_flag_;             // 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蜑企勁繝輔Λ繧ｰ・・rue縺ｧ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺九ｉ髯､螟厄ｼ・

protected:
	std::vector<Collider*> collision_list_; // 迴ｾ蝨ｨ縺薙・繧ｳ繝ｩ繧､繝繝ｼ縺ｫ陦晉ｪ√＠邯壹￠縺ｦ縺・ｋ莉悶・繧ｳ繝ｩ繧､繝繝ｼ縺ｮ繝ｪ繧ｹ繝・
};