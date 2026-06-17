import os

# Get exact bytes of the filenames to match against source files
files = os.listdir('Resource/2D')
for f in sorted(files):
    # print as bytes to see exact encoding
    print(f.encode('utf-8'))
