#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Thunder.h"
#include"CowManager.h"
#include"AnimalManager.h"

// 3D繧｢繧ｯ繧ｷ繝ｧ繝ｳ繧ｹ繝・・繧ｸ邂｡逅・
class Scene3D : public Scene
{
public: 
	// 繧ｲ繝ｼ繝騾ｲ陦後・繝輔ぉ繝ｼ繧ｺ
	enum Phase
	{
		Normal,         // 騾壼ｸｸ
		TornadoCrisis,  // 遶懷ｷｻ蟾ｨ螟ｧ蛹・
		MobSpawn,       // 逕溽黄螟ｧ驥上せ繝昴・繝ｳ
	};	
	
public:
	Scene3D();
	~Scene3D();

	/*
	 * @brief 繧ｹ繝・・繧ｸ蛻晄悄蛹・
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繧ｪ繝悶ず繧ｧ繧ｯ繝育函謌舌。GM蜀咲函
	 */
	void Initialize() override;

	/*
	 * @brief 繧ｷ繝ｼ繝ｳ邨ゆｺ・・逅・
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] BGM蛛懈ｭ｢
	 */
	void Finalize() override;

	/*
	 * @brief 繧ｹ繝・・繧ｸ謠冗判
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 逕ｻ髱｢謠冗判
	 */
	void Draw() override;

	/*
	 * @brief 繧ｹ繝・・繧ｸ譖ｴ譁ｰ
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繧ｿ繧､繝繧｢繝・・譎ゅ・繝ｪ繧ｶ繝ｫ繝磯・遘ｻ
	 */
	void Update() override;

	/*
	 * @brief 繝輔ぉ繝ｼ繧ｺ騾ｲ陦後↓繧医ｋ迺ｰ蠅・､牙喧
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繧ｫ繝｡繝ｩ繝ｻ遶懷ｷｻ迥ｶ諷句､画峩縲√せ繝昴・繝ｳ蜃ｦ逅・
	 */
	void PhaseUpdate();

private:
	float warningRadius;       // 隴ｦ蜻翫お繝ｪ繧｢迴ｾ蝨ｨ蜊雁ｾ・(莠育ｴ・
	float maxRadius;           // 隴ｦ蜻翫お繝ｪ繧｢譛螟ｧ蜊雁ｾ・(莠育ｴ・
	
	int tatumaki_spawn_timer_;   // 遶懷ｷｻ蜃ｺ迴ｾ繧ｿ繧､繝槭・
	int mMassSpawnTimer;       // 螟ｧ驥丞・迴ｾ縺ｮ髢馴囈繧ｿ繧､繝槭・
	Phase mpPhase;             // 迴ｾ蝨ｨ縺ｮ繝輔ぉ繝ｼ繧ｺ
	int mFontBackGraph;        // 隴ｦ蜻翫ユ繧ｭ繧ｹ繝郁レ譎ｯ逕ｨ繝上Φ繝峨Ν
};