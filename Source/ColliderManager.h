#pragma once
#include <vector>

class Collider;

// 繧ｲ繝ｼ繝蜀・・縺吶∋縺ｦ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ邂｡逅・＠縲∫嶌莠偵・蠖薙◆繧雁愛螳壹ｒ荳諡ｬ蜃ｦ逅・☆繧九・繝阪・繧ｸ繝｣繝ｼ繧ｯ繝ｩ繧ｹ
class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	/*
	 * @brief 豈弱ヵ繝ｬ繝ｼ繝逋ｻ骭ｲ縺輔ｌ縺溘さ繝ｩ繧､繝繝ｼ蜷悟｣ｫ縺ｮ莠､蟾ｮ蛻､螳壹ｒ邱丞ｽ薙◆繧翫〒螳溯｡後☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 場合さ繝ｩ繧､繝繝ｼ縺ｮ蠖薙◆繧雁愛螳壹・繧､繝吶Φ繝育匱陦後・蜻ｼ縺ｳ蜃ｺ縺励∽ｸ崎ｦ√さ繝ｩ繧､繝繝ｼ縺ｮ閾ｪ蜍輔け繝ｪ繝ｼ繝ｳ繧｢繝・・
	 */
	void Update();

	/*
	 * @brief ・医ョ繝舌ャ繧ｰ陦ｨ遉ｺ逕ｨ・牙推繧ｳ繝ｩ繧､繝繝ｼ縺ｮ螟門ｽ｢繧呈緒逕ｻ縺吶ｋ・育樟蝨ｨ蜃ｦ逅・↑縺暦ｼ・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	void Draw();

	/*
	 * @brief 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧貞叙蠕励☆繧具ｼ亥ｭ伜惠縺励↑縺・ｴ蜷医・譁ｰ隕冗函謌撰ｼ・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 ColliderManager繧ｯ繝ｩ繧ｹ縺ｮ髱咏噪繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｸ縺ｮ繝昴う繝ｳ繧ｿ
	 * [蜑ｯ菴懃畑] なし
	 */
	static ColliderManager* GetInstance();

	/*
	 * @brief 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧貞ｮ牙・縺ｫ隗｣謾ｾ縺励∝ｾ檎援莉倥￠繧定｡後≧
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 髱咏噪繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ(instance_)縺ｮdelete
	 */
	static void Finalize();

	/*
	 * @brief 繧ｳ繝ｩ繧､繝繝ｼ繧堤ｮ｡逅・Μ繧ｹ繝医↓霑ｽ蜉縺吶ｋ
	 * [蜈･蜉嫋 Collider: 霑ｽ蜉縺吶ｋ繧ｳ繝ｩ繧､繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mColliderList縺ｸ縺ｮ霑ｽ蜉
	 */
	void AddCollider(Collider* Collider);

	/*
	 * @brief 逋ｻ骭ｲ縺輔ｌ縺ｦ縺・ｋ縺吶∋縺ｦ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蜑企勁繝輔Λ繧ｰ繧堤ｫ九※縲∫ｮ｡逅・Μ繧ｹ繝医ｒ遨ｺ縺ｫ縺吶ｋ
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 蜈ｨ逋ｻ骭ｲ繧ｳ繝ｩ繧､繝繝ｼ縺ｮSetDeleteFlag(true)螳溯｡後√♀繧医・mColliderList縺ｮ繧ｯ繝ｪ繧｢
	 */
	void DeleteAllCollider();

	/*
	 * @brief 蜑企勁繝輔Λ繧ｰ(delete_flag_)縺檎悄縺ｫ險ｭ螳壹＆繧後※縺・ｋ繧ｳ繝ｩ繧､繝繝ｼ繧堤ｮ｡逅・Μ繧ｹ繝医°繧芽・蜍暮勁螟悶☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mColliderList縺九ｉ縺ｮ蜑企勁
	 */
	void DeleteAllColliderIfNeeded();

	/*
	 * @brief 迚ｹ螳壹・繧ｳ繝ｩ繧､繝繝ｼ繧堤ｮ｡逅・Μ繧ｹ繝医°繧画焔蜍輔〒髯､螟悶☆繧・
	 * [蜈･蜉嫋 collider: 髯､螟悶☆繧九さ繝ｩ繧､繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] mColliderList縺九ｉ縺ｮ迚ｹ螳壹さ繝ｩ繧､繝繝ｼ縺ｮ蜑企勁
	 */
	void RemoveCollider(Collider* collider);
	
	std::vector<Collider*>& GetColliderList() { return collider_list_; }

private:
	bool CheckCollision(Collider* colA, Collider* colB);

	std::vector<Collider*> collider_list_; // 蛻､螳壼ｯｾ雎｡縺ｨ縺ｪ繧九い繧ｯ繝・ぅ繝悶↑蜈ｨ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ邂｡逅・Μ繧ｹ繝・

	static ColliderManager* instance_;  // 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ邂｡逅・畑縺ｮ髱咏噪繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繝昴う繝ｳ繧ｿ
};