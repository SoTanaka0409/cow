#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

// Effekseer繧ｨ繝輔ぉ繧ｯ繝医お繝ｳ繧ｸ繝ｳ縺ｮ繧ｰ繝ｭ繝ｼ繝舌Ν縺ｪ蛻晄悄蛹悶∵ｯ弱ヵ繝ｬ繝ｼ繝縺ｮ譖ｴ譁ｰ繝ｻ謠冗判繧ｵ繧､繧ｯ繝ｫ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
class EffectManager
{
public:
	/*
	 * @brief 繝｡繝ｳ繝仙､画焚繧貞・譛溷喧縺励・ffekseer蛻晄悄蛹悶Γ繧ｽ繝・ラ繧貞他縺ｳ蜃ｺ縺・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	EffectManager();
	~EffectManager();

	/*
	 * @brief Direct3D險ｭ螳壹∵怙螟ｧ繝代・繝・ぅ繧ｯ繝ｫ謨ｰ險ｭ螳壹√せ繧ｯ繝ｪ繝ｼ繝ｳ繝ｪ繧ｻ繝・ヨ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ遲峨ｒ逋ｻ骭ｲ縺励※Effekseer繧定ｵｷ蜍輔☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] Effekseer繝ｩ繧､繝悶Λ繝ｪ縺ｮ繧ｰ繝ｭ繝ｼ繝舌Ν縺ｪ蛻晄悄蛹・
	 */
	void Initalize();

	/*
	 * @brief Effekseer蜈ｨ菴薙・豈弱ヵ繝ｬ繝ｼ繝縺ｮ3D繧ｨ繝輔ぉ繧ｯ繝亥・逕溽憾諷九ｒ譖ｴ譁ｰ縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜀咲函荳ｭ縺ｮ蜈ｨEffekseer繧ｨ繝輔ぉ繧ｯ繝医・騾ｲ陦梧峩譁ｰ
	 */
	void Update();

	/*
	 * @brief 蜀咲函荳ｭ縺ｮ縺吶∋縺ｦ縺ｮEffekseer繧ｨ繝輔ぉ繧ｯ繝医ｒ3D遨ｺ髢謎ｸ翫↓繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	void Draw();

private:
	VECTOR play_pos_; // 繝・せ繝亥・逕溽畑縺ｮ蠎ｧ讓呻ｼ域悴菴ｿ逕ｨ・・

	// 繝代Λ繝｡繝ｼ繧ｿ螳壽焚
	const int kEffectParticleLimit = 20000;  // 逕ｻ髱｢荳翫↓蜷梧凾縺ｫ陦ｨ遉ｺ蜿ｯ閭ｽ縺ｪ譛螟ｧ繝代・繝・ぅ繧ｯ繝ｫ謨ｰ
	const float kEffectSize = 1.0f;          // 蝓ｺ貅悶お繝輔ぉ繧ｯ繝医し繧､繧ｺ
	const int kEffectPlayerInterval = 300;   // 繧ｨ繝輔ぉ繧ｯ繝亥・逕溘・髢馴囈繝輔Ξ繝ｼ繝謨ｰ
	const float kEffectMoveSpeed = 0.2f;     // 繧ｨ繝輔ぉ繧ｯ繝医・遘ｻ蜍暮溷ｺｦ險ｭ螳・

	// 繧ｨ繝輔ぉ繧ｯ繝亥・逕溽ｮ｡逅・ワ繝ｳ繝峨Ν
	int effect_resource_handle_;               // 繝ｭ繝ｼ繝峨＆繧後◆繧ｨ繝輔ぉ繧ｯ繝医・繝ｪ繧ｽ繝ｼ繧ｹ繝上Φ繝峨Ν・域悴菴ｿ逕ｨ・・
	int playing_effect_handle_;                // 蜀咲函荳ｭ縺ｮ繧ｨ繝輔ぉ繧ｯ繝医う繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繝上Φ繝峨Ν・域悴菴ｿ逕ｨ・・
};
