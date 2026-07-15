import sys, re

def modify_file(path, replacements):
    content = None
    encodings = ['utf-8-sig', 'utf-8', 'cp932']
    used_encoding = None
    for enc in encodings:
        try:
            with open(path, 'r', encoding=enc) as f:
                content = f.read()
            used_encoding = enc
            break
        except UnicodeDecodeError:
            continue
            
    if content is None:
        print(f"Failed to read {path}")
        return
        
    for old, new in replacements:
        if isinstance(old, str):
            if old not in content:
                print(f"Warning: '{old[:40]}' not found in {path}")
            content = content.replace(old, new)
        else: # regex
            content = old.sub(new, content)
            
    with open(path, 'w', encoding=used_encoding) as f:
        f.write(content)
    print(f"Modified {path} using {used_encoding}")

modify_file('Source/Scene3D.cpp', [
    (re.compile(r'\tauto objMgr = ServiceLocator::GetObjectManager\(\);\n\tfor.*?obj->SetUseShadowMap\(shadow_map_handle_\);\n\t\}\n', re.DOTALL),
     '')
])

