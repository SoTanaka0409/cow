#pragma once
#include "Object3D.h"
#include <string>

// 閭梧勹謠冗判縺ｨ迚ｩ逅・愛螳壹ｒ蛻・屬縺励※譛驕ｩ蛹悶☆繧九◆繧√・縺､縺ｮ繝｢繝・Ν繧貞酔譛溽ｮ｡逅・☆繧・
class Stage : public Object3D
{
public:
	/*
	 * @brief 蝨ｰ蠖｢謠冗判繝｢繝・Ν縺ｨ陦晉ｪ∝愛螳夂畑繝｢繝・Ν繧偵◎繧後◇繧瑚ｪｭ縺ｿ霎ｼ繧薙〒驟咲ｽｮ縺吶ｋ
	 * [蜈･蜉嫋 initPos: 驟咲ｽｮ蛻晄悄蠎ｧ讓・ StageModelName: 蝨ｰ蠖｢謠冗判逕ｨ繝｢繝・Ν繝輔ぃ繧､繝ｫ繝代せ, stageCollsionModelName: 陦晉ｪ∝愛螳夂畑繝｢繝・Ν繝輔ぃ繧､繝ｫ繝代せ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 場合Δ繝・Ν縺ｮ繝ｭ繝ｼ繝峨√せ繧ｱ繝ｼ繝ｫ螟画峩縲√さ繝ｪ繧ｸ繝ｧ繝ｳ諠・ｱ縺ｮ讒狗ｯ峨′陦後ｏ繧後ｋ
	 */
	Stage(VECTOR initPos, std::string StageModelName, std::string stageCollsionModelName);
	virtual ~Stage();

	void Update() override;
	void Draw() override;

	/*
	 * @brief 蠑墓焚縺ｧ謖・ｮ壹＆繧後◆繧ｫ繝励そ繝ｫ鬆伜沺縺ｨ繧ｹ繝・・繧ｸ蝨ｰ蠖｢縺ｨ縺ｮ莠､蟾ｮ繧貞愛螳壹＠縲∬｡晉ｪ∫ｮ・園繧偵ョ繝舌ャ繧ｰ謠冗判縺吶ｋ
	 * [蜈･蜉嫋 pos1, pos2: 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ・邨らせ蠎ｧ讓・ r: 繧ｫ繝励そ繝ｫ縺ｮ蜊雁ｾ・
	 * [蜃ｺ蜉嫋 莠､蟾ｮ縺励◆蝣ｴ蜷医・true縲√◎縺・〒なしｴ蜷医・false
	 * [蜑ｯ菴懃畑] 莠､蟾ｮ縺励◆繝昴Μ繧ｴ繝ｳ縺・D繝・ヰ繝・げ謠冗判縺輔ｌ繧・
	 */
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	/*
	 * @brief 謖・ｮ壹＆繧後◆邱壼・縺ｨ繧ｹ繝・・繧ｸ蝨ｰ蠖｢縺ｨ縺ｮ莠､蟾ｮ菴咲ｽｮ繧定ｨ育ｮ励＠縺ｦ霑斐☆
	 * [蜈･蜉嫋 pos1, pos2: 邱壼・縺ｮ蟋狗せ・邨らせ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 莠､蟾ｮ縺励◆菴咲ｽｮ蠎ｧ讓呻ｼ井ｺ､蟾ｮ縺励※縺・↑縺・ｴ蜷医・(0,0,0)・・
	 * [蜑ｯ菴懃畑] なし
	 */
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

	/*
	 * @brief 謖・ｮ壹＆繧後◆邱壼・縺ｨ繧ｹ繝・・繧ｸ蝨ｰ蠖｢縺ｨ縺ｮ莠､蟾ｮ蛻､螳壹ｒ陦後＞縲∫ｵ先棡繧偵ョ繝舌ャ繧ｰ陦ｨ遉ｺ縺吶ｋ
	 * [蜈･蜉嫋 pos1, pos2: 邱壼・縺ｮ蟋狗せ・邨らせ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 莠､蟾ｮ縺励◆菴咲ｽｮ蠎ｧ讓呻ｼ井ｺ､蟾ｮ縺励※縺・↑縺・ｴ蜷医・(0,0,0)・・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢荳翫↓莠､蟾ｮ蠎ｧ讓吶・繝・ヰ繝・げ繝・く繧ｹ繝医′謠冗判縺輔ｌ繧・
	 */
	VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);

private:
	int model_handle_;
	int collision_handle_;
};
