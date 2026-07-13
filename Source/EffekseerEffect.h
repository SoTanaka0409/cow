#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

// 3D遨ｺ髢謎ｸ翫〒Effekseer(`.efk`)蠖｢蠑上・蛟句挨繧ｨ繝輔ぉ繧ｯ繝医ヵ繧｡繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲√Ο繝ｼ繝峨・蜀咲函・蛛懈ｭ｢・蠎ｧ讓呵ｿｽ蠕薙ｒ邂｡逅・☆繧九け繝ｩ繧ｹ
class EffekseerEffect
{
public:
	/*
	 * @brief 繧｢繧ｻ繝・ヨ繝輔ぃ繧､繝ｫ繝代せ縲√し繧､繧ｺ縲∝・譛溷ｺｧ讓吶ｒ蜿励￠蜿悶ｊ縲√い繧ｻ繝・ヨ縺ｮ繝ｭ繝ｼ繝峨ｒ陦後≧
	 * [蜈･蜉嫋 filename: 繧ｨ繝輔ぉ繧ｯ繝医ヵ繧｡繧､繝ｫ縺ｮ繝代せ, initPos: 蛻晄悄蠎ｧ讓・ kEffectSize: 謠冗判蛟咲紫繧ｹ繧ｱ繝ｼ繝ｫ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｨ繝輔ぉ繧ｯ繝医い繧ｻ繝・ヨ縺ｮ繝ｭ繝ｼ繝牙・逅・ｮ溯｡・
	 */
	EffekseerEffect(const char* filename, VECTOR initPos, float kEffectSize);

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ繧薙□繧ｨ繝輔ぉ繧ｯ繝医Μ繧ｽ繝ｼ繧ｹ繧奪X繝ｩ繧､繝悶Λ繝ｪ・・ffekseer・牙・縺九ｉ遐ｴ譽・☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｨ繝輔ぉ繧ｯ繝医い繧ｻ繝・ヨ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ
	 */
	~EffekseerEffect();

	/*
	 * @brief 繧ｨ繝輔ぉ繧ｯ繝医ヵ繧｡繧､繝ｫ繧偵Ο繝ｼ繝峨＠縲√Μ繧ｽ繝ｼ繧ｹ繝上Φ繝峨Ν繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] effectResourcehandle縺ｮ譖ｸ縺肴鋤縺・
	 */
	void Load();

	/*
	 * @brief 迴ｾ蝨ｨ蜀咲函荳ｭ縺ｮ繧ｨ繝輔ぉ繧ｯ繝井ｽ咲ｽｮ縲∝屓霆｢隗貞ｺｦ縲√せ繧ｱ繝ｼ繝ｫ蛟､繧呈怙譁ｰ蛟､縺ｫ霑ｽ蠕捺峩譁ｰ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜀咲函荳ｭ繧ｨ繝輔ぉ繧ｯ繝医・繝ｯ繝ｼ繝ｫ繝芽｡悟・縺ｮ譖ｴ譁ｰ
	 */
	void Update();

	/*
	 * @brief 謠冗判蜃ｦ逅・ｼ育ｩｺ繝｡繧ｽ繝・ラ縲・ffectManager縺悟・菴薙ｒ荳諡ｬ謠冗判縺吶ｋ縺溘ａ荳崎ｦ・ｼ・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Draw();

	/*
	 * @brief 繝ｭ繝ｼ繝峨＠縺溘お繝輔ぉ繧ｯ繝医Μ繧ｽ繝ｼ繧ｹ縺ｮ蜀咲函繧帝幕蟋九＠縲∝・逕溘う繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繝上Φ繝峨Ν繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜀咲函繝上Φ繝峨Ν (playingEffectHandle) 縺ｮ蜿門ｾ励∝・逕滉ｽ咲ｽｮ險ｭ螳・
	 */
	void Play();

	/*
	 * @brief 蜀咲函荳ｭ縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ｒ蠑ｷ蛻ｶ逧・↓蛛懈ｭ｢縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜀咲函荳ｭ繧ｨ繝輔ぉ繧ｯ繝医・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ豸域ｻ・
	 */
	void Stop();

	// 菴咲ｽｮ縲∝屓霆｢縲√せ繧ｱ繝ｼ繝ｫ縲√Ν繝ｼ繝苓ｨｭ螳壹・繧｢繧ｯ繧ｻ繝・し鄒､
	void SetPosition(VECTOR pos) { play_pos_ = pos; }
	VECTOR GetPosition() { return play_pos_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GetRotation() { return rotation_; }

	void SetScale(VECTOR scale) { mvScale = scale; }
	VECTOR GetScale() { return mvScale; }

	void SetLoop(bool mfLoop) { LoopFlag = mfLoop; }

	/*
	 * @brief 迴ｾ蝨ｨ繧ｨ繝輔ぉ繧ｯ繝医′蜀咲函荳ｭ縺ｧ縺ゅｋ縺九ｒ蛻､螳壹☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 蜀咲函荳ｭ縺ｪ繧液rue
	 * [蜑ｯ菴懃畑] なし
	 */
	bool IsPlaying()
	{
		return IsEffekseer3DEffectPlaying(playingEffectHandle) != -1;
	}

private:
	VECTOR play_pos_;             // 蜀咲函荳ｭ縺ｮ3D菴咲ｽｮ蠎ｧ讓・
	VECTOR rotation_;          // 蝗櫁ｻ｢隗貞ｺｦ・医Λ繧ｸ繧｢繝ｳ・・
	VECTOR mvScale;             // XYZ縺昴ｌ縺槭ｌ縺ｮ謠冗判諡｡螟ｧ繧ｹ繧ｱ繝ｼ繝ｫ

	const char* file_path_;       // 繧ｨ繝輔ぉ繧ｯ繝医ヵ繧｡繧､繝ｫ繧｢繧ｻ繝・ヨ縺ｮ繝代せ
	int effect_resource_handle_;   // 繧ｨ繝輔ぉ繧ｯ繝医・繝ｭ繝ｼ繝画ｸ医Μ繧ｽ繝ｼ繧ｹ繝上Φ繝峨Ν
	int playingEffectHandle;    // 迴ｾ蝨ｨ蜀咲函荳ｭ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ邂｡逅・ワ繝ｳ繝峨Ν

	float effectSize;           // 繧ｨ繝輔ぉ繧ｯ繝医Ο繝ｼ繝画凾縺ｮ蛻晄悄繧ｵ繧､繧ｺ蛟咲紫
	const int EffectPlayInterval = 300; // 蜀咲函蜻ｨ譛溘ヱ繝ｩ繝｡繝ｼ繧ｿ・域悴菴ｿ逕ｨ・・
	int PlayCount;              // 蜀咲函繝輔Ξ繝ｼ繝繧ｫ繧ｦ繝ｳ繧ｿ・域悴菴ｿ逕ｨ・・
	bool LoopFlag;              // 繝ｫ繝ｼ繝怜・逕溘☆繧九°縺ｩ縺・°縺ｮ繝輔Λ繧ｰ
};