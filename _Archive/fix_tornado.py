import sys

def fix_tornado():
    with open('Source/Tornado.cpp', 'r', encoding='utf-8-sig') as f:
        lines = f.readlines()
        
    for i in range(len(lines)):
        if 'velocity_ = VGet(0, 0, 0);' in lines[i]:
            lines[i] = lines[i].replace('velocity_ = VGet(0, 0, 0);', 'velocity_ = VGet(1.0f, 0.0f, 0.0f);')
            
        if 'velocity_ = VAdd(velocity_, VScale(targetDir, homingStrength));' in lines[i]:
            # Replace the homing logic with a safer version
            new_logic = '''			if (VSquareSize(velocity_) < 0.001f) {
				velocity_ = targetDir;
			} else {
				velocity_ = VAdd(velocity_, VScale(targetDir, homingStrength));
				velocity_ = VNorm(velocity_);
			}
'''
            lines[i] = new_logic
        elif 'velocity_ = VNorm(velocity_);' in lines[i]:
            lines[i] = '' # Remove the original VNorm line
            
    with open('Source/Tornado.cpp', 'w', encoding='utf-8-sig') as f:
        f.writelines(lines)

fix_tornado()
print('Fixed Tornado.cpp')
