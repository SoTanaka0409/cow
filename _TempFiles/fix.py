import codecs

path = r'Source\Player3D.cpp'
with codecs.open(path, 'r', 'shift_jis') as f:
    content = f.read()

content = content.replace('#include"AnimalMove.h"', '#include"AnimalMove.h"\r\n#include"CharacterState.h"')
content = content.replace('cow->SetCurrentState(STATE_WALK);', 'cow->SetCurrentState(STATE_WALK);\r\n\t\tcow->ChangeState(new StateWalk());')
content = content.replace('ani->SetCurrentState(STATE_WALK);', 'ani->SetCurrentState(STATE_WALK);\r\n\t\tani->ChangeState(new StateWalk());')

with codecs.open(path, 'w', 'shift_jis') as f:
    f.write(content)
