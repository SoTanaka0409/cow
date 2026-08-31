import re

with open('Source/Scene3D.cpp', 'rb') as f:
    raw = f.read()

# Find and replace broken DrawString lines with safe ASCII versions
# Line 255: DrawString(200, 350, "<broken jp>", GetColor(255, 255, 255), true);  -> "Cow mass spawning!"
# Line 260: DrawString(200, 350, "<broken jp>", GetColor(255, 255, 255), true);  -> "Giant tornado approaching!"

# Find each broken line by locating DrawString(200, 350 occurrences
# Replace the full broken string literal (from " to closing ")
# We identify by context: first occurrence is after "MASS SPAWN!", second after "TORNADO CRISIS!"

lines = raw.split(b'\n')
new_lines = []
mass_fixed = False
tornado_fixed = False

for line in lines:
    if b'DrawString(200, 350,' in line:
        if not mass_fixed:
            # Replace with safe string
            new_lines.append(b'\t\t\tDrawString(200, 350, "Cow mass spawning!", GetColor(255, 255, 255), true);')
            mass_fixed = True
        elif not tornado_fixed:
            new_lines.append(b'\t\t\tDrawString(200, 350, "Giant tornado approaching!", GetColor(255, 255, 255), true);')
            tornado_fixed = True
        else:
            new_lines.append(line)
    else:
        new_lines.append(line)

fixed_raw = b'\n'.join(new_lines)
with open('Source/Scene3D.cpp', 'wb') as f:
    f.write(fixed_raw)

print(f"Fixed: mass_fixed={mass_fixed}, tornado_fixed={tornado_fixed}")
