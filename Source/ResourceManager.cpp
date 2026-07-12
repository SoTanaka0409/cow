#include "DxLib.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// 逋ｻ骭ｲ縺輔ｌ縺溘☆縺ｹ縺ｦ縺ｮ繧ｪ繝ｪ繧ｸ繝翫Ν繝｢繝・Ν繝上Φ繝峨Ν繧堤ｴ譽・
	for (int i = 0; i < resource_map_list_.size(); i++)
	{
		MV1DeleteModel(resource_map_list_.at(i).second);
	}

	// 遒ｺ菫昴＠縺溷・蜑ｲ逕ｻ蜒上Γ繧ｿ繝・・繧ｿ讒矩菴薙が繝悶ず繧ｧ繧ｯ繝医ｒ繝｡繝｢繝ｪ縺九ｉ隗｣謾ｾ
	for (int i = 0; i < div_graphic_resource_map_list_.size(); i++)
	{
		delete div_graphic_resource_map_list_.at(i);
	}
	div_graphic_resource_map_list_.clear();

	MV1InitModel();
	InitGraph(); // DxLib縺ｫ隱ｭ縺ｿ霎ｼ縺ｾ繧後※縺・ｋ縺吶∋縺ｦ縺ｮ逕ｻ蜒上ｒ荳諡ｬ蜑企勁
}

/*
 * @brief 3D繝｢繝・Ν繝輔ぃ繧､繝ｫ繧偵く繝｣繝・す繝･隱ｭ縺ｿ霎ｼ縺ｿ縺吶ｋ・井ｺ碁㍾繝ｭ繝ｼ繝画凾縺ｯ隍・｣ｽ繝｢繝・Ν繧定ｿ斐☆・・
 * [蜈･蜉嫋 pathName: 繝｢繝・Ν繝輔ぃ繧､繝ｫ縺ｮ繝輔ぃ繧､繝ｫ繝代せ
 * [蜃ｺ蜉嫋 隍・｣ｽ縺輔ｌ縺溘Δ繝・Ν繝上Φ繝峨Ν (繧ｨ繝ｩ繝ｼ譎ゅ・-1)
 * [蜑ｯ菴懃畑] 譁ｰ隕上Ο繝ｼ繝画凾縺ｯ繧ｪ繝ｪ繧ｸ繝翫Ν縺ｨ縺励※resourceMapList縺ｫ霑ｽ蜉
 */
int ResourceManager::LoadModel(std::string pathName)
{
	// 譌｢縺ｫ蜷後§繝｢繝・Ν縺後Ο繝ｼ繝画ｸ医∩縺ｮ蝣ｴ蜷医・縲√◎縺ｮ繧ｪ繝ｪ繧ｸ繝翫Ν縺九ｉ隍・｣ｽ繧剃ｽ懈・縺励※霑斐☆
	for (int i = 0; i < resource_map_list_.size(); i++)
	{
		if (resource_map_list_.at(i).first == pathName)
		{
			return MV1DuplicateModel(resource_map_list_.at(i).second);
		}
	}

	// 譛ｪ繝ｭ繝ｼ繝峨・蝣ｴ蜷医・譁ｰ隕上〒繝｢繝・Ν繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ縺ｿ
	int handle = MV1LoadModel(pathName.c_str());
	if (handle == -1)
	{
		return -1;
	}

	resource_map_list_.push_back(std::pair<std::string, int>(pathName, handle));
	return MV1DuplicateModel(handle); // 繧ｪ繝ｪ繧ｸ繝翫Ν縺ｯ菫晉ｮ｡縺励∬､・｣ｽ蛛ｴ繧偵ご繝ｼ繝蛛ｴ縺ｧ菴ｿ逕ｨ縺輔○繧・
}

void ResourceManager::PreloadModel(std::string pathName)
{
	for (int i = 0; i < resource_map_list_.size(); i++)
	{
		if (resource_map_list_.at(i).first == pathName) return;
	}

	int handle = MV1LoadModel(pathName.c_str());
	if (handle != -1)
	{
		resource_map_list_.push_back(std::pair<std::string, int>(pathName, handle));
	}
}

