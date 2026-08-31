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
    ('void Scene3D::Initialize()\n{\n\tScene::Initialize();',
     'void Scene3D::Initialize()\n{\n\tScene::Initialize();\n\n\tshadow_map_handle_ = MakeShadowMap(2048, 2048);\n\tSetShadowMapDrawArea(shadow_map_handle_, VGet(-6000.0f, -500.0f, -6000.0f), VGet(6000.0f, 2000.0f, 6000.0f));\n\tSetShadowMapLightDirection(shadow_map_handle_, VGet(-1.0f, -2.0f, -1.0f));'),
    ('Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);',
     'Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);\n\n\tauto objMgr = ServiceLocator::GetObjectManager();\n\tfor (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dStage)) {\n\t\tobj->SetUseShadowMap(shadow_map_handle_);\n\t}\n\tfor (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dPlayer)) {\n\t\tobj->SetUseShadowMap(shadow_map_handle_);\n\t}\n\tfor (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dAnimal)) {\n\t\tobj->SetUseShadowMap(shadow_map_handle_);\n\t}\n\tfor (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dCow)) {\n\t\tobj->SetUseShadowMap(shadow_map_handle_);\n\t}'),
    ('void Scene3D::Draw()\n{\n\tScene::Draw();',
     'void Scene3D::Draw()\n{\n\tDrawShadowMap();\n\n\tScene::Draw();'),
    ('void Scene3D::Finalize()\n{\n\tMaster::mpSoundManager->StopBGM(SoundManager::kBgmGame);',
     'void Scene3D::Finalize()\n{\n\tif (shadow_map_handle_ != -1) {\n\t\tDeleteShadowMap(shadow_map_handle_);\n\t\tshadow_map_handle_ = -1;\n\t}\n\tMaster::mpSoundManager->StopBGM(SoundManager::kBgmGame);')
])

