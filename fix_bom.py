import os

def fix_double_bom(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.h'):
                path = os.path.join(root, file)
                with open(path, 'rb') as f:
                    content = f.read()
                
                # Check for double BOM
                bom = b'\xef\xbb\xbf'
                if content.startswith(bom + bom):
                    # Remove one BOM
                    content = content[3:]
                    with open(path, 'wb') as f:
                        f.write(content)
                    print(f"Fixed {path}")

fix_double_bom('Source')
