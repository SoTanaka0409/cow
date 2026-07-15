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
            if not old.search(content):
                print(f"Warning: regex not matched in {path}")
            content = old.sub(new, content)
            
    with open(path, 'w', encoding=used_encoding) as f:
        f.write(content)
    print(f"Modified {path} using {used_encoding}")

modify_file('Source/CharacterMove.h', [
    (re.compile(r'float GetHitRadius\(\) { return radius_; }'),
     'float GetHitRadius() { return radius_; }\n\n\tvoid SetUseShadowMap(int shadowMapHandle) override;')
])

modify_file('Source/CharacterMove.cpp', [
    (re.compile(r'void CharacterMove::OnEnter\(Collider\* collider, Collider\* check\)\n\{'),
     'void CharacterMove::SetUseShadowMap(int shadowMapHandle)\n{\n\tif (model_ != nullptr) model_->SetUseShadowMap(shadowMapHandle);\n}\n\nvoid CharacterMove::OnEnter(Collider* collider, Collider* check)\n{')
])

modify_file('Source/Scene3D.cpp', [
    (re.compile(r'void Scene3D::Initialize\(\)\n\{\n\s*Scene::Initialize\(\);'),
     'void Scene3D::Initialize()\n{\n\tScene::Initialize();\n\n\tshadow_map_handle_ = MakeShadowMap(2048, 2048);\n\tSetShadowMapDrawArea(shadow_map_handle_, VGet(-6000.0f, -500.0f, -6000.0f), VGet(6000.0f, 2000.0f, 6000.0f));\n\tSetShadowMapLightDirection(shadow_map_handle_, VGet(-1.0f, -2.0f, -1.0f));'),
    (re.compile(r'void Scene3D::Finalize\(\)\n\{\n'),
     'void Scene3D::Finalize()\n{\n\tif (shadow_map_handle_ != -1) {\n\t\tDeleteShadowMap(shadow_map_handle_);\n\t\tshadow_map_handle_ = -1;\n\t}\n')
])
