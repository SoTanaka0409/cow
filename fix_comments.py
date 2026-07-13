import os
import re

def clean_mojibake(text):
    # Regex for bad characters that replace them with good ones
    fixes = [
        (r'モチE[^a-zA-Z0-9_]{0,3}の', 'モデルの'),
        (r'モチE[^a-zA-Z0-9_]{0,3}フ', 'モデルフ'),
        (r'モチE[^a-zA-Z0-9_]{0,3}', 'モデル'),
        (r'なぁE[^a-zA-Z0-9_]{0,3}', 'なし'),
        (r'\?ȁE', 'なし'),
        (r'EȂ', 'なし'),
        (r'座樁E', '座標'),
        (r'座\?E', '座標'),
        (r'拡大玁E', '拡大率'),
        (r'チEEム', 'ビーム'),
        (r'エフェクチEE', 'エフェクトの'),
        (r'リセチE\?\?', 'リセット'),
        (r'チE\?\?チE\?\?晁EE', 'テスト用の'),
        (r'スチE\?Eタス', 'ステータス'),
        (r'スチEEEタス', 'ステータス'),
        (r'補正征EE', '補正後の'),
        (r'気絶時間\?E\?フレーム数\?E\?E', '気絶時間(フレーム数)'),
        (r'操作丁EE', '操作不能'),
        (r'甁EE', '生成'),
        (r'アクチE\?\?ブ', 'アクティブ'),
        (r'デフォルチEE', 'デフォルトの'),
        (r'相扁EE', '相手の'),
        (r'終亁E\?\?た', '終了した'),
        (r'ゲチE\?\?ー\?EセチE\?\?ー群', 'ゲッター・セッター群'),
        (r'1フレーム剁EE', '1フレーム前の'),
        (r'破棁E\?\?れる', '破棄される'),
        (r'スチE\?Eジ', 'ステージ'),
        (r'冁E\?\?', '内に'),
        (r'裁E\?\?', '用の'),
        (r'動皁E[^a-zA-Z0-9_]{0,3}保', '動的に確保'),
        (r'初期化すめE', '初期化する'),
        (r'オブジェクチEE', 'オブジェクトの'),
        (r'一允E\?\?琁E\?\?', '一元管理'),
        (r'管琁E\?\?スト', '管理リスト'),
        (r'処琁E\?\?', '処理、'),
        (r'降頁E\?\?', '降順'),
        (r'比輁E\?\?', '比較'),
        (r'遠ぁE\?\?E\?\?E\?\?E値', '遠い順のZ値'),
        (r'交差が開始した瞬間', '交差開始時'),
        (r'dlɂzANṼIIt𐧌䂷', '仕様により吸引アクションのオンオフを制御'),
        (r'X`E\?E\^XID\?E\?EIE\?\?LZɎgp\?E\?E', 'ステータスID（スキル加算判定に使用）'),
        (r'Ugʂɂ\?\{\^UIԂƕ``E\?E\^ێ\\', 'リザルト画面のボタンUIと描画データの保持'),
        (r'Q\[I\?\?ɍŏIXRAƃLO\\V\[', 'ゲーム終了時に最終スコアとランキングを表示するシーン'),
        (r'wiAXRAALOUI̕`R\}h\?E', '背景、スコア、ランキングUIの描画'),
        (r'莞ԌoߌɃ\^Cgʂւ̑Jڂv`E', '一定時間経過後にタイトル画面への遷移をチェック'),
        (r'gp摜nh̔j\?E\?\?BGM\~', '使用画像ハンドルの破棄とBGM停止'),
        (r'ŐVLÕ\[hƃtF\[h\?E', '最新ランキングのロードとフェードイン'),
        (r'\?E摜Eǂݍ݃`EE\^у\\\[Xnh\?Eێ\\\?E', '分割画像の読み込みデータとリソースハンドルの保持'),
        (r'摜AZ`Ẽt@CpX', '画像アセットのファイルパス'),
        (r'\?E\[hꂽOtB`Enh̔zE', 'ロードされたグラフィックハンドルの配列'),
        (r'E\?E', '分割数'),
        (r'\?E摜E', '総画像数'),
        (r'Ugʂɂ\?\{\^UIԂƕ``E\?E\^ێ\\', 'リザルト画面のボタンUIと描画データの保持'),
    ]
    for pattern, repl in fixes:
        text = re.sub(pattern, repl, text)
    return text

def rewrite_file(filepath):
    if not os.path.exists(filepath): return
    
    # Try different encodings
    content = None
    for enc in ['utf-8-sig', 'utf-8', 'cp932', 'shift_jis']:
        try:
            with open(filepath, 'r', encoding=enc) as f:
                content = f.read()
            break
        except:
            pass
    
    if content is None:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()

    # Clean mojibake
    content = clean_mojibake(content)

    # Clean trivial single line comments
    lines = content.split('\n')
    new_lines = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith('//'):
            if any(kw in stripped for kw in ['代入', '加算', 'ループ', 'TODO', '自明', '古い']):
                continue
        elif '//' in line:
            left, right = line.split('//', 1)
            if any(kw in right for kw in ['代入', '加算', 'ループ', 'TODO', '自明', '古い']):
                line = left.rstrip()
        new_lines.append(line)
    
    content = '\n'.join(new_lines)

    with open(filepath, 'w', encoding='utf-8-sig') as f:
        f.write(content)

files = [
    'Player3D.h', 'Player3D.cpp', 'ResourceManager.h', 'ResourceManager.cpp',
    'ResultScene.h', 'ResultScene.cpp', 'Rule.h', 'Rule.cpp',
    'Scene.h', 'Scene.cpp', 'Scene3D.cpp'
]

for f in files:
    rewrite_file('Source/' + f)
