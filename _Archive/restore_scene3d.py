import subprocess

# Get original bytes from git HEAD
r = subprocess.run(['git', 'show', 'HEAD:Source/Scene3D.cpp'], capture_output=True)
original = r.stdout

# Find the last new Wall line
last_wall_pos = original.rfind(b'new Wall(')
if last_wall_pos == -1:
    print('Wall not found!')
    exit(1)

# Find end of that line
line_end = original.find(b'\n', last_wall_pos)
print(f'Last wall line ends at byte {line_end}')
print(repr(original[last_wall_pos:line_end+1]))

# Build the addition (ASCII-only comment + the function call)
addition = (
    b'\n'
    b'\t// Wall is registered with kNone3d tag in Object3D ctor before SetTag(kTag3dWall) is called.\n'
    b'\t// Rebuild the tag cache after all Walls are created so collision detection works correctly.\n'
    b'\tServiceLocator::GetObjectManager()->RebuildTagCache3D();\n'
)

# Insert after the last Wall line
new_content = original[:line_end+1] + addition + original[line_end+1:]

with open('Source/Scene3D.cpp', 'wb') as f:
    f.write(new_content)
print('Written successfully')
print(f'Size: original={len(original)}, new={len(new_content)}')
