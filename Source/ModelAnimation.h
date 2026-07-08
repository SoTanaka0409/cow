#pragma once
#include <string>
#include <vector>
#include "ModelUtility.h"

// 3D繝｢繝・Ν・・V1蠖｢蠑擾ｼ峨・繝懊・繝ｳ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蛻ｶ蠕｡縺ｨ繝悶Ξ繝ｳ繝牙・逅・ｒ陦後≧繧ｯ繝ｩ繧ｹ
class ModelAnimation
{
public:
	/*
	 * @brief 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蛻ｶ蠕｡蟇ｾ雎｡縺ｮ繝｢繝・Ν繝上Φ繝峨Ν繧堤匳骭ｲ縺吶ｋ
	 * [蜈･蜉嫋 modelHandle: DX繝ｩ繧､繝悶Λ繝ｪ縺ｮ繝｢繝・Ν繝上Φ繝峨Ν
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜷・ｨｮ蜀・Κ迥ｶ諷句､画焚縺ｮ蛻晄悄蛹悶√ョ繝輔か繝ｫ繝医い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｸ縺ｮ蛻・ｊ譖ｿ縺・
	 */
	ModelAnimation(int modelHandle);
	~ModelAnimation();

	/*
	 * @brief 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譎る俣繧帝ｲ陦後＆縺帙∝・繧頑崛縺域凾縺ｮ繝悶Ξ繝ｳ繝芽｣憺俣繧呈峩譁ｰ縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] DX繝ｩ繧､繝悶Λ繝ｪ縺ｮ繝｢繝・Ν繝上Φ繝峨Ν縺ｫ蟇ｾ縺励※迴ｾ蝨ｨ縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ迥ｶ諷九・譎ょ綾繧貞渚譏
	 */
	void Update();

	/*
	 * @brief 蜀咲函縺吶ｋ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧貞・繧頑崛縺医ｋ
	 * [蜈･蜉嫋 state: 蛻・ｊ譖ｿ縺亥・縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ迥ｶ諷・ index: 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ逡ｪ蜿ｷ
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜑阪・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｨ縺ｮ繝悶Ξ繝ｳ繝牙・逅・′髢句ｧ九＆繧後ｋ
	 */
	void ChangeAnimation(AnimationState state, int index = 1);

	void SetLoop(bool isLoop) { mbLoop = isLoop; }
	void SetLoopFinishState(AnimationState state) { mnLoopFinishState = state; }
	void SetAnimationBlend(bool isBlend);

	AnimationState GetNowState() { return mnState; }
	bool IsLoopFinish() { return mbLoopFinish; }

	void SetAnimationCount(float count) { mfAnimationCount = count; }

private:
	int mnModelHandle;                  // DX繝ｩ繧､繝悶Λ繝ｪ縺ｮ3D繝｢繝・Ν繝上Φ繝峨Ν
	float mfAnimationTime;              // 迴ｾ蝨ｨ蜀咲函荳ｭ縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ譎ょ綾・育ｧ抵ｼ・
	float mfAnimationCount;             // 蜀咲函譎る俣繧帝ｲ繧√ｋ騾溷ｺｦ菫よ焚
	int mnAnimationIndex;               // 迴ｾ蝨ｨ縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ
	float mfOldAnimationTime;           // 繝悶Ξ繝ｳ繝臥ｧｻ陦悟燕縺ｮ蜿､縺・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譎ょ綾・育ｧ抵ｼ・
	int mnOldAnimationIndex;            // 蜿､縺・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ
	float mfAnimBlendRate;              // 譁ｰ譌ｧ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝悶Ξ繝ｳ繝画ｯ皮紫 (0.0?1.0)
	AnimationState mnState;             // 迴ｾ蝨ｨ縺ｮ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ迥ｶ諷・
	bool mbLoop;                        // 繝ｫ繝ｼ繝怜・逕溘☆繧九°縺ｩ縺・°
	AnimationState mnLoopFinishState;   // 髱槭Ν繝ｼ繝励い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ邨ゆｺ・ｾ後↓驕ｷ遘ｻ縺吶ｋ迥ｶ諷・
	bool mbLoopFinish;                  // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺・蜻ｨ螳御ｺ・＠縺溘°縺ｩ縺・°
};

