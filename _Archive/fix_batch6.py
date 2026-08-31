# -*- coding: utf-8 -*-
import os
import re

def sync_docstrings(h_path, cpp_path):
    with open(h_path, 'r', encoding='utf-8-sig') as f:
        h_text = f.read()
    
    with open(cpp_path, 'r', encoding='utf-8-sig') as f:
        cpp_text = f.read()

    docstrings = re.findall(r'(\/\*[\s\S]*?\*\/)\s+(?:virtual\s+)?(?:[\w\<\>\:\*]+\s+)*?(\w+)\s*\(', h_text)
    
    doc_map = {func: doc for doc, func in docstrings}
    
    new_cpp_text = cpp_text
    
    for func, doc in doc_map.items():
        # Avoid duplicate doc
        if doc in new_cpp_text:
            continue
            
        # find where the function is defined: return_type ClassName::FuncName(
        # or ClassName::FuncName(
        
        # simple regex to find the start of the line that declares ClassName::FuncName(
        pattern = r'^([^\n]*\w+::' + func + r'\s*\()'
        
        def repl(m):
            return doc + '\n' + m.group(1)
            
        new_cpp_text = re.sub(pattern, repl, new_cpp_text, flags=re.MULTILINE)

    with open(cpp_path, 'w', encoding='utf-8-sig') as f:
        f.write(new_cpp_text)

files = [
    ('Source/Combo.h', 'Source/Combo.cpp'),
    ('Source/Cow.h', 'Source/Cow.cpp'),
    ('Source/CowManager.h', 'Source/CowManager.cpp'),
    ('Source/CowMove.h', 'Source/CowMove.cpp'),
    ('Source/Cow_2.h', 'Source/Cow_2.cpp'),
    ('Source/Cow_Tutorial.h', 'Source/Cow_Tutorial.cpp'),
    ('Source/DebugCamera.h', 'Source/DebugCamera.cpp'),
    ('Source/EffectManager.h', 'Source/EffectManager.cpp'),
    ('Source/EffekseerEffect.h', 'Source/EffekseerEffect.cpp'),
    ('Source/Fever.h', 'Source/Fever.cpp')
]

for h, cpp in files:
    try:
        sync_docstrings(h, cpp)
    except Exception as e:
        print(f"Failed {h}: {e}")
