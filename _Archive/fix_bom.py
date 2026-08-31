import sys

content = """#pragma once
#include "CowMove.h"

// Cow1型の標準的な敵キャラクターとして振る舞いを定義するため
class Cow : public CowMove
{
public:
\t/*
\t * 標準の牛キャラクターとしてスコアと経験値を設定するため
\t * [入力] filename: モデルパス, initPos: 初期配置座標, num: 基礎スコア値
\t * [出力] なし
\t * [副作用] 基礎スコアと経験値を初期化
\t */
\tCow(std::string filename, VECTOR initPos, float num);

\t/*
\t * 派生クラスでの安全なリソース解放を保証するため
\t * [入力] なし
\t * [出力] なし
\t * [副作用] なし
\t */
\tvirtual ~Cow();
};
"""

with open(r'c:\Users\student\Desktop\programing\cow\Source\Cow.h', 'w', encoding='utf-8-sig') as f:
    f.write(content)
