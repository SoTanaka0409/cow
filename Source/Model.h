#pragma once
#include "DxLib.h"
#include <string>

// 3D繝｢繝・Ν・・V1蠖｢蠑擾ｼ峨・隱ｭ縺ｿ霎ｼ縺ｿ縲∵緒逕ｻ縲∝ｺｧ讓吶・繧ｹ繧ｱ繝ｼ繝ｫ・繝・け繧ｹ繝√Ε蛻ｶ蠕｡繧偵き繝励そ繝ｫ蛹悶☆繧九け繝ｩ繧ｹ
class Model
{
public:
	/*
	 * @brief 3D繝｢繝・Ν繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲∝・譛溷ｺｧ讓吶〒繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ, initPos: 蛻晄悄蠎ｧ讓・ isSeparateAnimation: 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蛻・屬繝輔Λ繧ｰ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繝ｪ繧ｽ繝ｼ繧ｹ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ邨檎罰縺ｧ繝｢繝・Ν繝上Φ繝峨Ν縺檎｢ｺ菫昴＆繧後ｋ
	 */
	Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);
	~Model();   

	/*
	 * @brief 繝｢繝・Ν縺ｮ繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝諠・ｱ繧奪X繝ｩ繧､繝悶Λ繝ｪ縺ｮ繝｢繝・Ν繝上Φ繝峨Ν縺ｫ驕ｩ逕ｨ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Update();  

	/*
	 * @brief 3D繝｢繝・Ν繧呈緒逕ｻ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 3D遨ｺ髢薙∈縺ｮ謠冗判縺瑚｡後ｏ繧後ｋ
	 */
	void Draw();    

	VECTOR GetPosition() { return position_; } 
	void SetPosition(VECTOR pos) { position_ = pos; }  

	VECTOR GetRotation() { return rotation_; } 
	void SetRotation(VECTOR rot) { rotation_ = rot; }  

	/*
	 * @brief 繝｢繝・Ν縺ｮ諡｡螟ｧ邇・ｒ3霆ｸ蛟句挨縺ｫ險ｭ螳壹☆繧・
	 * [蜈･蜉嫋 scale: 場合ｻｸ縺ｮ諡｡螟ｧ邇・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繝｢繝・Ν縺ｮ陦ｨ遉ｺ繧ｵ繧､繧ｺ縺悟､牙喧縺吶ｋ
	 */
	void SetScale(VECTOR scale);

	/*
	 * @brief 繝｢繝・Ν縺ｮ諡｡螟ｧ邇・ｒ蝮・ｭ峨↓險ｭ螳壹☆繧・
	 * [蜈･蜉嫋 scale: 諡｡螟ｧ蛟咲紫
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void SetScale(float scale);

	/*
	 * @brief 繝｢繝・Ν縺ｮ繝・け繧ｹ繝√Ε逕ｻ蜒上ｒ蟾ｮ縺玲崛縺医ｋ
	 * [蜈･蜉嫋 filename: 逕ｻ蜒上・繝輔ぃ繧､繝ｫ繝代せ, index: 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ逡ｪ蜿ｷ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 譌｢蟄倥・繝・け繧ｹ繝√Ε繝上Φ繝峨Ν縺後≠繧後・隗｣謾ｾ縺輔ｌ縲∵眠繝・け繧ｹ繝√Ε縺悟牡繧雁ｽ薙※繧峨ｌ繧・
	 */
	void SetTexture(std::string filename, int index = 0);

	/*
	 * @brief 繝｢繝・Ν縺ｮ繝・ぅ繝輔Η繝ｼ繧ｺ・域僑謨｣・峨き繝ｩ繝ｼ繧定ｨｭ螳壹☆繧・
	 * [蜈･蜉嫋 r, g, b, a: 繧ｫ繝ｩ繝ｼ謌仙・(0.0?1.0)
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繝｢繝・Ν縺ｮ蜿榊ｰ・・縺ｮ濶ｲ縺悟､画峩縺輔ｌ繧・
	 */
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	int mnHandle;                 // DX繝ｩ繧､繝悶Λ繝ｪ縺ｮ3D繝｢繝・Ν繝上Φ繝峨Ν
	VECTOR position_;            // 驟咲ｽｮ蠎ｧ讓・
	VECTOR rotation_;            // 蝗櫁ｻ｢蛟､・医Λ繧ｸ繧｢繝ｳ・・
	float mfScale;                // 繧ｹ繧ｱ繝ｼ繝ｫ
	int mnChangeTextureHandle;    // 繝・け繧ｹ繝√Ε蟾ｮ縺玲崛縺域凾縺ｮ繧ｰ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν
};