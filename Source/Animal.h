#pragma once
#include "AnimalMove.h"

// ステージ上に出現する一般の動物（牛以外）を制御するクラス
class Animal : public AnimalMove
{
public:
	/*
	 * @brief 動物オブジェクトを初期化し、初期座標にモデルを配置する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] なし
	 */
	Animal(std::string filename, VECTOR initPos);
	virtual ~Animal();

	/*
	 * @brief 移動、コライダー位置合わせ、モデル更新を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update() override;
};

