import sys

def modify_file(path, replacements):
    with open(path, 'r', encoding='cp932') as f:
        content = f.read()
        
    for old, new in replacements:
        if old not in content:
            print(f"Warning: '{old[:40]}' not found in {path}")
        content = content.replace(old, new)
        
    with open(path, 'w', encoding='cp932') as f:
        f.write(content)
    print(f"Modified {path} using cp932")

modify_file('Source/Scene3D.cpp', [
    ('\tMaster::mpSoundManager->SetBGMVolume(120);',
     '\tMaster::mpSoundManager->SetBGMVolume(120);\n\tSetUseShadowMap(0, shadow_map_handle_);')
])

