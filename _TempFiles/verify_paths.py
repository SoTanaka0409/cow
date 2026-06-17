import os
import re

# Check Score.cpp and Player3D.cpp for image path correctness
files_to_check = ['Source/Score.cpp', 'Source/Player3D.cpp', 'Source/TitleScene.cpp']
resource_base = 'Resource/2D'

for filepath in files_to_check:
    with open(filepath, 'rb') as f:
        data = f.read()
    # strip BOM if present
    if data.startswith(b'\xef\xbb\xbf'):
        data = data[3:]
    text = data.decode('utf-8', errors='replace')
    paths = re.findall(r'LoadGraph\("(.*?)"\)', text)
    print(f'\n--- {filepath} ---')
    for p in paths:
        exists = os.path.exists(p)
        print(f'  {"OK  " if exists else "MISS"}: {p}')
