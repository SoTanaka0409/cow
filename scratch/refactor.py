import re
import os

files_to_process = {
    'Source/SoundManager.cpp': [
        (re.compile(r'for\s*\(auto\s+it\s*=\s*bgm_handle_list_\.begin\(\);\s*it\s*!=\s*bgm_handle_list_\.end\(\);\s*it\+\+\)'), 
         r'for (auto& pair : bgm_handle_list_)'),
        (re.compile(r'for\s*\(auto\s+it\s*=\s*se_handle_list_\.begin\(\);\s*it\s*!=\s*se_handle_list_\.end\(\);\s*it\+\+\)'), 
         r'for (auto& pair : se_handle_list_)'),
        (re.compile(r'it->second'), r'pair.second'),
        (re.compile(r'it->first'), r'pair.first')
    ],
    'Source/CowMove.cpp': [
        (re.compile(r'for\s*\(\s*int\s+i\s*=\s*0;\s*i\s*<\s*cows\.size\(\);\s*\+\+i\s*\)\s*\{\s*CowMove\*\s*cow\s*=\s*dynamic_cast<CowMove\*>\(cows\[i\]\);'), 
         r'for (auto& obj : cows) {\n\t\tCowMove* cow = dynamic_cast<CowMove*>(obj);')
    ],
    'Source/Player3D.cpp': [
        (re.compile(r'for\s*\(\s*int\s+i\s*=\s*0;\s*i\s*<\s*walls\.size\(\);\s*i\+\+\s*\)\s*\{\s*auto\s*w\s*=\s*walls\[i\];'), 
         r'for (auto& w : walls) {')
    ],
    'Source/CharacterMove.cpp': [
        (re.compile(r'for\s*\(\s*int\s+i\s*=\s*0;\s*i\s*<\s*walls\.size\(\);\s*i\+\+\s*\)\s*\{\s*auto\s*w\s*=\s*walls\[i\];'), 
         r'for (auto& w : walls) {')
    ]
}

base_dir = r'c:\Users\student\Desktop\programing\cow'

for rel_path, rules in files_to_process.items():
    path = os.path.join(base_dir, rel_path)
    if not os.path.exists(path):
        print(f"File not found: {path}")
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

