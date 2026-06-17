import sys
import codecs

files = [
    r'c:\Users\student\Desktop\programing\cow\Source\TitleScene.h',
    r'c:\Users\student\Desktop\programing\cow\Source\TitleScene.cpp',
    r'c:\Users\student\Desktop\programing\cow\Source\TutorialScene.h',
    r'c:\Users\student\Desktop\programing\cow\Source\TutorialScene.cpp',
    r'c:\Users\student\Desktop\programing\cow\Source\ResultScene.h',
    r'c:\Users\student\Desktop\programing\cow\Source\ResultScene.cpp'
]

for f in files:
    try:
        content = codecs.open(f, 'r', 'utf-8-sig').read()
    except UnicodeDecodeError:
        content = codecs.open(f, 'r', 'cp932').read()
    codecs.open(f, 'w', 'utf-8-sig').write(content)
    print(f"Converted {f} to utf-8-sig")
