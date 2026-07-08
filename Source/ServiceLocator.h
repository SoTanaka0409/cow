#pragma once
#include <vector>

class Scene;
class ObjectManager;
class CowManager;
class AnimalManager;
class GameManager;
class Fever;
class Player3D;

// 繧ｰ繝ｭ繝ｼ繝舌Ν縺ｪ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ繧・が繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繧｢繧ｯ繧ｻ繧ｹ繧呈署萓帙☆繧九Ο繧ｱ繝ｼ繧ｿ繝ｼ
// 萓晏ｭ俶ｧ縺ｮ豕ｨ蜈･繧堤ｰ｡逡･蛹悶☆繧九◆繧∝推繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ蜿門ｾ励ｒ莉ｲ莉九☆繧・
class ServiceLocator
{
public:
	/*
	 * @brief 迴ｾ蝨ｨ繧｢繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 Scene繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static Scene* GetCurrentScene();

	/*
	 * @brief 繧ｪ繝悶ず繧ｧ繧ｯ繝育ｮ｡逅・け繝ｩ繧ｹ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 ObjectManager繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static ObjectManager* GetObjectManager();

	/*
	 * @brief 繝励Ξ繧､繝､繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝・蜊倅ｽ・繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 Player3D繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static Player3D* GetPlayer();

	/*
	 * @brief 蟄伜惠縺吶ｋ蜈ｨ繝励Ξ繧､繝､繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝医ｒ蜿門ｾ励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 Player3D繝昴う繝ｳ繧ｿ縺ｮ繝吶け繧ｿ繝ｼ
	 * [蜑ｯ菴懃畑] 繝吶け繧ｿ繝ｼ讒狗ｯ峨・繧ｳ繧ｹ繝医≠繧・
	 */
	static std::vector<Player3D*> GetPlayers();

	/*
	 * @brief 迚帙く繝｣繝ｩ繧ｯ繧ｿ繝ｼ邂｡逅・け繝ｩ繧ｹ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 CowManager繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static CowManager* GetCowManager();

	/*
	 * @brief 縺昴・莉悶・蜍慕黄邂｡逅・け繝ｩ繧ｹ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 AnimalManager繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static AnimalManager* GetAnimalManager();

	/*
	 * @brief 繧ｲ繝ｼ繝騾ｲ陦檎ｮ｡逅・け繝ｩ繧ｹ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 GameManager繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static GameManager* GetGameManager();

	/*
	 * @brief 繝輔ぅ繝ｼ繝舌・迥ｶ諷狗ｮ｡逅・け繝ｩ繧ｹ繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 Fever繝昴う繝ｳ繧ｿ (蜿門ｾ怜､ｱ謨玲凾縺ｯnullptr)
	 * [蜑ｯ菴懃畑] 縺ｪ縺・
	 */
	static Fever* GetFever();
};
