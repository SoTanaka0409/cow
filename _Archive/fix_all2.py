import os
import re

files = [
    'Floor.h', 'GameConstants.h', 'GameManager.cpp', 'GameManager.h',
    'GameTimer.cpp', 'GameTimer.h', 'GoldCow.cpp', 'GoldCow.h',
    'InputManager.cpp', 'InputManager.h', 'Level.cpp', 'Level.h',
    'LoadingScene.cpp', 'LoadingScene.h', 'Main.cpp', 'Master.h',
    'Model.cpp', 'Model.h', 'ModelAnimation.cpp', 'ModelAnimation.h',
    'ModelUtility.h'
]

for f in files:
    path = os.path.join('Source', f)
    if not os.path.exists(path): continue
    
    with open(path, 'rb') as file:
        data = file.read()
    try: text = data.decode('utf-8')
    except: text = data.decode('cp932', errors='ignore')

    lines = text.split('\n')
    new_lines = []
    for line in lines:
        if '//' in line:
            comment_part = line[line.find('//'):]
            if '繝' in comment_part or '' in comment_part or 'E' in comment_part or '\ufffd' in comment_part:
                line = line[:line.find('//')].rstrip()
        new_lines.append(line)
        
    text = '\n'.join(new_lines)
    
    with open(path, 'w', encoding='utf-8-sig') as file:
        file.write(text)
        
print("Cleaned up remaining garbled comments.")
