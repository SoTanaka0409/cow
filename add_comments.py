import os
import re

headers = ['GameTimer.h', 'GoldCow.h', 'InputManager.h', 'Level.h', 'LoadingScene.h', 'ModelAnimation.h', 'ModelUtility.h']

def generate_docstring(func_name):
    action = func_name
    if 'Update' in func_name: return '/*\n\t * 状態の更新を行う。\n\t * [入力] なし\n\t * [出力] なし\n\t * [副作用] 内部状態が更新される\n\t */'
    if 'Draw' in func_name: return '/*\n\t * 描画処理を行う。\n\t * [入力] なし\n\t * [出力] なし\n\t * [副作用] 画面に描画される\n\t */'
    if 'Init' in func_name or func_name[0].isupper() and func_name == func_name.split('(')[0]: return '/*\n\t * 初期化を行う。\n\t * [入力] 必要なパラメータ\n\t * [出力] なし\n\t * [副作用] インスタンスの初期化\n\t */'
    return f'/*\n\t * {func_name}の処理を行う。\n\t * [入力] 必要なパラメータ\n\t * [出力] 戻り値\n\t * [副作用] なし\n\t */'

for h in headers:
    path = os.path.join('Source', h)
    if not os.path.exists(path): continue
    with open(path, 'r', encoding='utf-8-sig') as f:
        text = f.read()

    lines = text.split('\n')
    new_lines = []
    i = 0
    while i < len(lines):
        line = lines[i]
        # match a function declaration inside a class (basic heuristic)
        match = re.match(r'^(\s+)([\w\<\>\[\]\*]+\s+\w+\(.*?\);)', line)
        if match and not 'override' in line:
            indent = match.group(1)
            # check if there's a comment right above
            if i > 0 and '*/' not in lines[i-1] and '//' not in lines[i-1]:
                func_name = re.search(r'(\w+)\(', line).group(1) if re.search(r'(\w+)\(', line) else '関数'
                doc = generate_docstring(func_name).replace('\t', indent)
                new_lines.append(doc)
        
        # also match constructor
        match2 = re.match(r'^(\s+)(\w+\(.*?\);)', line)
        if match2 and not 'return' in line and not '=' in line:
            indent = match2.group(1)
            if i > 0 and '*/' not in lines[i-1] and '//' not in lines[i-1]:
                func_name = re.search(r'(\w+)\(', line).group(1) if re.search(r'(\w+)\(', line) else '関数'
                doc = generate_docstring(func_name).replace('\t', indent)
                new_lines.append(doc)
        
        new_lines.append(line)
        i += 1

    with open(path, 'w', encoding='utf-8-sig') as f:
        f.write('\n'.join(new_lines))

print("Added missing comments to headers.")
