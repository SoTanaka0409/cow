import os
import glob
import re

def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    original_content = content
    
    # 1. Player
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player)", "ServiceLocator::GetPlayer()")
    
    # 2. CowManager
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->mpCowManager", "ServiceLocator::GetCowManager()")
    
    # 3. AnimalManager
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->mpAnimalManager", "ServiceLocator::GetAnimalManager()")

    # 4. GameManager
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->mpGameManager", "ServiceLocator::GetGameManager()")
    
    # 5. Fever
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->mpFever", "ServiceLocator::GetFever()")
    
    # 6. ObjectManager
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->GetObjectManager()", "ServiceLocator::GetObjectManager()")
    
    # 7. CollisionManager
    content = content.replace("Master::mpSceneManager->GetCurrentScene()->GetCollisionManager()", "ServiceLocator::GetCurrentScene()->GetCollisionManager()")

    if content != original_content:
        # Add #include "ServiceLocator.h" if not present
        if '#include "ServiceLocator.h"' not in content and '#include"ServiceLocator.h"' not in content:
            # Find the last include to insert after it
            last_include_pos = content.rfind('#include')
            if last_include_pos != -1:
                end_of_line = content.find('\n', last_include_pos)
                content = content[:end_of_line+1] + '#include "ServiceLocator.h"\n' + content[end_of_line+1:]
            else:
                content = '#include "ServiceLocator.h"\n' + content
                
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Refactored: {filepath}")

for filepath in glob.glob('Source/**/*.cpp', recursive=True):
    # ServiceLocator.cpp は対象外
    if 'ServiceLocator.cpp' not in filepath:
        process_file(filepath)
