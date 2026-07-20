#pragma once
#include "CowMove.h"
#include "dxlib.h"

// チュートリアル時の特別な挙動（スコア高め等）を処理するため
class Cow_Tutorial : public CowMove
{
public:
	/*
	 * チュートリアル用の固有パラメータを設定するため
	 * [入力] filename: モデルファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_Tutorial(std::string filename, VECTOR initPos);

	/*
	 * リソース解放によりメモリリークを防ぐため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Cow_Tutorial();
};
