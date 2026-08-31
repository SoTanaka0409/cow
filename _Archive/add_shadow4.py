import sys

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
        if old not in content:
            print(f"Warning: '{old[:40]}' not found in {path}")
        content = content.replace(old, new)
        
    with open(path, 'w', encoding=used_encoding) as f:
        f.write(content)
    print(f"Modified {path} using {used_encoding}")

modify_file('Source/Scene3D.cpp', [
    ('''Master::mpSoundManager->SetBGMVolume(120);

	auto objMgr = ServiceLocator::GetObjectManager();
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dStage)) {
		obj->SetUseShadowMap(shadow_map_handle_);
	}
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dPlayer)) {
		obj->SetUseShadowMap(shadow_map_handle_);
	}
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dAnimal)) {
		obj->SetUseShadowMap(shadow_map_handle_);
	}
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dCow)) {
		obj->SetUseShadowMap(shadow_map_handle_);
	}''',
     'Master::mpSoundManager->SetBGMVolume(120);\n\n\tSetUseShadowMap(0, shadow_map_handle_);')
])

