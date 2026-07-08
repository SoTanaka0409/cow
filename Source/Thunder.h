#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "EffekseerEffect.h"

class SphereCollider;
class CapsuleCollider;

// 繝励Ξ繧､繝､繝ｼ縺ｮ遘ｻ蜍輔ｒ蛻ｶ髯舌☆繧九◆繧√√Λ繝ｳ繝繝菴咲ｽｮ縺ｫ關ｽ髮ｷ繧堤匱逕溘＆縺帙ｋ繝医Λ繝・・
class Thunder : public Object3D
{
public:
	enum State
	{
		IDLE,
		WARNING,
		STRIKE,
		END
	};

public:
	/*
	 * @brief 關ｽ髮ｷ繝医Λ繝・・繧貞・譛滉ｽ咲ｽｮ縺ｧ逕滓・縺励∝推遞ｮ繧ｨ繝輔ぉ繧ｯ繝医・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧剃ｽ懈・縺吶ｋ
	 * [蜈･蜉嫋 pos: 逕滓・蠎ｧ讓呻ｼ亥・譛溽憾諷九・髱槭い繧ｯ繝・ぅ繝厄ｼ・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜷・ｨｮEffekseerEffect繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮnew
	 */
	Thunder(VECTOR pos);

	/*
	 * @brief 繝ｭ繝ｼ繝峨＠縺溘お繝輔ぉ繧ｯ繝郁ｳ・ｺ舌ｒ螳牙・縺ｫ隗｣謾ｾ縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｨ繝輔ぉ繧ｯ繝郁ｳ・ｺ舌・遐ｴ譽・
	 */
	virtual ~Thunder() override;

	/*
	 * @brief 迥ｶ諷九・繧ｷ繝ｳ縺ｮ譖ｴ譁ｰ縲∝ｺｧ讓咏ｧｻ蜍輔√・繝ｬ繧､繝､繝ｼ縺ｮ陦晉ｪ∵､懃衍縲√♀繧医・繧ｹ繧ｿ繝ｳ逕ｨ繧ｨ繝輔ぉ繧ｯ繝医・騾ｲ陦梧峩譁ｰ繧定｡後≧
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 關ｽ髮ｷ蠎ｧ讓吶・繝ｩ繝ｳ繝繝譖ｴ譁ｰ縲∫憾諷句､画峩縲√お繝輔ぉ繧ｯ繝亥・逕・
	 */
	void Update() override;

	/*
	 * @brief 關ｽ髮ｷ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蜿ｯ隕門喧繝・ヰ繝・げ謠冗判・育樟蝨ｨ縺ｯ繧ｳ繝｡繝ｳ繝医い繧ｦ繝茨ｼ・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	void Draw() override;

	/*
	 * @brief 關ｽ髮ｷ縺ｮ遞ｼ蜒咲憾諷九ｒ蜿門ｾ励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 遞ｼ蜒堺ｸｭ縺ｪ繧液rue
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	bool IsActive() const;

	/*
	 * @brief 謖・ｮ壻ｽ咲ｽｮ縺ｫ蟇ｾ縺吶ｋ蠖薙◆繧雁愛螳壹ｒ陦後≧・育樟蝨ｨ縺ｯ譛ｪ菴ｿ逕ｨ縲＾nEnter縺ｧ蜃ｦ逅・ｼ・
	 * [蜈･蜉嫋 playerPos: 繝励Ξ繧､繝､繝ｼ蠎ｧ讓・ range: 蛻､螳壼濠蠕・
	 * [蜃ｺ蜉嫋 繝偵ャ繝医＠縺溷ｴ蜷医・true
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	bool CheckHit(VECTOR playerPos, float range);

	/*
	 * @brief 關ｽ髮ｷ蛻､螳壹′逋ｺ逕溘＠縺ｦ縺・ｋ迸ｬ髢薙↓繝励Ξ繧､繝､繝ｼ縺ｨ謗･隗ｦ縺励◆蝣ｴ蜷医√・繝ｬ繧､繝､繝ｼ繧剃ｸ螳壽凾髢薙せ繧ｿ繝ｳ縺輔○繧・
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繝励Ξ繧､繝､繝ｼ縺ｮ繧ｹ繧ｿ繝ｳ驕ｩ逕ｨ縲√せ繧ｿ繝ｳ繧ｨ繝輔ぉ繧ｯ繝医・蜀咲函縲√き繝｡繝ｩ繧ｷ繧ｧ繧､繧ｯ
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	VECTOR pos_;
	int mWarningTimer;
	int mStrikeTimer;
	State mState;
	int mIntervalTimer;
	bool mActive;

public:
	EffekseerEffect* mpThunder;
	EffekseerEffect* mpWarning;
	EffekseerEffect* mpStun;

	// 螟壽ｮｵ繝偵ャ繝医↓繧医ｋ繧ｹ繧ｿ繝ｳ縺ｮ豌ｸ邯壼喧繧帝亟縺舌◆繧√・繝輔Λ繧ｰ
	bool mHasStunned;
	int mStunEffectTimer;
};