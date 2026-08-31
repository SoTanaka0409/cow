#pragma once
#include "AnimalMove.h"

/// @brief 仕様制約: プレイヤーのターゲット対象（熊など）とは異なる、一般環境生物を定義する
/// @details 環境生物特有の軽量なAIや衝突判定を管理するため、基底クラスから処理を分離
class Animal : public AnimalMove
{
public:
	/// @brief 初期化処理を行う
	/// @details filename(モデルファイルパス)
	/// @details initPos(初期生成座標) | 出力: なし
	/// @details 外部モデルのロード処理が走るため、大量生成時のパフォーマンス低下に注意
	Animal(std::string filename, VECTOR initPos);

	/// @brief メモリリーク防止のため、ロードしたモデルや確保済みの物理リソースを破棄する
	virtual ~Animal();

	/// @brief なし | 出力: なし
	/// @details 内部座標とアニメーション状態を毎フレーム更新し、他オブジェクトとの衝突状態を変化させる
	void Update() override;
};
