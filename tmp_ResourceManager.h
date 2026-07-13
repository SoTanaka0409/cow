#pragma once
#include <vector>
#include <string>
#include"Dxlib.h"

// 刁E画像E読み込みチEEタおよびリソースハンドル惁Eを保持する構造佁E
struct DivGraphData
{
	std::string file_path_;	// 画像アセチEのファイルパス
	int* div_handle_list_;		// 刁EロードされたグラフィチEハンドルの配E
	int div_x_;				// 水平方向E刁E数
	int div_y_;				// 垂直方向E刁E数
	int all_num_;				// 刁E画像E総数

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
		delete[] div_handle_list_; // 動的確保されたグラフィチEハンドル配Eの解放漏れを防ぁE
	}
};

// 3DモチEめEDチEスチャアセチEの二重ロードを防止し、一括管琁EるEネEジャークラス
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	/*
	 * @brief 3DモチEファイルをキャチEュ読み込みするE二重ロード時は褁EモチEを返すEE
	 * [入力] pathName: アセチEファイルパス
	 * [出力] 褁EされたモチEハンドルEエラー時E -1EE
	 * [副作用] 新規ロード時はresourceMapListにオリジナルハンドルを追加登録
	 */
	int LoadModel(std::string pathName);
	void PreloadModel(std::string pathName);

	/*
	 * @brief 2D画像ファイルをキャチEュ読み込みするE二重ロード時は既存Eハンドルを返すEE
	 * [入力] pathName: アセチEファイルパス
	 * [出力] 画像グラフィチEハンドルEエラー時E -1EE
	 * [副作用] 新規ロード時はgraphicResourceMapListに登録
	 */
	int LoadGraphics(std::string pathName);
	void PreloadGraphics(std::string pathName);

	/*
	 * @brief 刁E画像ファイルをキャチEュ読み込みし、E割チEEタ構造体を返す
	 * [入力] pathName: アセチEファイルパス, all_num_: 刁E総数, numX: 横刁E数, numY: 縦刁E数
	 * [出力] 刁E画像データ構造体へのポインタEエラー時E nullptrEE
	 * [副作用] 新規ロード時はdivGraphicResourceMapListに構造体を追加登録
	 */
	DivGraphData* LoadDivGraphics(std::string pathName, int all_num_, int numX, int numY);

	/*
	 * @brief キャチEュ管琁EれてぁEリソースの総数を取得すめE
	 * [入力] Ȃ
	 * [出力] 登録されてぁEリソースの合計数
	 * [副作用] Ȃ
	 */
	int GetTotalResource() { return static_cast<int>(resource_map_list_.size() + graphic_resource_map_list_.size() + div_graphic_resource_map_list_.size()); }

private:
	std::vector<std::pair<std::string, int>> resource_map_list_;			// キャチEュ管琁EモチEアセチEリスチE(パス名とオリジナルハンドルのペア)
	std::vector<std::pair<std::string, int>> graphic_resource_map_list_;	// キャチEュ管琁E2DグラフィチEリスチE
	std::vector<DivGraphData*> div_graphic_resource_map_list_;				// キャチEュ管琁E刁E画像データリスチE
};
