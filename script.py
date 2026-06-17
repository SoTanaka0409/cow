# -*- coding: utf-8 -*-
import os
import re
import codecs

def read_file(path):
    encodings = ['utf-8', 'utf-8-sig', 'shift_jis', 'cp932']
    for enc in encodings:
        try:
            with codecs.open(path, 'r', enc) as f:
                return f.read()
        except UnicodeDecodeError:
            pass
    raise Exception("Failed to read " + path)

def write_file(path, content):
    with codecs.open(path, 'w', 'utf-8-sig') as f:
        f.write(content)

base_dir = r"c:\Users\student\Desktop\programing\cow\Source"

# 1. Cow.h
path = os.path.join(base_dir, "Cow.h")
content = read_file(path)
content = re.sub(r'//\s*一般的な牛キャラクター（タグ:\s*Cow_1）を制御するクラス', '// タグCow_1の標準キャラクター定義（基本行動はCowMoveに準拠）', content)
content = re.sub(r'/\*\s*\*\s*@brief\s*牛オブジェクトを初期位置に配置し、スコア・経験値を設定する\s*\*\s*\[入力\]\s*filename:\s*モデルのファイルパス,\s*initPos:\s*初期座標,\s*num:\s*回収時の基礎スコア値\s*\*\s*\[出力\]\s*なし\s*\*\s*\[副作用\]\s*なし\s*\*/',
    '/*\n\t * @brief オブジェクト初期化\n\t * [入力] filename: モデルパス, initPos: 初期配置座標, num: 基礎スコア値\n\t * [出力] なし\n\t * [副作用] スコアと経験値の設定\n\t */', content)
write_file(path, content)

# 2. Cow.cpp
path = os.path.join(base_dir, "Cow.cpp")
content = read_file(path)
write_file(path, content)

# 3. CowMove.h
path = os.path.join(base_dir, "CowMove.h")
content = read_file(path)
content = re.sub(r'//\s*ステージ上をAI自律移動する牛キャラクターの基底クラス', '// AIによる自律移動と吸い込み判定を管理する基底クラス', content)
content = re.sub(r'//\s*牛の種類識別用タグ', '// 個別仕様分岐のための牛種類識別用タグ', content)

content = re.sub(r'/\*\s*\*\s*@brief\s*牛キャラクターをロードし、初期状態をセットする\s*\*\s*\[入力\]\s*filename:\s*モデルのファイルパス,\s*initPos:\s*初期スポーン座標\s*\*\s*\[出力\]\s*なし\s*\*\s*\[副作用\]\s*モデルおよび吸引演出用Effekseerエフェクトのインスタンス生成\s*\*/',
    '/*\n\t * @brief 初期化\n\t * [入力] filename: モデルパス, initPos: 初期配置座標\n\t * [出力] なし\n\t * [副作用] Effekseerエフェクトのインスタンス生成\n\t */', content)

content = re.sub(r'/\*\s*\*\s*@brief\s*AIによる自律徘徊移動や、吸引された際の上昇同期処理などを実行する\s*\*/',
    '/*\n\t * @brief 移動処理\n\t * [入力] なし\n\t * [出力] なし\n\t * [副作用] 座標の更新、AI自律移動および吸引時上昇同期\n\t */', content)

content = re.sub(r'/\*\s*\*\s*@brief\s*吸引され限界高度に達した際、スコア・経験値をプレイヤーに加算して回収（消滅）する\s*\*/',
    '/*\n\t * @brief 限界高度到達時の消滅処理\n\t * [入力] なし\n\t * [出力] なし\n\t * [副作用] スコア・経験値の加算、エフェクト再生\n\t */', content)

content = re.sub(r'/\*\s*\*\s*@brief\s*餌の効果によってアクターに倒された時の回収（消滅）処理\s*\*/',
    '/*\n\t * @brief 餌によるキル処理\n\t * [入力] なし\n\t * [出力] なし\n\t * [副作用] 削除フラグの有効化\n\t */', content)

content = re.sub(r'/\*\s*\*\s*@brief\s*牛の死亡処理を統合\s*\*/',
    '/*\n\t * @brief 死亡時の共通処理\n\t * [入力] reason: 死亡理由\n\t * [出力] なし\n\t * [副作用] コンボ・スコアの計算、削除フラグの有効化\n\t */', content)

content = re.sub(r'Tag_cow\s+mntag_cow;\s*//\s*牛のタグ種類', 'Tag_cow mntag_cow;                  // 個別仕様分岐のためのタグ', content)
content = re.sub(r'float\s+mColliderRadius\s*=\s*50\.0f;\s*//\s*コライダーの判定半径', 'float mColliderRadius = 50.0f;      // 衝突判定用の半径制約', content)
content = re.sub(r'bool\s+mCowtDelete;\s*//\s*吸引消滅エフェクト終了後の完全削除フラグ', 'bool mCowtDelete;                   // エフェクト終了待機用削除フラグ', content)
content = re.sub(r'EffekseerEffect\*\s+mpCowVm;\s*//\s*吸引時に発生するエフェクト', 'EffekseerEffect* mpCowVm;           // 吸引演出用エフェクト', content)
content = re.sub(r'int\s+mEffectTimer;\s*//\s*吸引消滅エフェクトの再生残り時間タイマー', 'int mEffectTimer;                   // エフェクト再生完了までの待機フレーム', content)
content = re.sub(r'bool\s+TutrialVacumFlag;\s*//\s*チュートリアル判定フラグ', 'bool TutrialVacumFlag;              // チュートリアル用の特別な判定フラグ', content)

write_file(path, content)

# 4. CowMove.cpp
path = os.path.join(base_dir, "CowMove.cpp")
content = read_file(path)
content = re.sub(r'//\s*コンボロジック', '// 同種連続キルによるボーナススコア計算', content)
write_file(path, content)

# 5. Cow_2.h
path = os.path.join(base_dir, "Cow_2.h")
content = read_file(path)
content = re.sub(r'//\s*少し価値の高い中型の牛キャラクター（タグ:\s*Cow_2）を制御するクラス', '// タグCow_2用クラス（スコア等のパラメータが標準と異なるための拡張）', content)
content = re.sub(r'/\*\s*\*\s*@brief\s*中型の牛オブジェクトを初期座標に配置し、スコア・コライダー半径などを設定する\s*\*\s*\[入力\]\s*filename:\s*モデルのファイルパス,\s*initPos:\s*初期スポーン座標\s*\*\s*\[出力\]\s*なし\s*\*\s*\[副作用\]\s*なし\s*\*/',
    '/*\n\t * @brief 初期化\n\t * [入力] filename: モデルパス, initPos: 初期配置座標\n\t * [出力] なし\n\t * [副作用] 個別のスコア・コライダー半径の設定\n\t */', content)
write_file(path, content)

# 6. Cow_2.cpp
path = os.path.join(base_dir, "Cow_2.cpp")
content = read_file(path)
write_file(path, content)

print("Done")
