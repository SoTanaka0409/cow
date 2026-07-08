#pragma once
#include <vector>
#include <string>
#include"Dxlib.h"

// 分割画像の読み込みデータおよびリソースハンドル情報を保持する構造体
struct DivGraphData
{
	std::string filePath;	// 画像アセットのファイルパス
	int* divHandleList;		// 分割ロードされたグラフィックハンドルの配列
	int divX;				// 水平方向の分割数
	int divY;				// 垂直方向の分割数
	int allNum;				// 分割画像の総数

	DivGraphData(
		std::string filePath,
		int divX,
		int divY,
		int allNum
	)
	{
		this->filePath = filePath;
		this->divHandleList = new int[allNum];
		this->divX = divX;
		this->divY = divY;
		this->allNum = allNum;
	}

	~DivGraphData()
	{
		delete[] divHandleList; // 動的確保されたグラフィックハンドル配列の解放漏れを防ぐ
	}
};

// 3Dモデルや2Dテクスチャアセットの二重ロードを防止し、一括管理するマネージャークラス
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	/*
	 * @brief 3Dモデルファイルをキャッシュ読み込みする（二重ロード時は複製モデルを返す）
	 * [入力] pathName: アセットファイルパス
	 * [出力] 複製されたモデルハンドル（エラー時は -1）
	 * [副作用] 新規ロード時はresourceMapListにオリジナルハンドルを追加登録
	 */
	int LoadModel(std::string pathName);
	void PreloadModel(std::string pathName);

	/*
	 * @brief 2D画像ファイルをキャッシュ読み込みする（二重ロード時は既存のハンドルを返す）
	 * [入力] pathName: アセットファイルパス
	 * [出力] 画像グラフィックハンドル（エラー時は -1）
	 * [副作用] 新規ロード時はgraphicResourceMapListに登録
	 */
	int LoadGraphics(std::string pathName);
	void PreloadGraphics(std::string pathName);

	/*
	 * @brief 分割画像ファイルをキャッシュ読み込みし、分割データ構造体を返す
	 * [入力] pathName: アセットファイルパス, allNum: 分割総数, numX: 横分割数, numY: 縦分割数
	 * [出力] 分割画像データ構造体へのポインタ（エラー時は nullptr）
	 * [副作用] 新規ロード時はdivGraphicResourceMapListに構造体を追加登録
	 */
	DivGraphData* LoadDivGraphics(std::string pathName, int allNum, int numX, int numY);

	/*
	 * @brief キャッシュ管理されているリソースの総数を取得する
	 * [入力] なし
	 * [出力] 登録されているリソースの合計数
	 * [副作用] なし
	 */
	int GetTotalResource() { return static_cast<int>(resourceMapList.size() + graphicResourceMapList.size() + divGraphicResourceMapList.size()); }

private:
	std::vector<std::pair<std::string, int>> resourceMapList;			// キャッシュ管理用モデルアセットリスト (パス名とオリジナルハンドルのペア)
	std::vector<std::pair<std::string, int>> graphicResourceMapList;	// キャッシュ管理用2Dグラフィックリスト
	std::vector<DivGraphData*> divGraphicResourceMapList;				// キャッシュ管理用分割画像データリスト
};

