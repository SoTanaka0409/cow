import codecs
import re

def process_file_regex(path, replacements):
    encoding = 'utf-8-sig'
    try:
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
    except UnicodeDecodeError:
        encoding = 'shift_jis'
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
            
    for old_pattern, new_text in replacements:
        content = re.sub(old_pattern, new_text, content, flags=re.DOTALL)
        
    with codecs.open(path, 'w', encoding) as f:
        f.write(content)

# 1. Score.cpp
process_file_regex(r'Source\Score.cpp', [
    (r'void Score::Draw\(\)\r?\n\{\r?\n\s*int x = 100;', 'void Score::Draw()\r\n{\r\n\tint x = 20;')
])

# 2. Player3D.cpp
process_file_regex(r'Source\Player3D.cpp', [
    (r'int gaugeX = 50;', 'int gaugeX = 20;')
])

print("Done")
