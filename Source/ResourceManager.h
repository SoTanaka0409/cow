#pragma once
#include <vector>
#include <string>
#include"Dxlib.h"

// 蛻・牡逕ｻ蜒上・隱ｭ縺ｿ霎ｼ縺ｿ繝・・繧ｿ縺翫ｈ縺ｳ繝ｪ繧ｽ繝ｼ繧ｹ繝上Φ繝峨Ν諠・ｱ繧剃ｿ晄戟縺吶ｋ讒矩菴・
struct DivGraphData
{
	std::string file_path_;	// 逕ｻ蜒上い繧ｻ繝・ヨ縺ｮ繝輔ぃ繧､繝ｫ繝代せ
	int* div_handle_list_;		// 蛻・牡繝ｭ繝ｼ繝峨＆繧後◆繧ｰ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν縺ｮ驟榊・
	int div_x_;				// 豌ｴ蟷ｳ譁ｹ蜷代・蛻・牡謨ｰ
	int div_y_;				// 蝙ら峩譁ｹ蜷代・蛻・牡謨ｰ
	int all_num_;				// 蛻・牡逕ｻ蜒上・邱乗焚

	DivGraphData(
		std::string file_path_,
		int div_x_,
		int div_y_,
		int all_num_
	)
	{
		this->file_path_ = file_path_;
		this->div_handle_list_ = new int[all_num_];
		this->div_x_ = div_x_;
		this->div_y_ = div_y_;
		this->all_num_ = all_num_;
	}

	~DivGraphData()
	{
		delete[] div_handle_list_; // 蜍慕噪遒ｺ菫昴＆繧後◆繧ｰ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν驟榊・縺ｮ隗｣謾ｾ貍上ｌ繧帝亟縺・
	}
};

// 3D繝｢繝・Ν繧・D繝・け繧ｹ繝√Ε繧｢繧ｻ繝・ヨ縺ｮ莠碁㍾繝ｭ繝ｼ繝峨ｒ髦ｲ豁｢縺励∽ｸ諡ｬ邂｡逅・☆繧九・繝阪・繧ｸ繝｣繝ｼ繧ｯ繝ｩ繧ｹ
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	/*
	 * @brief 3D繝｢繝・Ν繝輔ぃ繧､繝ｫ繧偵く繝｣繝・す繝･隱ｭ縺ｿ霎ｼ縺ｿ縺吶ｋ・井ｺ碁㍾繝ｭ繝ｼ繝画凾縺ｯ隍・｣ｽ繝｢繝・Ν繧定ｿ斐☆・・
	 * [蜈･蜉嫋 pathName: 繧｢繧ｻ繝・ヨ繝輔ぃ繧､繝ｫ繝代せ
	 * [蜃ｺ蜉嫋 隍・｣ｽ縺輔ｌ縺溘Δ繝・Ν繝上Φ繝峨Ν・医お繝ｩ繝ｼ譎ゅ・ -1・・
	 * [蜑ｯ菴懃畑] 譁ｰ隕上Ο繝ｼ繝画凾縺ｯresourceMapList縺ｫ繧ｪ繝ｪ繧ｸ繝翫Ν繝上Φ繝峨Ν繧定ｿｽ蜉逋ｻ骭ｲ
	 */
	int LoadModel(std::string pathName);
	void PreloadModel(std::string pathName);

	/*
	 * @brief 2D逕ｻ蜒上ヵ繧｡繧､繝ｫ繧偵く繝｣繝・す繝･隱ｭ縺ｿ霎ｼ縺ｿ縺吶ｋ・井ｺ碁㍾繝ｭ繝ｼ繝画凾縺ｯ譌｢蟄倥・繝上Φ繝峨Ν繧定ｿ斐☆・・
	 * [蜈･蜉嫋 pathName: 繧｢繧ｻ繝・ヨ繝輔ぃ繧､繝ｫ繝代せ
	 * [蜃ｺ蜉嫋 逕ｻ蜒上げ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν・医お繝ｩ繝ｼ譎ゅ・ -1・・
	 * [蜑ｯ菴懃畑] 譁ｰ隕上Ο繝ｼ繝画凾縺ｯgraphicResourceMapList縺ｫ逋ｻ骭ｲ
	 */
	int LoadGraphics(std::string pathName);
	void PreloadGraphics(std::string pathName);

	/*
	 * @brief 蛻・牡逕ｻ蜒上ヵ繧｡繧､繝ｫ繧偵く繝｣繝・す繝･隱ｭ縺ｿ霎ｼ縺ｿ縺励∝・蜑ｲ繝・・繧ｿ讒矩菴薙ｒ霑斐☆
	 * [蜈･蜉嫋 pathName: 繧｢繧ｻ繝・ヨ繝輔ぃ繧､繝ｫ繝代せ, all_num_: 蛻・牡邱乗焚, numX: 讓ｪ蛻・牡謨ｰ, numY: 邵ｦ蛻・牡謨ｰ
	 * [蜃ｺ蜉嫋 蛻・牡逕ｻ蜒上ョ繝ｼ繧ｿ讒矩菴薙∈縺ｮ繝昴う繝ｳ繧ｿ・医お繝ｩ繝ｼ譎ゅ・ nullptr・・
	 * [蜑ｯ菴懃畑] 譁ｰ隕上Ο繝ｼ繝画凾縺ｯdivGraphicResourceMapList縺ｫ讒矩菴薙ｒ霑ｽ蜉逋ｻ骭ｲ
	 */
	DivGraphData* LoadDivGraphics(std::string pathName, int all_num_, int numX, int numY);

	/*
	 * @brief 繧ｭ繝｣繝・す繝･邂｡逅・＆繧後※縺・ｋ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ邱乗焚繧貞叙蠕励☆繧・
	 * [蜈･蜉嫋 なし
	 * [蜃ｺ蜉嫋 逋ｻ骭ｲ縺輔ｌ縺ｦ縺・ｋ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜷郁ｨ域焚
	 * [蜑ｯ菴懃畑] なし
	 */
	int GetTotalResource() { return static_cast<int>(resource_map_list_.size() + graphic_resource_map_list_.size() + div_graphic_resource_map_list_.size()); }

private:
	std::vector<std::pair<std::string, int>> resource_map_list_;			// 繧ｭ繝｣繝・す繝･邂｡逅・畑繝｢繝・Ν繧｢繧ｻ繝・ヨ繝ｪ繧ｹ繝・(繝代せ蜷阪→繧ｪ繝ｪ繧ｸ繝翫Ν繝上Φ繝峨Ν縺ｮ繝壹い)
	std::vector<std::pair<std::string, int>> graphic_resource_map_list_;	// 繧ｭ繝｣繝・す繝･邂｡逅・畑2D繧ｰ繝ｩ繝輔ぅ繝・け繝ｪ繧ｹ繝・
	std::vector<DivGraphData*> div_graphic_resource_map_list_;				// 繧ｭ繝｣繝・す繝･邂｡逅・畑蛻・牡逕ｻ蜒上ョ繝ｼ繧ｿ繝ｪ繧ｹ繝・
};
