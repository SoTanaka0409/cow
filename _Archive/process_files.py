import os
import re

files = [
    'Scene3D.h','SceneManager.cpp','SceneManager.h','Score.cpp','Score.h',
    'SelectionManager.cpp','SelectionManager.h','ServiceLocator.cpp','ServiceLocator.h',
    'Skill.cpp','Skill.h','SkyBox.cpp','SkyBox.h','SoundManager.cpp','SoundManager.h',
    'SphereCollider.cpp','SphereCollider.h','Stage.cpp','Stage.h','Texture.cpp','Texture.h'
]

why_keywords = ["ため", "ので", "から", "防ぐ", "保証", "回避", "対策", "仕様", "考慮", "制限", "防止", "軽減", "意図", "目的"]

mojibake_dict = {
    "スチE?Eジ": "ステージ",
    "スチEEジ": "ステージ",
    "管琁EめE": "管理する",
    "管?E": "管理",
    "管琁E": "管理",
    "巨大匁E": "巨大化",
    "スポ?Eン": "スポーン",
    "スポEン": "スポーン",
    "初期匁E": "初期化",
    "終?E?E?E": "終了処理",
    "終亁E": "終了処理",
    "環?E??化": "環境変化",
    "処?E": "処理",
    "半征E(予?E": "半径(予測)",
    "半征E": "半径",
    "タイマ?E": "タイマー",
    "タイマE": "タイマー",
    "大?E現": "大量出現",
    "アクチE??ブ": "アクティブ",
    "アクチEブ": "アクティブ",
    "安?E": "安全",
    "破?E??": "破棄し、",
    "?E期化": "初期化",
    "変わってぁE??ぁE??合?E": "変わっていない場合は",
    "?E??替え": "切り替え",
    "生?E": "生成",
    "負数にならなぁEぁE": "負数にならないように",
    "加箁E": "加算",
    "リセチE": "リセット",
    "設宁E": "設定",
    "タイムアチEE後EネEムエントリー開始": "タイムアップ後のネームエントリー開始",
    "タイムアチE?E時?Eリザルト?E移": "タイムアップ時のリザルト画面遷移",
    "開姁E": "開始",
    "上佁E佁E": "上位3位",
    "保孁E": "保存",
    "作E": "作成",
    "チEチE": "テキスト",
    "バチEァ": "バッファ",
    "めE": "る",
    "ぁE": "い",
    "琁E": "理",
    "老E?E": "考慮",
    "特?E": "特定",
    "収雁E": "収集",
    "持E": "指定",
    "頁E": "項目",
    "インチEクス": "インデックス",
    "チEスチャ": "テクスチャ",
    "カーチEE": "カード(",
    "ꍇ": "各",
    "！E": "（",
    "？E": "）",
    "Ȃ": "なし",
    "蜈･蜉": "入力",
    "蜃ｺ蜉": "出力",
    "蜑ｯ菴懃畑": "副作用",
    "タイムアチの?の時?のリザルト?の移": "タイムアップ時のリザルト画面遷移",
    "負数にならないいコアを加算": "負数にならないようにスコアを加算",
    "scoreるに設定": "scoreを0に設定",
    "タイムアチEE後E": "タイムアップ後",
    "読み込みE不在時E初期化（": "読み込み（不在時は初期化）",
    "任意E数値を画像": "任意の数値を画像",
    "キー入力ハンドルの有効匁E": "キー入力ハンドルの有効化",
    "のNTのR": "ENTER",
    "rankingの更新、rank.txtの作E": "rankingの更新、rank.txtの作成",
    "ランキングのチEチE描画": "ランキングのテキスト描画",
    "ランキング(上位3位に登録": "ランキング（上位3位）に登録",
    "プレイ中のスコア描画": "プレイ中のスコア描画",
    "座樁E": "座標"
}

def fix_mojibake(text):
    for k, v in mojibake_dict.items():
        text = text.replace(k, v)
    text = text.replace('\ufffd', '')
    return text.strip()

