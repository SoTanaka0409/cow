#pragma once
#include"dxlib.h"


// 繝励Ξ繧､繝､繝ｼ縺ｮ陦悟虚蛻ｶ髯舌→縺ｪ繧九ご繝ｼ繝蜈ｨ菴薙・蛻ｶ髯先凾髢薙ｒ邂｡逅・☆繧九け繝ｩ繧ｹ
class GameTimer 
{
public:
	// 繧ｿ繧､繝槭・繧剃ｽｿ逕ｨ縺吶ｋ迥ｶ豕√ち繧ｰ
	enum Tag_Num
	{
		Tag_Game,      // 繝励Ξ繧､荳ｭ縺ｪ縺ｩ縺ｮ謠冗判縺悟ｿ・ｦ√↑繧ｷ繝ｼ繝ｳ
		Tag_NoGame,    // 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｪ縺ｩ譎る俣蛻ｶ髯舌ｒ謠冗判縺励↑縺・す繝ｼ繝ｳ
	};

public:
	GameTimer(VECTOR pos, int timer, Tag_Num num);
	~GameTimer();

	/*
	 * @brief 蛻ｶ髯先凾髢薙・貂帷ｮ怜・逅・ｒ陦後＞縲∫ｵゆｺ・愛螳壹ｒ逶｣隕悶☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 谿九ｊ譎る俣縺ｮ譖ｴ譁ｰ縲∵凾髢灘・繧梧凾縺ｫmbFlag縺翫ｈ縺ｳmbStopFlag繧稚rue蛹・
	 */
	void Update();

	/*
	 * @brief 繝励Ξ繧､繝､繝ｼ縺ｫ谿九ｊ譎る俣繧帝夂衍縺吶ｋ縺溘ａ縺ｮUI繧呈緒逕ｻ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｰ繝ｩ繝輔ぅ繝・け謠冗判
	 */
	void Draw();

	// 譎る俣蛻・ｌ讀懃衍縺ｪ縺ｩ縺ｮ螟夜Κ繧｢繧ｯ繧ｻ繧ｹ逕ｨ繝｡繧ｽ繝・ラ
	bool OutTimerFlag() { return mbFlag; }
	void SetOutTimerFlag(bool flag) { mbFlag = flag; }
	int GetTime() const { return Time; }

private:
	int Time;                  // 謠冗判縺翫ｈ縺ｳ譎る俣蛻・ｌ蛻､螳壹・蝓ｺ貅悶→縺ｪ繧区ｮ九ｊ遘呈焚
	VECTOR position_;         // 逕ｻ髱｢隗｣蜒丞ｺｦ縺ｫ蜷医ｏ縺帙※UI繧帝・鄂ｮ縺吶ｋ縺溘ａ縺ｮ蝓ｺ貅門ｺｧ讓・
	bool mbFlag;               // 繧ｲ繝ｼ繝繧ｪ繝ｼ繝舌・驕ｷ遘ｻ縺ｮ繝医Μ繧ｬ繝ｼ縺ｨ縺ｪ繧区凾髢灘・繧後ヵ繝ｩ繧ｰ
	bool mbStopFlag;           // 繝昴・繧ｺ荳ｭ繧・Μ繧ｶ繝ｫ繝育判髱｢縺ｧ縺ｮ譎る俣騾ｲ陦後ｒ豁｢繧√ｋ繝輔Λ繧ｰ

	int score_text_image_;        // 豈弱ヵ繝ｬ繝ｼ繝縺ｮ繝ｭ繝ｼ繝峨ｒ髦ｲ縺舌◆繧√・LIMIT逕ｻ蜒上ワ繝ｳ繝峨Ν

	int mLastTime;             // 1遘堤ｵ碁℃繧貞愛螳壹☆繧九◆繧√・蜑榊屓險域ｸｬ譎る俣
	Tag_Num tag_;             // 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｨ譛ｬ邱ｨ縺ｧ謠冗判蜃ｦ逅・ｒ蛻・￠繧九◆繧√・繧ｿ繧ｰ
};