import codecs

with open('Source/Scene3D.cpp', 'r', encoding='utf-8') as f:
    text = f.read()

# Replace strings
replacements = {
    '螻ｱ': '山',
    '蝨ｰ蠖｢': '地形',
    '遏ｳ': '石',
    '譁ｰ縺励＞譟ｵ': '新しい柵',
    '陬・｣ｾ': '装飾',
    '迚帙′螟ｧ驥冗匱逕?E!': '牛が大量発生！',
    '蜿?E?鬚ｨ縺悟ｷ?E?螟ｧ蛹・!': '竜巻が巨大化！',
}

for k, v in replacements.items():
    text = text.replace(k, v)

with open('Source/Scene3D.cpp.temp', 'w', encoding='utf-8') as f:
    f.write(text)
