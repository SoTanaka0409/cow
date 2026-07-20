#pragma once
#include "CowMove.h"

// Cow1型の標準的な敵キャラクターとして振る舞いを定義するため
class Cow : public CowMove
{
public:
	/*
	 * 標準の牛キャラクターとしてスコアと経験値を設定するため
	 * [入力] filename: モデルパス, initPos: 初期配置座標, num: 基礎スコア値
	 * [出力] なし
	 * [副作用] 基礎スコアと経験値を初期化
	 */
	Cow(std::string filename, VECTOR initPos, float num);

	/*
	 * 派生クラスでの安全なリソース解放を保証するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Cow();
};
