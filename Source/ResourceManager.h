#pragma once
#include <vector>
#include <string>
#include"Dxlib.h"

struct DivGraphData
{
	std::string file_path_;    // 画像のファイルパス
	int* div_handle_list_;     // 分割された画像ハンドルの配列
	int div_x_;                // 水平方向の分割数
	int div_y_;                // 垂直方向の分割数
	int all_num_;              // 分割画像の総数

	// [入力] file_path_: パス, div_x_: 横分割数, div_y_: 縦分割数, all_num_: 総数 [出力] なし [副作用] ヒープ領域への配列確保
	// 生成時に動的確保を行うため、ライフサイクルの管理（所有権）に注意すること
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

	// [入力] なし [出力] なし [副作用] 確保したハンドル配列のメモリ解放
	// メモリリーク（解放漏れ）を防ぐため、newで確保した配列を確実にdelete[]する
	~DivGraphData()
	{
		delete[] div_handle_list_;
	}
};

// アセットの二重ロードによるVRAM枯渇や処理落ち（スパイク）を防ぐため、ロード済みリソースのキャッシュと共有化を一元管理するクラス
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// [入力] pathName: ファイルパス [出力] 複製モデルハンドル(エラー時-1) [副作用] オリジナルハンドルのロードとキャッシュ登録
	// モデルは姿勢(アニメーション等)を個別に持つため、同じパスでも参照ではなくMV1DuplicateModelによるクローン（複製）を返す仕様にしている
	int LoadModel(std::string pathName);

	// [入力] pathName: ファイルパス [出力] なし [副作用] オリジナルハンドルのロードとキャッシュ登録
	// プレイ中の動的ロードによるフレーム落ち（スパイク）を防ぐため、シーン初期化時などの安全なタイミングで事前読み込みを行う
	void PreloadModel(std::string pathName);

	// [入力] pathName: ファイルパス [出力] 画像ハンドル(エラー時-1) [副作用] 画像のロードとキャッシュ登録
	// 2D画像は個別の状態を持たないため、VRAM節約を優先し、複製ではなくキャッシュした同じハンドル（参照）を使い回す
	int LoadGraphics(std::string pathName);

	// [入力] pathName: ファイルパス [出力] なし [副作用] 画像のロードとキャッシュ登録
	// 非同期ロード未対応のため、ゲームプレイ中の呼び出しは避け、必ずロード画面等で読み込みを完了させること
	void PreloadGraphics(std::string pathName);

	// [入力] pathName: パス, all_num_: 総数, numX: 横分割数, numY: 縦分割数 [出力] 分割画像データへのポインタ(エラー時nullptr) [副作用] 画像ロードとキャッシュ登録
	// 異なる分割数で同じパスをロードした場合の不整合バグを考慮していないため、アセットごとに分割数は一意である前提で運用すること
	DivGraphData* LoadDivGraphics(std::string pathName, int all_num_, int numX, int numY);

	// [入力] なし [出力] 登録リソース総数 [副作用] なし
	// 主にデバッグ時のメモリ使用量見積もりや、シーン遷移時のリーク検知の指標として用いる
	int GetTotalResource() { return static_cast<int>(resource_map_list_.size() + graphic_resource_map_list_.size() + div_graphic_resource_map_list_.size()); }

private:
	std::vector<std::pair<std::string, int>> resource_map_list_;         // 3Dモデルのパスとオリジナルハンドルのペアリスト
	std::vector<std::pair<std::string, int>> graphic_resource_map_list_; // 2D画像のパスとハンドルのペアリスト
	std::vector<DivGraphData*> div_graphic_resource_map_list_;           // 分割画像データのリスト
};
