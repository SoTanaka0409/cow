#pragma once
#include "CowMove.h"
#include "dxlib.h"

/// @brief チュートリアル時の特別な挙動（スコア高め等）を処理するため
class Cow_Tutorial : public CowMove
{
public:
	/// @brief チュートリアル用の固有パラメータを設定するため
	/// @param filename モデルファイルパス
	/// @param initPos 初期座標
	Cow_Tutorial(std::string filename, VECTOR initPos);

	/// @brief リソース解放によりメモリリークを防ぐため
	virtual ~Cow_Tutorial();
};
