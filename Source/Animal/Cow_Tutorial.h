#pragma once
#include "CowMove.h"
#include "dxlib.h"

/// @brief チュートリアル時の特別な挙動（スコア高め等）を処理するため
class Cow_Tutorial : public CowMove
{
public:
	/// @brief チュートリアル用の固有パラメータを設定するため
	/// @param filename モデルファイルパス
	/// @param init_pos 初期座標
	Cow_Tutorial(const std::string& filename, VECTOR init_pos);

	/// @brief リソース解放によりメモリリークを防ぐため
	virtual ~Cow_Tutorial();
};