def decode_smart(b):
    if b.startswith(b'\xff\xfe'): return b.decode('utf-16-le')
    try: return b.decode('utf-8')
    except: pass
    try:
        s = b.decode('shift_jis')
        if '繝' in s or '蜈' in s or '蜑' in s:
            return b.decode('utf-8', errors='replace')
        return s
    except: pass
    s = b.decode('shift_jis', errors='replace')
    if '繝' in s or '蜈' in s or '蜑' in s:
        return b.decode('utf-8', errors='replace')
    return b.decode('utf-8', errors='replace')

def process_file(path):
    with open(path, 'rb') as f:
        b = f.read()
    
    text = decode_smart(b)
    text = text.replace('\r\n', '\n')
    
    def block_repl(m):
        block = m.group(0)
        if '@brief' in block or '[入力]' in block or '蜈･蜉' in block or '[出力]' in block or '蜃ｺ蜉' in block:
            desc, in_desc, out_desc, side_desc = "", "なし", "なし", "なし"
            lines = block.split('\n')
            for l in lines:
                l = l.strip(' \t/*')
                if not l: continue
                l = l.replace('蜈･蜉', '入力').replace('蜃ｺ蜉', '出力').replace('蜑ｯ菴懃畑', '副作用')
                l = l.replace('Ȃ', 'なし').replace('なぁE', 'なし').replace('なし', 'なし')
                l = fix_mojibake(l)
                
                if '@brief' in l:
                    desc = l.replace('@brief', '').strip()
                elif '入力]' in l:
                    m_in = re.search(r'\[入力\](.*?)\[', l)
                    if m_in: in_desc = m_in.group(1).strip()
                    else: 
                        m_in2 = re.search(r'\[入力\](.*)', l)
                        if m_in2: in_desc = m_in2.group(1).strip()
                    m_out = re.search(r'\[出力\](.*?)\[', l)
                    if m_out: out_desc = m_out.group(1).strip()
                    else:
                        m_out2 = re.search(r'\[出力\](.*)', l)
                        if m_out2: out_desc = m_out2.group(1).strip()
                    m_side = re.search(r'\[副作用\](.*)', l)
                    if m_side: side_desc = m_side.group(1).strip()
                elif '出力]' in l:
                    out_desc = l.split(']')[1].strip()
                elif '副作用]' in l:
                    side_desc = l.split(']')[1].strip()
                elif not desc and not '[' in l:
                    desc = l.strip()
            
            desc = fix_mojibake(desc)
            in_desc = fix_mojibake(in_desc)
            out_desc = fix_mojibake(out_desc)
            side_desc = fix_mojibake(side_desc)
            
            if not desc: desc = "処理を行う"
            return f"/*\n * {desc}\n * [入力] {in_desc}\n * [出力] {out_desc}\n * [副作用] {side_desc}\n */"
        return block
    
    text = re.sub(r'/\*.*?\*/', block_repl, text, flags=re.DOTALL)
    
    new_lines = []
    for line in text.split('\n'):
        if '//' in line:
            parts = line.split('//', 1)
            code_part = parts[0]
            comment_part = parts[1].strip()
            
            # keep if why keywords are in it, and we delete TODOs or purely obvious
            keep = any(w in comment_part for w in why_keywords) and 'TODO' not in comment_part.upper()
            
            if keep:
                comment_part = fix_mojibake(comment_part)
                if code_part.strip():
                    new_lines.append(f"{code_part}// {comment_part}")
                else:
                    new_lines.append(f"{parts[0]}// {comment_part}")
            else:
                if code_part.strip():
                    new_lines.append(code_part.rstrip())
        else:
            new_lines.append(line)
            
    final_text = '\n'.join(new_lines)
    
    with open(path, 'w', encoding='utf-8-sig') as f:
        f.write(final_text)

for f in files:
    path = os.path.join('Source', f)
    if os.path.exists(path):
        process_file(path)

print("Done")
