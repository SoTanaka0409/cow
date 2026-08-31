import os

files_to_fix = [
    'Source/TitleScene.cpp',
    'Source/ResultScene.cpp',
    'Source/Score.cpp',
    'Source/GameTimer.cpp'
]

replacements = {
    '1?Epng': '1位.png',
    '2?Epng': '2位.png',
    '3?Epng': '3位.png',
    'スタートEタン1.png': 'スタートぼたん1.png',
    'せってぁEたん1.png': 'せっていぼたん1.png',
    'しゅぁEめEぼたん (1).png': 'しゅうりょうぼたん (1).png',
    'リザルチEpng': 'リザルト.png',
    'コンボ数': 'コンボ'
}

for file_path in files_to_fix:
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
            
        for old_str, new_str in replacements.items():
            content = content.replace(old_str, new_str)
            
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
            
        print(f"Fixed {file_path}")
    except Exception as e:
        print(f"Error {file_path}: {e}")

