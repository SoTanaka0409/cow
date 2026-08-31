import xml.etree.ElementTree as ET
import os

def get_category(filename):
    fname = filename.lower()
    if 'scene' in fname:
        return 'Scene'
    if 'manager' in fname:
        return 'Manager'
    if any(x in fname for x in ['player', 'cow', 'animal', 'character', 'goldcow']):
        return 'Character'
    if 'collider' in fname or 'collision' in fname:
        return 'Collision'
    if any(x in fname for x in ['object', 'model', 'bait', 'fever', 'tornado', 'thunder', 'mountain', 'skybox', 'floor', 'wall', 'stage', 'level', 'texture']):
        return 'Object'
    if any(x in fname for x in ['main', 'timer', 'input', 'servicelocator', 'master', 'camera', 'rule', 'score', 'combo', 'skill', 'effekseer', 'utility', 'ui']):
        return 'System'
    return 'Other'

# Register namespaces
ET.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')

tree = ET.parse('Project1.vcxproj.filters')
root = tree.getroot()
ns = {'ns': 'http://schemas.microsoft.com/developer/msbuild/2003'}

# Keep track of unique filters we need
filters_needed = set()

# Update ClCompile and ClInclude elements
for item_group in root.findall('ns:ItemGroup', ns):
    for cl_compile in item_group.findall('ns:ClCompile', ns):
        include_path = cl_compile.get('Include')
        if include_path and (include_path.startswith('Source\\') or include_path.startswith('Source/')):
            basename = os.path.basename(include_path)
            cat = get_category(basename)
            filter_node = cl_compile.find('ns:Filter', ns)
            if filter_node is not None:
                new_filter = f"ソース ファイル\\{cat}"
                filter_node.text = new_filter
                filters_needed.add(new_filter)
                filters_needed.add("ソース ファイル")
                
    for cl_include in item_group.findall('ns:ClInclude', ns):
        include_path = cl_include.get('Include')
        if include_path and (include_path.startswith('Source\\') or include_path.startswith('Source/')):
            basename = os.path.basename(include_path)
            cat = get_category(basename)
            filter_node = cl_include.find('ns:Filter', ns)
            if filter_node is not None:
                new_filter = f"ヘッダー ファイル\\{cat}"
                filter_node.text = new_filter
                filters_needed.add(new_filter)
                filters_needed.add("ヘッダー ファイル")

# Update Filter definitions
filter_item_group = None
for item_group in root.findall('ns:ItemGroup', ns):
    if item_group.find('ns:Filter', ns) is not None:
        filter_item_group = item_group
        break

if filter_item_group is None:
    filter_item_group = ET.SubElement(root, 'ItemGroup')

# Get existing filters
existing_filters = set()
for f in filter_item_group.findall('ns:Filter', ns):
    existing_filters.add(f.get('Include'))

# Add missing filters
import uuid
for f in sorted(list(filters_needed)):
    if f not in existing_filters:
        new_f = ET.SubElement(filter_item_group, 'Filter', {'Include': f})
        uid = ET.SubElement(new_f, 'UniqueIdentifier')
        uid.text = '{' + str(uuid.uuid4()).upper() + '}'

# Pretty print XML manually to avoid losing the format
import xml.dom.minidom
xml_str = ET.tostring(root, encoding='utf-8')
dom = xml.dom.minidom.parseString(xml_str)
formatted_xml = dom.toprettyxml(indent="  ")

# Minidom adds an extra xml declaration, we just want to write it out
with open('Project1.vcxproj.filters', 'w', encoding='utf-8') as f:
    for line in formatted_xml.splitlines():
        if line.strip():
            f.write(line + '\n')

print("Filters updated successfully.")