/*
 * @brief 2D逕ｻ蜒上ヵ繧｡繧､繝ｫ繧偵く繝｣繝・す繝･隱ｭ縺ｿ霎ｼ縺ｿ縺吶ｋ・井ｺ碁㍾繝ｭ繝ｼ繝画凾縺ｯ譌｢蟄倥・繝上Φ繝峨Ν繧定ｿ斐☆・・
 * [蜈･蜉嫋 pathName: 逕ｻ蜒上ヵ繧｡繧､繝ｫ縺ｮ繝輔ぃ繧､繝ｫ繝代せ
 * [蜃ｺ蜉嫋 逕ｻ蜒上げ繝ｩ繝輔ぅ繝・け繝上Φ繝峨Ν (繧ｨ繝ｩ繝ｼ譎ゅ・-1)
 * [蜑ｯ菴懃畑] 譁ｰ隕上Ο繝ｼ繝画凾縺ｯ繧ｭ繝｣繝・す繝･逕ｨ繝ｪ繧ｹ繝医↓霑ｽ蜉
 */
int ResourceManager::LoadGraphics(std::string pathName)
{
	for (int i = 0; i < graphic_resource_map_list_.size(); i++)
	{
		if (graphic_resource_map_list_.at(i).first == pathName)
		{
			return graphic_resource_map_list_.at(i).second;
		}
	}

	int handle = LoadGraph(pathName.c_str());
	if (handle == -1)
	{
		return -1;
	}

	graphic_resource_map_list_.push_back(std::pair<std::string, int>(pathName, handle));
	return handle;
}

void ResourceManager::PreloadGraphics(std::string pathName)
{
	for (int i = 0; i < graphic_resource_map_list_.size(); i++)
	{
		if (graphic_resource_map_list_.at(i).first == pathName) return;
	}

	int handle = LoadGraph(pathName.c_str());
	if (handle != -1)
	{
		graphic_resource_map_list_.push_back(std::pair<std::string, int>(pathName, handle));
	}
}

/*
 * @brief 蛻・牡逕ｻ蜒上ヵ繧｡繧､繝ｫ繧偵く繝｣繝・す繝･隱ｭ縺ｿ霎ｼ縺ｿ縺励∝・蜑ｲ繝・・繧ｿ讒矩菴薙ｒ霑斐☆
 * [蜈･蜉嫋 pathName: 逕ｻ蜒上ヵ繧｡繧､繝ｫ繝代せ, all_num_: 蛻・牡邱乗焚, numX: 讓ｪ蛻・牡謨ｰ, numY: 邵ｦ蛻・牡謨ｰ
 * [蜃ｺ蜉嫋 蛻・牡繝・・繧ｿ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繧､繝ｳ繧ｿ (繧ｨ繝ｩ繝ｼ譎ゅ・nullptr)
 * [蜑ｯ菴懃畑] 譁ｰ隕上Ο繝ｼ繝画凾縺ｯ繝｡繧ｿ繝・・繧ｿ讒矩菴薙ｒ蜍慕噪逕滓・縺励※繧ｭ繝｣繝・す繝･縺ｫ霑ｽ蜉
 */
DivGraphData* ResourceManager::LoadDivGraphics(std::string pathName, int all_num_, int numX, int numY)
{
	for (int i = 0; i < div_graphic_resource_map_list_.size(); i++)
	{
		if (div_graphic_resource_map_list_.at(i)->file_path_ == pathName)
		{
			return div_graphic_resource_map_list_.at(i);
		}
	}

	// 蛻・牡蜃ｦ逅・ｒ陦後≧縺溘ａ縺ｮ蝓ｺ逶､逕ｻ蜒上し繧､繧ｺ諠・ｱ蜿門ｾ礼畑縺ｨ縺励※縲∽ｸ蠎ｦ荳譎ら噪縺ｫ逕ｻ蜒上ｒ繝ｭ繝ｼ繝峨☆繧・
	int handle = LoadGraph(pathName.c_str());
	if (handle == -1)
	{
		return nullptr;
	}

	DivGraphData *data = new DivGraphData(
		pathName,
		numX, numY,
		all_num_
	);

	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);
	DeleteGraph(handle); // 繧ｵ繧､繧ｺ蜿門ｾ怜ｾ後・荳譎ゅワ繝ｳ繝峨Ν縺ｯ荳崎ｦ√↑縺溘ａ隗｣謾ｾ

	// 謖・ｮ壹＆繧後◆繧ｰ繝ｪ繝・ラ蛻・牡謨ｰ縺ｨ1繧ｳ繝槭し繧､繧ｺ繧貞渕縺ｫ縲∝・蜑ｲ逕ｻ蜒上→縺励※場合ワ繝ｳ繝峨Ν繧偵Ο繝ｼ繝・
	int loadResult = LoadDivGraph(pathName.c_str(), all_num_, numX, numY, sizeX / numX, sizeY / numY, data->div_handle_list_);
	if (loadResult == -1)
	{
		delete data;
		return nullptr;
	}

	div_graphic_resource_map_list_.push_back(data);
	return data;
}