import os

source_dir = r"c:\Users\Admin\Desktop\progtram\Project1-sss\Source"
files = [os.path.join(dp, f) for dp, dn, filenames in os.walk(source_dir) for f in filenames if f.endswith('.cpp') or f.endswith('.h')]

for file in files:
    content = None
    encodings = ['utf-8-sig', 'utf-8', 'cp932']
    for enc in encodings:
        try:
            with open(file, 'r', encoding=enc) as f:
                content = f.read()
            break
        except UnicodeDecodeError:
            continue
    if content is not None:
        with open(file, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Converted {file}")
    else:
        print(f"Failed to read {file}")
