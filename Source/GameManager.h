#pragma once
#include"DxLib.h"
#include<vector>

class GameTimer;

// 繧､繝吶Φ繝磯ｲ陦後ヵ繧ｧ繝ｼ繧ｺ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
class GameManager
{
public:
	enum GameStepType
	{
		kCowGet,
		kFinal,
	};

	struct GameStepData
	{
		GameStepType type;
		bool TrueFlag = true;
	};

	enum class GamePhase
	{
		kNormal,
		kMassSpawn,
		kTornadoCrisis
	};

public:
	GameManager();
	~GameManager();

	/*
	 * 迴ｾ蝨ｨ縺ｮ繧ｲ繝ｼ繝繝輔ぉ繝ｼ繧ｺ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 迴ｾ蝨ｨ縺ｮGamePhase
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	GamePhase GetCurrentPhase() const { return current_phase_; }

	/*
	 * 繧ｲ繝ｼ繝繝輔ぉ繝ｼ繧ｺ繧定ｨｭ螳壹＠繧､繝吶Φ繝育憾諷九ｒ蛻・ｊ譖ｿ縺医ｋ
	 * [蜈･蜉嫋 phase: 譁ｰ縺励＞繝輔ぉ繝ｼ繧ｺ迥ｶ諷・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] current_phase_ 縺梧峩譁ｰ縺輔ｌ繧・
	 */
	void SetCurrentPhase(GamePhase phase) { current_phase_ = phase; }

	/*
	 * 繧ｹ繧ｳ繧｢逋ｻ骭ｲ繧剃ｼｴ縺・ｵゆｺ・せ繝・ャ繝励∈縺ｮ驕ｷ遘ｻ繧定｡後≧
	 * [蜈･蜉嫋 type: 驕ｷ遘ｻ蜈医・繧ｹ繝・ャ繝励ち繧､繝・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 騾ｲ陦後せ繝・ャ繝怜､画峩縲√ヵ繝ｩ繧ｰ譖ｴ譁ｰ縲√ロ繝ｼ繝蜈･蜉幃幕蟋・
	 */
	void GameNextStep(GameStepType type);
	 
	GameStepType GetType() { return type_; }

	/*
	 * 繝輔ぉ繝ｼ繝峨う繝ｳ遲峨√ご繝ｼ繝騾ｲ陦後↓蠢・ｦ√↑貍泌・謠冗判繧定｡後≧
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢縺ｸ縺ｮ證苓ｻ｢遏ｩ蠖｢謠冗判
	 */
	void Draw();

	/*
	 * 繧ｲ繝ｼ繝縺ｮ騾ｲ陦檎憾諷九→蛻ｶ髯先凾髢薙ｒ逶｣隕悶・譖ｴ譁ｰ縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｿ繧､繝槭・譖ｴ譁ｰ縲√ヵ繧ｧ繝ｼ繧ｺ驕ｷ遘ｻ謚ｽ驕ｸ螳溯｡・
	 */
	void Update();

	GameTimer* GetGameTimer() const { return game_timer_; }

private:
	std::vector<GameStepData*> data_; // 蜍慕噪遒ｺ菫昴＆繧後◆繧ｹ繝・ャ繝励ョ繝ｼ繧ｿ縺ｮ繝ｪ繧ｹ繝・
	GameStepType type_;
	GamePhase current_phase_;

	float fade_timer_;                  // 繧ｷ繝ｼ繝ｳ髢句ｧ区凾縺ｮ繝輔ぉ繝ｼ繝画ｼ泌・逕ｨ
	bool fade_flag_;
	
	int phase_timer_;                 // 繝輔ぉ繝ｼ繧ｺ驕ｷ遘ｻ繧､繝吶Φ繝医・逋ｺ逕溘ち繧､繝溘Φ繧ｰ蝓ｺ貅・
	int phase_change_count_;

	GameTimer* game_timer_;           // 繧ｿ繧､繝槭・邂｡逅・畑繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ
};