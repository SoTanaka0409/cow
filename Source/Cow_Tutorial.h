#pragma once
#include "CowMove.h"
#include "dxlib.h"

// チュートリアルシーンで使用される特別な牛キャラクター（タグ: Cow_T）を制御するクラス
class Cow_Tutorial : public CowMove
{
public:
	/*
	 * @brief チュートリアル用の牛オブジェクトを初期位置に配置し、スコア・経験値を設定する
	 * [入力] filename: モデルのファイルパス, initPos: 初期スポーン座標
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_Tutorial(std::string filename, VECTOR initPos);
	virtual ~Cow_Tutorial();

	void Update() override;
};
