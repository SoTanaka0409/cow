import codecs

def process_file(path, replacements):
    # Detect encoding
    encoding = 'utf-8-sig'
    try:
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
    except UnicodeDecodeError:
        encoding = 'shift_jis'
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
            
    for old, new in replacements:
        content = content.replace(old, new)
        
    with codecs.open(path, 'w', encoding) as f:
        f.write(content)

# 1. Update CharacterMove.h
process_file(r'Source\CharacterMove.h', [
    ('class CharacterState;', 'class CharacterState;\r\nclass Player3D;'),
    ('class CharacterMove : public Object3D\r\n{\r\npublic:', 'class CharacterMove : public Object3D\r\n{\r\npublic:\r\n\tvoid SetTargetPlayer(Player3D* player) { mpTargetPlayer = player; }\r\n\tPlayer3D* GetTargetPlayer() const { return mpTargetPlayer; }'),
    ('protected:\r\n\tModel* mpModel;', 'protected:\r\n\tPlayer3D* mpTargetPlayer = nullptr;\r\n\tModel* mpModel;')
])

# 2. Update CharacterMove.cpp
process_file(r'Source\CharacterMove.cpp', [
    ('mCurrentState = STATE_IDLE;', 'mCurrentState = STATE_IDLE;\r\n\tmpTargetPlayer = nullptr;')
])

# 3. Update Player3D.cpp
process_file(r'Source\Player3D.cpp', [
    ('cow->IncreaseVacuumTimer();', 'cow->SetTargetPlayer(this);\r\n\t\t\tcow->IncreaseVacuumTimer();'),
    ('ani->IncreaseVacuumTimer();', 'ani->SetTargetPlayer(this);\r\n\t\t\tani->IncreaseVacuumTimer();'),
    ('cow->ResetVacuumTimer();', 'cow->SetTargetPlayer(nullptr);\r\n\t\tcow->ResetVacuumTimer();'),
    ('ani->SetCurrentState(STATE_WALK);', 'ani->SetTargetPlayer(nullptr);\r\n\t\tani->SetCurrentState(STATE_WALK);')
])

# 4. Update CowMove.cpp
process_file(r'Source\CowMove.cpp', [
    ('auto p = ServiceLocator::GetPlayer();\r\n\t\tPlayer3D* player = dynamic_cast<Player3D*>(p);', 'Player3D* player = mpTargetPlayer;'),
    ('auto p = ServiceLocator::GetPlayer();\r\n\tPlayer3D* player = dynamic_cast<Player3D*>(p);', 'Player3D* player = mpTargetPlayer;')
])

# 5. Update AnimalMove.cpp
process_file(r'Source\AnimalMove.cpp', [
    ('auto p = ServiceLocator::GetPlayer();\r\n\tPlayer3D* player = dynamic_cast<Player3D*>(p);', 'Player3D* player = mpTargetPlayer;')
])

print("Done")
