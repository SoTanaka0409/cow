#pragma once
#include "AnimalMove.h"

// 仕様制約: プレイヤーのターゲット対象となる牛以外の一般動物を定義する
class Animal : public AnimalMove
{
public:
	/*
	 * @brief 動物オブジェクトの生成と初期配置
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] オブジェクトの初期化および内部パラメータの設定
	 */
	Animal(std::string filename, VECTOR initPos);
	virtual ~Animal();

	/*
	 * @brief オブジェクトの毎フレーム更新
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標やコライダーなどの内部状態更新
	 */
	void Update() override;
};
