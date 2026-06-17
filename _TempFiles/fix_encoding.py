import os

files_to_fix = [
    r"Source\Scene3D.cpp",
    r"Source\TutorialScene.cpp",
    r"Source\Fever.cpp",
    r"Source\CharacterState.cpp",
    r"Source\CharacterState.h",
    r"Source\GameConstants.h",
    r"Source\AnimalMove.cpp",
    r"Source\CowMove.cpp"
]

for file in files_to_fix:
    if os.path.exists(file):
        try:
            # Read as UTF-8
            with open(file, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Write as Shift-JIS
            with open(file, 'w', encoding='cp932') as f:
                f.write(content)
            print(f"Fixed {file}")
        except Exception as e:
            print(f"Error fixing {file}: {e}")
