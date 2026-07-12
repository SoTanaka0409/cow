#pragma once
#include "Object3D.h"
#include <vector>
#include <string>

// 繧ｲ繝ｼ繝蜀・・蠎企擇・亥ｹｳ髱｢繝昴Μ繧ｴ繝ｳ・峨ｒ逕滓・・謠冗判縺吶ｋ繧ｯ繝ｩ繧ｹ
class Floor : public Object3D
{
public:
	/*
	 * @brief 蠎翫・繝ｪ繧ｴ繝ｳ縺ｮ繝・け繧ｹ繝√Ε逕ｻ蜒上→遽・峇繝代Λ繝｡繝ｼ繧ｿ繧定ｨｭ螳壹＠縺ｦ蛻晄悄蛹悶☆繧・
	 * [蜈･蜉嫋 filename: 繝・け繧ｹ繝√Ε逕ｻ蜒上∈縺ｮ繝代せ, centerPos: 蠎翫・蝓ｺ貅紋ｸｭ必要ｺｧ讓・ topLeft: 蟾ｦ荳顔ｫｯ縺ｮ逶ｸ蟇ｾ蠎ｧ讓・ bottomRight: 蜿ｳ荳狗ｫｯ縺ｮ逶ｸ蟇ｾ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繝・け繧ｹ繝√Ε縺後Γ繝｢繝ｪ縺ｫ繝ｭ繝ｼ繝峨＆繧後・らせ繝舌ャ繝輔ぃ(vertex_)縺瑚ｨｭ螳壹＆繧後ｋ
	 */
	Floor(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight);
	virtual ~Floor();

	void Update() override;
	void Draw() override;

private:
	int graph_handle_;      // 蠎翫・繝・け繧ｹ繝√Ε繧ｰ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν
	VERTEX3D vertex_[4];    // 蠎企擇繧呈ｧ区・縺吶ｋ4縺､縺ｮ鬆らせ繝・・繧ｿ
};

