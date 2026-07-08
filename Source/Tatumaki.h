#pragma once
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

class Tatumaki : public Object3D
{
public:
	/*
	 * @brief 遶懷ｷｻ繧ｪ繝悶ず繧ｧ繧ｯ繝医・蛻晄悄蛹・
	 * [蜈･蜉嫋 pos: 遶懷ｷｻ縺ｮ蛻晄悄繧ｹ繝昴・繝ｳ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 3D繧ｨ繝輔ぉ繧ｯ繝・EffekseerEffect)縺ｮ繝ｪ繧ｽ繝ｼ繧ｹ繧偵Γ繝｢繝ｪ縺ｫ繝ｭ繝ｼ繝峨＠縲∝・譛溘せ繧ｱ繝ｼ繝ｫ繧定ｨｭ螳壹☆繧・
	 */
	Tatumaki(VECTOR pos);

	/*
	 * @brief 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜍慕噪遒ｺ菫昴＠縺檸ffekseerEffect縺ｮ遐ｴ譽・↓繧医ｋ繝｡繝｢繝ｪ隗｣謾ｾ繧定｡後≧
	 */
	virtual ~Tatumaki() override;

	/*
	 * @brief 繝輔Ξ繝ｼ繝豈弱・霑ｽ蠕薙・貍泌・譖ｴ譁ｰ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繝励Ξ繧､繝､繝ｼ縺ｸ縺ｮ繝帙・繝溘Φ繧ｰ遘ｻ蜍輔∵ｮｵ髫守噪縺ｪ繧ｵ繧､繧ｺ螟画峩(Lerp)縲∝所縺ｳ荳螳夐俣髫斐〒縺ｮSE蜀咲函繧定｡後≧
	 */
	void Update() override;

	/*
	 * @brief 謠冗判蜃ｦ逅・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 譛ｬ繧ｯ繝ｩ繧ｹ閾ｪ菴薙・謠冗判縺ｯ陦後ｏ縺ｪ縺・ｼ域緒逕ｻ縺ｯEffekseer蛛ｴ縺ｧ蜃ｦ逅・＆繧後ｋ縺溘ａ・・
	 */
	void Draw() override;

	/*
	 * @brief 蠖薙◆繧雁愛螳壹∈縺ｮ萓ｵ蜈･讀懃衍
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 謗･隗ｦ縺励◆逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繝励Ξ繧､繝､繝ｼ謗･隗ｦ譎ゅ∽ｻ墓ｧ倥↓蝓ｺ縺･縺阪・繝ｬ繧､繝､繝ｼ蠎ｧ讓吶ｒ蠑ｷ蛻ｶ逧・↓繝ｩ繝ｳ繝繝繝ｯ繝ｼ繝励＆縺帙√き繝｡繝ｩ繧呈険蜍輔＆縺帙ｋ
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;
	
	/*
	 * @brief 蠖薙◆繧雁愛螳壼・縺ｮ貊槫惠讀懃衍
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 謗･隗ｦ縺励◆逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	
	/*
	 * @brief 蠖薙◆繧雁愛螳壹°繧峨・騾蜃ｺ讀懃衍
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 謗･隗ｦ縺励◆逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * @brief 遶懷ｷｻ縺ｮ蟾ｨ螟ｧ蛹悶Δ繝ｼ繝芽ｨｭ螳・
	 * [蜈･蜉嫋 isCrisis: 蟾ｨ螟ｧ蛹也憾諷九↓縺吶ｋ蝣ｴ蜷医・true
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 迥ｶ諷九ヵ繝ｩ繧ｰ縺ｮ譖ｴ譁ｰ縲よｬ｡蝗槭・Update譎ゅ↓繧ｹ繧ｱ繝ｼ繝ｫ縺ｨ蠖薙◆繧雁愛螳壹・繧ｵ繧､繧ｺ縺梧僑蠑ｵ縺輔ｌ繧・
	 */
	void SetCrisisMode(bool isCrisis) { is_crisis_ = isCrisis; }

private:
	VECTOR pos_;
	VECTOR velocity_;
	int move_timer_;
	float speed_;
	EffekseerEffect* tatu_;

	bool is_crisis_;
	float current_scale_ratio_;
	float current_radius_;

	int effect_timer_;
	int tatumaki_spawn_timer_;
};