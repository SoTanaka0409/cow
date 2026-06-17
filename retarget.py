import codecs

with codecs.open('Project1.vcxproj', 'r', 'utf-8-sig') as f:
    content = f.read()

content = content.replace('<PlatformToolset>v145</PlatformToolset>', '<PlatformToolset>v143</PlatformToolset>')

with codecs.open('Project1.vcxproj', 'w', 'utf-8-sig') as f:
    f.write(content)

print("Retargeted vcxproj to v143")
