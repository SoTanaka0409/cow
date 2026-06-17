#pragma once
#include "CowMove.h"
#include "dxlib.h"

// チュートリアル進行用の特殊調整（スコア高め等）を行う専用クラス
class Cow_Tutorial : public CowMove
{
public:
	/*
	 * @brief チュートリアル用の固有パラメータを設定し初期化する
	 * [入力] filename: モデルファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_Tutorial(std::string filename, VECTOR initPos);

	/*
	 * @brief デストラクタ
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Cow_Tutorial();
};
