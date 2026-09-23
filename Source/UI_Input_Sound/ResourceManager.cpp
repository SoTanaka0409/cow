#include "DxLib.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// 登録されたすべてのオリジナルモデルハンドルを解放
	for (int i = 0; i < resource_map_list_.size(); i++)
	{
		MV1DeleteModel(resource_map_list_.at(i).second);
	}

	// 確保した分割画像メタデータオブジェクトをメモリから解放
	for (int i = 0; i < div_graphic_resource_map_list_.size(); i++)
	{
		delete div_graphic_resource_map_list_.at(i);
	}
	div_graphic_resource_map_list_.clear();

	MV1InitModel();
	InitGraph(); // DxLibに読み込まれているすべての画像を一括削除
}

int ResourceManager::LoadModel(const std::string& pathName)
{
	// 既に同じモデルがロード済みの場合は、そのオリジナルから複製を作成して返す
	for (int i = 0; i < resource_map_list_.size(); i++)
	{
		if (resource_map_list_.at(i).first == pathName)
		{
			return MV1DuplicateModel(resource_map_list_.at(i).second);
		}
	}

	int handle = MV1LoadModel(pathName.c_str());
	if (handle == -1)
	{
		return -1;
	}

	resource_map_list_.push_back(std::pair<std::string, int>(pathName, handle));
	return MV1DuplicateModel(handle);
}

void ResourceManager::PreloadModel(const std::string& pathName)
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

/// @brief 2D画像ファイルをキャッシュ読み込みする（既存ロード時は既存のハンドルを返す）
/// @details キャッシュヒット時もハンドルの有効性を検証し、無効なら再ロードする
/// @details これによりシーン切り替え後にハンドルが無効になった場合でも正しく復元できる
int ResourceManager::LoadGraphics(const std::string& pathName)
{
	for (int i = 0; i < graphic_resource_map_list_.size(); i++)
	{
		if (graphic_resource_map_list_.at(i).first == pathName)
		{
			int cachedHandle = graphic_resource_map_list_.at(i).second;
			// キャッシュヒット時にハンドルの有効性を確認。無効な場合は再ロード
			int checkW, checkH;
			if (GetGraphSize(cachedHandle, &checkW, &checkH) != -1)
			{
				return cachedHandle; // 有効なハンドルをそのまま返す
			}
			// ハンドルが無効になっている場合は再ロード
			int newHandle = LoadGraph(pathName.c_str());
			if (newHandle == -1) return -1;
			graphic_resource_map_list_.at(i).second = newHandle;
			return newHandle;
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

void ResourceManager::PreloadGraphics(const std::string& pathName)
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

DivGraphData* ResourceManager::LoadDivGraphics(const std::string& pathName, int all_num_, int numX, int numY)
{
	for (int i = 0; i < div_graphic_resource_map_list_.size(); i++)
	{
		if (div_graphic_resource_map_list_.at(i)->file_path_ == pathName)
		{
			return div_graphic_resource_map_list_.at(i);
		}
	}

	// 分割処理を行うための基底画像サイズ情報取得用として、一時的に画像をロードする
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
	DeleteGraph(handle); // サイズ取得後の一時ハンドルは不要なため解放

	// 指定されたグリッド分割数と1コマサイズを基に、分割画像としてハンドルをロード
	int loadResult = LoadDivGraph(pathName.c_str(), all_num_, numX, numY, sizeX / numX, sizeY / numY, data->div_handle_list_);
	if (loadResult == -1)
	{
		delete data;
		return nullptr;
	}

	div_graphic_resource_map_list_.push_back(data);
	return data;
}
