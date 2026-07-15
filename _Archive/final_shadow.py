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
    ('void Scene3D::Initialize()\n{\n\tMaster::camera_->Initialize();',
     'void Scene3D::Initialize()\n{\n\tshadow_map_handle_ = MakeShadowMap(2048, 2048);\n\tSetShadowMapDrawArea(shadow_map_handle_, VGet(-6000.0f, -500.0f, -6000.0f), VGet(6000.0f, 2000.0f, 6000.0f));\n\tSetShadowMapLightDirection(shadow_map_handle_, VGet(-1.0f, -2.0f, -1.0f));\n\n\tMaster::camera_->Initialize();'),
    ('Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);\n\n\tMaster::mpSoundManager->SetBGMVolume(120);',
     'Master::mpSoundManager->PlayBGM(SoundManager::kBgmGame);\n\n\tMaster::mpSoundManager->SetBGMVolume(120);\n\n\tSetUseShadowMap(0, shadow_map_handle_);'),
    ('void Scene3D::Draw()\n{\n\tScene::Draw();',
     'void Scene3D::Draw()\n{\n\tDrawShadowMap();\n\n\tScene::Draw();'),
    ('void Scene3D::Finalize()\n{\n\tMaster::mpSoundManager->StopBGM();',
     'void Scene3D::Finalize()\n{\n\tif (shadow_map_handle_ != -1) {\n\t\tDeleteShadowMap(shadow_map_handle_);\n\t\tshadow_map_handle_ = -1;\n\t}\n\tMaster::mpSoundManager->StopBGM();')
])

with open('Source/Scene3D.cpp', 'a', encoding='cp932') as f:
    f.write('''
void Scene3D::DrawShadowMap()
{
	if (shadow_map_handle_ == -1) return;

	ShadowMap_DrawSetup(shadow_map_handle_);

	auto objMgr = ServiceLocator::GetObjectManager();
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dPlayer)) {
		obj->Draw();
	}
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dAnimal)) {
		obj->Draw();
	}
	for (auto obj : objMgr->GetObject3DListByTag(Object3D::kTag3dCow)) {
		obj->Draw();
	}

	ShadowMap_DrawEnd();
}
''')

