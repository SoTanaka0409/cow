import sys

def fix_animal():
    with open('Source/AnimalMove.cpp', 'r', encoding='utf-8-sig') as f:
        lines = f.readlines()
    for i in range(len(lines)):
        if 'player->mpScore->AddScore(mfScore);' in lines[i]:
            lines[i] = lines[i].replace('player->mpScore->AddScore(mfScore);', 'player->mpScore->AddScore(static_cast<int>(mfScore));')
    with open('Source/AnimalMove.cpp', 'w', encoding='utf-8-sig') as f:
        f.writelines(lines)

def fix_cow():
    with open('Source/CowMove.cpp', 'r', encoding='utf-8-sig') as f:
        lines = f.readlines()
    for i in range(len(lines)):
        if 'player->mpScore->AddScore(mfScore * player->combo_->GetMultiplier());' in lines[i]:
            lines[i] = lines[i].replace('player->mpScore->AddScore(mfScore * player->combo_->GetMultiplier());', 'player->mpScore->AddScore(static_cast<int>(mfScore * player->combo_->GetMultiplier()));')
    with open('Source/CowMove.cpp', 'w', encoding='utf-8-sig') as f:
        f.writelines(lines)

def fix_scene3d():
    with open('Source/Scene3D.cpp', 'r', encoding='cp932') as f:
        lines = f.readlines()
    for i in range(len(lines)):
        if 'VGet(' in lines[i] and 'Object_Stage' in lines[i]:
            # Replace some common int patterns in VGet with floats
            lines[i] = lines[i].replace('6000, 0', '6000.0f, 0.0f')
            lines[i] = lines[i].replace('-6000, 0', '-6000.0f, 0.0f')
            lines[i] = lines[i].replace('5500, 0', '5500.0f, 0.0f')
            lines[i] = lines[i].replace('-5500, 0', '-5500.0f, 0.0f')
            lines[i] = lines[i].replace('-5000 + 2000 * i', '-5000.0f + 2000.0f * i')
            lines[i] = lines[i].replace('-5000 + 2000 * i, 0, -6000', '-5000.0f + 2000.0f * i, 0.0f, -6000.0f')
            lines[i] = lines[i].replace('-5000 + 2000 * i, 0, 6000', '-5000.0f + 2000.0f * i, 0.0f, 6000.0f')
            lines[i] = lines[i].replace('-4000 + 4000 * i', '-4000.0f + 4000.0f * i')
            
    with open('Source/Scene3D.cpp', 'w', encoding='cp932') as f:
        f.writelines(lines)

fix_animal()
fix_cow()
fix_scene3d()
print('Fixes applied.')
