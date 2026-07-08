#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

// 2D逕ｻ蜒上い繧ｻ繝・ヨ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲∽ｽ咲ｽｮ繧・し繧､繧ｺ縲√せ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ繧呈欠螳壹＠縺ｦ逕ｻ髱｢謠冗判繧定｡後≧繧ｯ繝ｩ繧ｹ
class Texture
{
public:
	Texture(std::string filename, VECTOR centerPosition, int graphsize_x, int graphsize_y, int transFlag);
	~Texture();

	/*
	 * @brief 繝・け繧ｹ繝√Ε繧・position_ 繧剃ｸｭ蠢・↓謖・ｮ壹し繧､繧ｺ縺ｧ謠冗判縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢・医ヰ繝・け繝舌ャ繝輔ぃ・峨↓逕ｻ蜒上ｒ謠冗判
	 */
	void Draw();

	/*
	 * @brief 繝・け繧ｹ繝√Ε繧帝壼ｸｸ繧ｵ繧､繧ｺ繧医ｊ蟆代＠諡｡螟ｧ縺励◆迥ｶ諷九〒荳ｭ蠢・緒逕ｻ縺吶ｋ・医・繧ｿ繝ｳ繝帙ヰ繝ｼ逕ｨ縺ｪ縺ｩ縲∫樟蝨ｨ譛ｪ菴ｿ逕ｨ・・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢縺ｫ諡｡螟ｧ逕ｻ蜒上ｒ謠冗判
	 */
	void SizeDraw();

	/*
	 * @brief 繝・け繧ｹ繝√Ε縺ｮ豈弱ヵ繝ｬ繝ｼ繝縺ｮ譖ｴ譁ｰ蜃ｦ逅・ｼ育樟蝨ｨ蜃ｦ逅・↑縺暦ｼ・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	void Update();

	/*
	 * @brief 繝・け繧ｹ繝√Ε繧剃ｻｻ諢上・繧ｹ繧ｱ繝ｼ繝ｫ豈皮紫縺ｧ荳ｭ蠢・緒逕ｻ縺吶ｋ
	 * [蜈･蜉嫋 scale: 繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ蛟咲紫・・.0f縺檎ｭ牙搾ｼ・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢縺ｫ諡｡螟ｧ/邵ｮ蟆上＠縺溽判蜒上ｒ謠冗判
	 */
	void DrawScale(float scale);

	// 繧ｻ繝・ち繝ｼ繝ｻ繧ｲ繝・ち繝ｼ鄒､
	void SetPosition(VECTOR centerPosition) { position_ = centerPosition; }
	VECTOR GetPosition() { return position_; }
	int GetSizeX() { return mnSizeX; }
	int GetSizeY() { return mnSizeY; }
	int GetWidth() { return mNewGameW; }
	int GetHeight() { return mNewGameH; }

private:
	int mnHandle;       // 繝ｭ繝ｼ繝峨＠縺溽判蜒上・繧ｰ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν
	VECTOR position_;  // 逕ｻ髱｢謠冗判譎ゅ・荳ｭ蠢・ｺｧ讓・
	int mnSizeX;        // 逕ｻ蜒上ヵ繧｡繧､繝ｫ譛ｬ譚･縺ｮ讓ｪ蟷・
	int mnSizeY;        // 逕ｻ蜒上ヵ繧｡繧､繝ｫ譛ｬ譚･縺ｮ邵ｦ蟷・
	bool mnTransFlag;   // 閭梧勹縺ｮ騾城℃・磯城℃繝√Ε繝ｳ繝阪Ν・峨ｒ譛牙柑縺ｫ縺吶ｋ縺九←縺・°縺ｮ繝輔Λ繧ｰ

	int mNewGameW;      // 謖・ｮ壹＆繧後◆謠冗判逕ｨ縺ｮ讓ｪ蟷・
	int mNewGameH;      // 謖・ｮ壹＆繧後◆謠冗判逕ｨ縺ｮ邵ｦ蟷・
};

#endif
