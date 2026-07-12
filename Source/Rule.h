#pragma once
#include"Scene.h"

// 繝励Ξ繧､繝､繝ｼ迺ｰ蠅・↓蜷医ｏ縺帙◆髻ｳ驥剰ｪｿ謨ｴ繧呈署萓帙☆繧九◆繧√∬ｨｭ螳壹す繝ｼ繝ｳ繧貞・髮｢
class Rule : public Scene
{
public:
	Rule();
	~Rule();

	/*
	 * @brief 險ｭ螳夂判髱｢陦ｨ遉ｺ逕ｨ繝輔か繝ｳ繝医ｄ閭梧勹逕ｻ蜒上・隱ｭ縺ｿ霎ｼ縺ｿ縲・浹驥剰ｨｭ螳壹・隱ｭ縺ｿ霎ｼ縺ｿ繧定｡後≧
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 蜷・ｨｮ繝上Φ繝峨Ν蛻晄悄蛹悶∬ｨｭ螳夂判髱｢蟆ら畑BGM縺ｮ蜀咲函髢句ｧ・
	 */
	void Initialize() override;

	/*
	 * @brief 繧ｭ繝ｼ蜈･蜉幢ｼ井ｸ贋ｸ九く繝ｼ縺ｧ鬆・岼驕ｸ謚槭∝ｷｦ蜿ｳ繧ｭ繝ｼ縺ｧ髻ｳ驥丞､画峩・峨♀繧医・繝槭え繧ｹ謫堺ｽ懊・蜃ｦ逅・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 驕ｸ謚槭う繝ｳ繝・ャ繧ｯ繧ｹ縲∝渕貅夜浹驥剰ｨｭ螳・Master)縺ｮ譖ｴ譁ｰ縲。GM/SE縺ｮ蜊ｳ譎ょ渚譏
	 */
	void Update() override;

	/*
	 * @brief 閭梧勹縲√ち繧､繝医Ν繝ｭ繧ｴ縲∝推險ｭ螳壹ヰ繝ｼ縺翫ｈ縺ｳ髻ｳ驥乗焚蛟､縺ｮ謠冗判
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢謠冗判繧ｳ繝槭Φ繝峨・螳溯｡・
	 */
	void Draw() override;

	/*
	 * @brief 險ｭ螳夂判髱｢縺ｧ菴ｿ逕ｨ縺励◆逕ｻ蜒上・繝輔か繝ｳ繝医ワ繝ｳ繝峨Ν縺ｮ荳諡ｬ隗｣謾ｾ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧｢繧ｻ繝・ヨ縺ｮ繝｡繝｢繝ｪ隗｣謾ｾ縲。GM縺ｮ蛛懈ｭ｢
	 */
	void Finalize() override;

	enum MenuType
	{
		kMenuBgm,
		kMenuSe,
		kMenuBack,
		kMenuMax
	};

private:
	int rule_graph_;
	int font_handle_;
	int title_font_handle_;

	MenuType selected_index_ = kMenuBgm;
	// 髻ｳ驥上せ繝ｩ繧､繝繝ｼ謫堺ｽ懈凾縺ｫSE縺梧ｯ弱ヵ繝ｬ繝ｼ繝魑ｴ繧九・繧帝亟縺舌◆繧√・繧､繝ｳ繧ｿ繝ｼ繝舌Ν
	int play_se_delay_ = 0;
	// 繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ逶ｴ蠕後・蜈･蜉帶戟縺｡雜翫＠縺ｫ繧医ｋ隱､蜍穂ｽ懊ｒ髦ｲ縺舌◆繧・
	int scene_frames_ = 0;
};
