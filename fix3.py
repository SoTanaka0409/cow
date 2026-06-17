import codecs
import re

def process_file_regex(path, replacements):
    encoding = 'utf-8-sig'
    try:
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
    except UnicodeDecodeError:
        encoding = 'shift_jis'
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
            
    for old_pattern, new_text in replacements:
        content = re.sub(old_pattern, new_text, content, flags=re.DOTALL)
        
    with codecs.open(path, 'w', encoding) as f:
        f.write(content)

# Fever.h
process_file_regex(r'Source\Fever.h', [
    (r'void StartFever\(\);', 'void StartFever(class Player3D* player);'),
    (r'float playerStatus;', 'float playerStatus;\r\n\tclass Player3D* mpFeverPlayer;')
])

# Fever.cpp
process_file_regex(r'Source\Fever.cpp', [
    (r'Fever::Fever\(\)\r?\n\s*: dropTime\(0\)\r?\n\s*, DropCount\(0\)', 'Fever::Fever()\r\n\t: dropTime(0)\r\n\t, DropCount(0)\r\n\t, mpFeverPlayer(nullptr)'),
    (r'void Fever::StartFever\(\)\r?\n\{\r?\n\s*//[^\n]*\r?\n\s*auto p = ServiceLocator::GetPlayer\(\);\r?\n\s*Player3D\* player = dynamic_cast<Player3D\*>\(p\);', 'void Fever::StartFever(Player3D* player)\r\n{\r\n\tif (player == nullptr) return;\r\n\tmpFeverPlayer = player;'),
    (r'void Fever::EndFever\(\)\r?\n\{\r?\n\s*//[^\n]*\r?\n\s*auto p = ServiceLocator::GetPlayer\(\);\r?\n\s*Player3D\* player = dynamic_cast<Player3D\*>\(p\);\r?\n\s*player->SetStatusAttack\(playerStatus\);', 'void Fever::EndFever()\r\n{\r\n\tif (mpFeverPlayer != nullptr)\r\n\t{\r\n\t\tmpFeverPlayer->SetStatusAttack(playerStatus);\r\n\t\tmpFeverPlayer = nullptr;\r\n\t}')
])

# Cow_gold.cpp
process_file_regex(r'Source\Cow_gold.cpp', [
    (r'ServiceLocator::GetFever\(\)->StartFever\(\);', 'ServiceLocator::GetFever()->StartFever(mpTargetPlayer);')
])

# Camera.h
process_file_regex(r'Source\Camera.h', [
    (r'void SetPosition\(VECTOR pos\) \{ mvPosition = pos; \}', 'void SetPosition(VECTOR pos) { mvPosition = pos; }\r\n\tvoid SetTarget(Object3D* target) { mpTarget = target; }')
])

print("Done")
