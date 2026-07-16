#include "Model.h"
#include "Master.h"

/*
 * 入力: filename (モデルパス), initPos (初期座標), isSeparateAnimation (アニメーション分離フラグ)
 * 出力: なし
 * 副作用: 内部変数の初期化、およびリソースマネージャ経由でのモデルハンドルの取得
 */
Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
	: position_(initPos)
	, scale_(1.0f)
	, change_texture_handle_(-1)
{
	// 同一モデルの複数生成時にVRAMを圧迫するのを防ぐため、独自のリソースマネージャ経由でハンドルを共有取得する
	handle_ = Master::resource_manager_->LoadModel(filename.c_str());
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 保持しているモデルハンドルの破棄
 */
Model::~Model()
{
	if (change_texture_handle_ != -1)
	{
		// 二重解放によるクラッシュや他オブジェクトのテクスチャ消失バグを防ぐため、
		// 独自ロードしたテクスチャ(change_texture_handle_)の破棄はResourceManager側の管理に一任する
	}
	MV1DeleteModel(handle_);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 座標および回転のライブラリ側への同期
 */
void Model::Update()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 3Dモデルの描画
 */
void Model::Draw()
{
	// 外部仕様依存: DxLibの仕様上、他オブジェクトの半透明設定がグローバルステートとして残留し、
	// 意図せずモデルが透けて描画されるバグを防ぐため、描画直前にアルファ乗算を明示的に無効化する
	MV1SetUseDrawMulAlphaColor(handle_, FALSE);
	MV1DrawModel(handle_);
}

/*
 * 入力: scale (XYZ各軸のスケール倍率)
 * 出力: なし
 * 副作用: 3Dモデルの拡縮設定の適用
 */
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(handle_, scale);
}

/*
 * 入力: scale (全体に対する一律のスケール倍率)
 * 出力: なし
 * 副作用: 3Dモデルの拡縮設定の適用
 */
void Model::SetScale(float scale)
{
	SetScale(VGet(scale, scale, scale));
}

/*
 * 入力: filename (テクスチャパス), index (差し替え対象のテクスチャ番号)
 * 出力: なし
 * 副作用: 差し替え用テクスチャのロードと適用
 */
void Model::SetTexture(std::string filename, int index)
{
	if (change_texture_handle_ != -1)
	{
		// デストラクタ同様、リソースマネージャが管理するメモリ領域の安全性を担保するため明示的な破棄は行わない
	}
	change_texture_handle_ = Master::resource_manager_->LoadGraphics(filename);
	MV1SetTextureGraphHandle(handle_, index, change_texture_handle_, FALSE);
}

/*
 * 入力: r, g, b, a (適用する色のRGBA成分 0.0f～1.0f)
 * 出力: なし
 * 副作用: モデル内の全マテリアルに対するディフューズ(拡散反射)カラーの適用
 */
void Model::SetColor(float r, float g, float b, float a)
{
	int matNum = MV1GetMaterialNum(handle_);

	// 複数パーツで構成されるモデルにおいて、特定の部位だけ元の色が残り不自然な見た目になるのを防ぐため、
	// 内部の全マテリアルを走査して指定色で強制上書きする
	for (int i = 0; i < matNum; ++i)
	{
		MV1SetMaterialDifColor(handle_, i, GetColorF(r, g, b, a));
	}
}