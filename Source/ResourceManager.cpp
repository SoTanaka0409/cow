#include "DxLib.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// 登録されたすべてのオリジナルモデルハンドルを破棄
	for (int i = 0; i < resourceMapList.size(); i++)
	{
		MV1DeleteModel(resourceMapList.at(i).second);
	}

	// 確保した分割画像メタデータ構造体オブジェクトをメモリから解放
	for (int i = 0; i < divGraphicResourceMapList.size(); i++)
	{
		delete divGraphicResourceMapList.at(i);
	}
	divGraphicResourceMapList.clear();

	MV1InitModel();
	InitGraph(); // DxLibに読み込まれているすべての画像を一括削除
}

/*
 * @brief 3Dモデルファイルをキャッシュ読み込みする（二重ロード時は複製モデルを返す）
 * [入力] pathName: モデルファイルのファイルパス
 * [出力] 複製されたモデルハンドル (エラー時は-1)
 * [副作用] 新規ロード時はオリジナルとしてresourceMapListに追加
 */
int ResourceManager::LoadModel(std::string pathName)
{
	// 既に同じモデルがロード済みの場合は、そのオリジナルから複製を作成して返す
	for (int i = 0; i < resourceMapList.size(); i++)
	{
		if (resourceMapList.at(i).first == pathName)
		{
			return MV1DuplicateModel(resourceMapList.at(i).second);
		}
	}

	// 未ロードの場合は新規でモデルファイルを読み込み
	int handle = MV1LoadModel(pathName.c_str());
	if (handle == -1)
	{
		return -1;
	}

	resourceMapList.push_back(std::pair<std::string, int>(pathName, handle));
	return MV1DuplicateModel(handle); // オリジナルは保管し、複製側をゲーム側で使用させる
}

/*
 * @brief 2D画像ファイルをキャッシュ読み込みする（二重ロード時は既存のハンドルを返す）
 * [入力] pathName: 画像ファイルのファイルパス
 * [出力] 画像グラフィックハンドル (エラー時は-1)
 * [副作用] 新規ロード時はキャッシュ用リストに追加
 */
int ResourceManager::LoadGraphics(std::string pathName)
{
	for (int i = 0; i < graphicResourceMapList.size(); i++)
	{
		if (graphicResourceMapList.at(i).first == pathName)
		{
			return graphicResourceMapList.at(i).second;
		}
	}

	int handle = LoadGraph(pathName.c_str());
	if (handle == -1)
	{
		return -1;
	}

	graphicResourceMapList.push_back(std::pair<std::string, int>(pathName, handle));
	return handle;
}

/*
 * @brief 分割画像ファイルをキャッシュ読み込みし、分割データ構造体を返す
 * [入力] pathName: 画像ファイルパス, allNum: 分割総数, numX: 横分割数, numY: 縦分割数
 * [出力] 分割データオブジェクトポインタ (エラー時はnullptr)
 * [副作用] 新規ロード時はメタデータ構造体を動的生成してキャッシュに追加
 */
DivGraphData* ResourceManager::LoadDivGraphics(std::string pathName, int allNum, int numX, int numY)
{
	for (int i = 0; i < divGraphicResourceMapList.size(); i++)
	{
		if (divGraphicResourceMapList.at(i)->filePath == pathName)
		{
			return divGraphicResourceMapList.at(i);
		}
	}

	// 分割処理を行うための基盤画像サイズ情報取得用として、一度一時的に画像をロードする
	int handle = LoadGraph(pathName.c_str());
	if (handle == -1)
	{
		return nullptr;
	}

	DivGraphData *data = new DivGraphData(
		pathName,
		numX, numY,
		allNum
	);

	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);
	DeleteGraph(handle); // サイズ取得後の一時ハンドルは不要なため解放

	// 指定されたグリッド分割数と1コマサイズを基に、分割画像として各ハンドルをロード
	int loadResult = LoadDivGraph(pathName.c_str(), allNum, numX, numY, sizeX / numX, sizeY / numY, data->divHandleList);
	if (loadResult == -1)
	{
		delete data;
		return nullptr;
	}

	divGraphicResourceMapList.push_back(data);
	return data;
}
