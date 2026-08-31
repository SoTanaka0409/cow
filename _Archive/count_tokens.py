import sys
from pathlib import Path
import tiktoken

root = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(r"C:\Users\student\Desktop\programing\cow")
text_exts = {
    '.cpp', '.cc', '.c', '.h', '.hpp', '.txt', '.md', '.py', '.json', '.xml', '.ini', '.yml', '.yaml',
    '.cs', '.sln', '.vcxproj', '.filters', '.user', '.vcproj', '.js', '.ts', '.tsx', '.jsx', '.cfg',
    '.bat', '.ps1', '.sh', '.csv', '.log', '.rc', '.resx'
}
enc = tiktoken.get_encoding('cl100k_base')

total_tokens = 0
file_counts = 0
non_text = 0
skipped = 0

for path in root.rglob('*'):
    if path.is_file():
        try:
            if path.suffix.lower() in text_exts:
                data = path.read_bytes()
                try:
                    decoded = data.decode('utf-8')
                except UnicodeDecodeError:
                    decoded = data.decode('utf-8', errors='replace')
                total_tokens += len(enc.encode(decoded))
                file_counts += 1
            else:
                non_text += 1
        except Exception:
            skipped += 1

print('total_tokens', total_tokens)
print('file_counts', file_counts)
print('non_text', non_text)
print('skipped', skipped)
