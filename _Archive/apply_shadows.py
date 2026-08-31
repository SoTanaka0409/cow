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
            print(f"Warning: {old[:30]}... not found in {path}")
        content = content.replace(old, new)
        
    with open(path, 'w', encoding=used_encoding) as f:
        f.write(content)
    print(f"Modified {path} using {used_encoding}")

modify_file('Source/Object3D.h', [
    ('virtual void OnExit(Collider* collider, Collider* check);', 
     'virtual void OnExit(Collider* collider, Collider* check);\n\n\tvirtual void SetUseShadowMap(int shadowMapHandle) {}')
])

modify_file('Source/Model.h', [
    ('void SetColor(float r, float g, float b, float a = 1.0f);',
     'void SetColor(float r, float g, float b, float a = 1.0f);\n\n\tvoid SetUseShadowMap(int shadowMapHandle);')
])

modify_file('Source/Model.cpp', [
    ('void Model::SetColor(float r, float g, float b, float a)',
     'void Model::SetUseShadowMap(int shadowMapHandle)\n{\n\tMV1SetUseShadowMap(mnHandle, 0, shadowMapHandle);\n}\n\nvoid Model::SetColor(float r, float g, float b, float a)')
])

modify_file('Source/Object_Stage.h', [
    ('void SetColor(float r, float g, float b, float a = 1.0f);',
     'void SetColor(float r, float g, float b, float a = 1.0f);\n\tvoid SetUseShadowMap(int shadowMapHandle) override;')
])

modify_file('Source/Object_Stage.cpp', [
    ('void Object_Stage::SetColor(float r, float g, float b, float a)',
     'void Object_Stage::SetUseShadowMap(int shadowMapHandle)\n{\n\tif (model_ != nullptr) model_->SetUseShadowMap(shadowMapHandle);\n}\n\nvoid Object_Stage::SetColor(float r, float g, float b, float a)')
])

modify_file('Source/CharacterMove.h', [
    ('virtual void OnTrigger(Collider* collider, Collider* check) override;',
     'virtual void OnTrigger(Collider* collider, Collider* check) override;\n\tvoid SetUseShadowMap(int shadowMapHandle) override;')
])

modify_file('Source/CharacterMove.cpp', [
    ('void CharacterMove::OnTrigger(Collider* collider, Collider* check)',
     'void CharacterMove::SetUseShadowMap(int shadowMapHandle)\n{\n\tif (model_ != nullptr) model_->SetUseShadowMap(shadowMapHandle);\n}\n\nvoid CharacterMove::OnTrigger(Collider* collider, Collider* check)')
])

modify_file('Source/Scene3D.h', [
    ('void PhaseUpdate();',
     'void PhaseUpdate();\n\n\tvoid DrawShadowMap();'),
    ('int font_back_graph_;',
     'int font_back_graph_;\n\n\tint shadow_map_handle_;')
])

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

print("All modifications done.")
