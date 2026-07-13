import re
import os

files_to_process = {
    'Source/CowMove.cpp': [
        (re.compile(r'for\s*\(\s*int\s+i\s*=\s*0;\s*i\s*<\s*cows\.size\(\);\s*\+\+i\s*\)\s*\{\s*CowMove\*\s*otherCow\s*=\s*dynamic_cast<CowMove\*>\(cows\.at\(i\)\);'), 
         r'for (auto& obj : cows)\n\t{\n\t\tCowMove* otherCow = dynamic_cast<CowMove*>(obj);')
    ],
    'Source/Player3D.cpp': [
        (re.compile(r'for\s*\(\s*int\s+i\s*=\s*0;\s*i\s*<\s*walls\.size\(\);\s*i\+\+\s*\)\s*\{\s*Wall\*\s*wall\s*=\s*dynamic_cast<Wall\*>\(walls\.at\(i\)\);'), 
         r'for (auto& w : walls)\n\t\t{\n\t\t\tWall* wall = dynamic_cast<Wall*>(w);')
    ],
    'Source/CharacterMove.cpp': [
        (re.compile(r'for\s*\(\s*int\s+i\s*=\s*0;\s*i\s*<\s*walls\.size\(\);\s*i\+\+\s*\)\s*\{\s*Wall\*\s*wall\s*=\s*dynamic_cast<Wall\*>\(walls\.at\(i\)\);'), 
         r'for (auto& w : walls)\n\t\t{\n\t\t\tWall* wall = dynamic_cast<Wall*>(w);')
    ]
}

base_dir = r'c:\Users\student\Desktop\programing\cow'

for rel_path, rules in files_to_process.items():
    path = os.path.join(base_dir, rel_path)
    if not os.path.exists(path):
        continue
        
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
        
    orig_content = content
    for pattern, repl in rules:
        content = pattern.sub(repl, content)
        
    if content != orig_content:
        with open(path, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Updated {rel_path}")
    else:
        print(f"No changes for {rel_path}")

