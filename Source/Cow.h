#pragma once
#include "CowMove.h"

// 一般的な牛キャラクター（タグ: Cow_1）を制御するクラス
class Cow : public CowMove
{
public:
	/*
	 * @brief 牛オブジェクトを初期位置に配置し、スコア・経験値を設定する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標, num: 回収時の基礎スコア値
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow(std::string filename, VECTOR initPos, float num);
	virtual ~Cow();

	void Update() override;
};
