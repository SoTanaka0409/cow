#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include <vector>

// 繧ｲ繝ｼ繝蜀・・螢・擇・亥屁隗貞ｽ｢繝昴Μ繧ｴ繝ｳ・峨ｒ逕滓・繝ｻ謠冗判縺吶ｋ繧ｯ繝ｩ繧ｹ
class Wall : public Object3D
{
public:
	/*
	 * @brief 螢・擇縺ｮ繝・け繧ｹ繝√Ε逕ｻ蜒上→遽・峇繝代Λ繝｡繝ｼ繧ｿ繧定ｨｭ螳壹＠縺ｦ蛻晄悄蛹悶☆繧・
	 * [蜈･蜉嫋 filename: 繝・け繧ｹ繝√Ε逕ｻ蜒上∈縺ｮ繝代せ, centerPos: 螢√・荳ｭ蠢・ｺｧ讓・ topLeft: 蟾ｦ荳翫・逶ｸ蟇ｾ蠎ｧ讓・ bottomRight: 蜿ｳ荳九・逶ｸ蟇ｾ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繝・け繧ｹ繝√Ε縺後Ο繝ｼ繝画凾縺ｫ繝ｭ繝ｼ繝峨＆繧後・らせ繝舌ャ繝輔ぃ(mVertex)縺瑚ｨｭ螳壹＆繧後ｋ
	 */
	Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);
	virtual ~Wall();

	void Update() override;
	void Draw() override;

	/*
	 * @brief 螢√ｒ讒区・縺吶ｋ4縺､縺ｮ鬆らせ繝・・繧ｿ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 VERTEX3D讒矩菴薙・繝吶け繧ｿ繝ｼ
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	std::vector<VERTEX3D> GetVertex();

private:
	int mnGraphHandle;      // 螢√ユ繧ｯ繧ｹ繝√Ε縺ｮ繧ｰ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν
	VERTEX3D mVertex[4];    // 螢・擇繧呈ｧ区・縺吶ｋ4縺､縺ｮ鬆らせ繝・・繧ｿ
};
