#pragma once
#include "AnimalMove.h"

// 仕様制約: プレイヤーのターゲット対象（熊など）とは異なる、一般環境生物を定義する
// 環境生物特有の軽量なAIや衝突判定を管理するため、基底クラスから処理を分離
class Animal : public AnimalMove
{
public:
	// 入力: filename(モデルファイルパス), initPos(初期生成座標) | 出力: なし
	// 副作用: 外部モデルのロード処理が走るため、大量生成時のパフォーマンス低下に注意
	Animal(std::string filename, VECTOR initPos);

	// メモリリーク防止のため、ロードしたモデルや確保済みの物理リソースを破棄する
	virtual ~Animal();

	// 入力: なし | 出力: なし
	// 副作用: 内部座標とアニメーション状態を毎フレーム更新し、他オブジェクトとの衝突状態を変化させる
	void Update() override;
};
