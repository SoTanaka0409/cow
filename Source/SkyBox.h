#pragma once
class Model;

#include "Object3D.h"

// 閭梧勹縺ｮ遨ｺ・医せ繧ｫ繧､繝懊ャ繧ｯ繧ｹ繝｢繝・Ν・峨ｒ邂｡逅・・謠冗判縺吶ｋ繧ｯ繝ｩ繧ｹ
class SkyBox : public Object3D
{
public:
	/*
	 * @brief 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ逕ｨ縺ｮ繝｢繝・Ν縺ｨ蛻晄悄蠎ｧ讓吶ｒ謖・ｮ壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
	 * [蜈･蜉嫋 filename: 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ縺ｮ繝｢繝・Ν繝輔ぃ繧､繝ｫ繝代せ, pos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ逕ｨModel繧ｯ繝ｩ繧ｹ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	 */
	SkyBox(std::string filename, VECTOR pos);
	virtual ~SkyBox();

	/*
	 * @brief 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ縺ｮ繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝譖ｴ譁ｰ蜃ｦ逅・ｒ陦後≧
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜀・Κ繝｢繝・Ν縺ｮ菴咲ｽｮ諠・ｱ縺ｮ譖ｴ譁ｰ
	 */
	void Update() override;

	/*
	 * @brief 繝ｩ繧､繝・ぅ繝ｳ繧ｰ險育ｮ励ｒ荳譎ら噪縺ｫ辟｡蜉ｹ蛹悶＠縲∫ｩｺ縺ｮ繝｢繝・Ν繧呈・繧九￥謠冗判縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Draw() override;

	/*
	 * @brief 遨ｺ繝｢繝・Ν蜈ｨ菴薙・陦ｨ遉ｺ繧ｹ繧ｱ繝ｼ繝ｫ・亥､ｧ縺阪＆・峨ｒ險ｭ螳壹☆繧・
	 * [蜈･蜉嫋 scale: 諡｡螟ｧ邇・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void SetScale(float scale);

	/*
	 * @brief 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ繝｢繝・Ν縺ｮ繝・け繧ｹ繝√Ε繧貞・繧頑崛縺医ｋ
	 * [蜈･蜉嫋 filename: 繝・け繧ｹ繝√Ε逕ｻ蜒上ヱ繧ｹ, index: 繝槭ユ繝ｪ繧｢繝ｫ繧､繝ｳ繝・ャ繧ｯ繧ｹ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void SetModelTexture(std::string filename, int index = 0);

private:
	Model* model_;          // 繧ｹ繧ｫ繧､繝懊ャ繧ｯ繧ｹ繧呈ｧ区・縺吶ｋ3D繝｢繝・Ν縺ｸ縺ｮ繝昴う繝ｳ繧ｿ
};
