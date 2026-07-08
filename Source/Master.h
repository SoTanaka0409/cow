#pragma once
#include"SceneManager.h"
#include"SoundManager.h"
#include"ResourceManager.h"
#include<string>
#include<vector>
#include<iostream>
#include"Camera.h"
#include"DebugCamera.h"
#include"Score.h"
#include"Scene.h"
#include"Level.h"
#include"EffectManager.h"
#include"Fever.h"

// 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ蝗樣∩縺ｨ蜷・ｩ溯・縺ｸ縺ｮ繧ｰ繝ｭ繝ｼ繝舌Ν繧｢繧ｯ繧ｻ繧ｹ繧呈署萓帙☆繧九◆繧・撕逧・け繝ｩ繧ｹ縺ｨ縺励※螳夂ｾｩ
class Master
{
public:
	static SceneManager* mpSceneManager;      // 繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺医→繝ｩ繧､繝輔し繧､繧ｯ繝ｫ邂｡逅・畑
	static SoundManager* mpSoundManager;      // BGM/SE縺ｮ荳諡ｬ蛻ｶ蠕｡逕ｨ
	static Score* mpScore;                    // UI陦ｨ遉ｺ縺ｨ繝ｪ繧ｶ繝ｫ繝育ｮ怜・縺ｮ縺溘ａ縺ｮ繧ｹ繧ｳ繧｢諠・ｱ
	static ResourceManager* mpResourceManager;// 鬆ｻ郢√↑繝ｭ繝ｼ繝峨↓繧医ｋ繧ｫ繧ｯ縺､縺阪ｒ髦ｲ縺舌い繧ｻ繝・ヨ繧ｭ繝｣繝・す繝･
	static Camera* mpCamera;                  // 繝励Ξ繧､繝､繝ｼ霑ｽ蠕鍋畑縺ｮ繝｡繧､繝ｳ繧ｫ繝｡繝ｩ
	static DebugCamera* mpDebugCamera;        // 繝・ヰ繝・げ譎ゅ・閾ｪ逕ｱ隕也せ遒ｺ隱咲畑
	static bool mbIsDebugCamera;              // 蜈･蜉幃・譁ｭ縺ｨ繧ｫ繝｡繝ｩ謫堺ｽ懊ｒ蛻・ｊ譖ｿ縺医ｋ繝・ヰ繝・げ繝輔Λ繧ｰ
	static Level* mpLevel;                    // 繝代Λ繝｡繝ｼ繧ｿ繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ逕ｨ繝ｬ繝吶Ν諠・ｱ
	static EffectManager* mpEffectManager;    // Effekseer繧ｨ繝輔ぉ繧ｯ繝医・謠冗判繝ｻ蟇ｿ蜻ｽ邂｡逅・畑
	
	static bool SelectSkill;                  // 繧ｹ繧ｭ繝ｫ驕ｸ謚樔ｸｭ縺ｮ繧ｲ繝ｼ繝騾ｲ陦悟●豁｢逕ｨ繝輔Λ繧ｰ
	static int mnTutorialcount;               // 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮ騾ｲ陦悟ｺｦ蛻､螳夂畑繧ｫ繧ｦ繝ｳ繧ｿ繝ｼ
	static bool GameFinishFlag;               // 繝ｪ繧ｶ繝ｫ繝育ｧｻ陦後ｒ蛻ｶ蠕｡縺吶ｋ邨ゆｺ・憾諷九ヵ繝ｩ繧ｰ
	static bool FeverFlag;                    // 閾ｪ蜍募精蠑輔→螟ｧ驥丞・迴ｾ繝｢繝ｼ繝峨ｒ譛牙柑縺ｫ縺吶ｋ繝輔Λ繧ｰ
	static int mnCaughtCowCount;              // 螳溽ｸｾ隗｣髯､縺ｨ繧､繝吶Φ繝育匱逕溷愛螳夂畑縺ｮ邱丞屓蜿取焚
	static bool tutorial_vacum_flag_;             // 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｧ縺ｮ蜷ｸ蠑墓桃菴憺＃謌舌ヵ繝ｩ繧ｰ

	static float mfDeltaTime;                 // 繝輔Ξ繝ｼ繝繝ｬ繝ｼ繝亥､牙虚繧貞精蜿弱☆繧九◆繧√・邨碁℃譎る俣(遘・
	static float GetDeltaTimeScaler() { return mfDeltaTime * 60.0f; } // 60FPS蝓ｺ貅悶・遘ｻ蜍暮㍼陬懈ｭ｣繧ｹ繧ｱ繝ｼ繝ｫ
};