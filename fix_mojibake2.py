import codecs
files = ['Source/TitleScene.cpp', 'Source/ResultScene.cpp', 'Source/Score.cpp']
replacements = {'1菴・png': '1位.png', '2菴・png': '2位.png', '3菴・png': '3位.png', '繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ.png': 'ランキング.png', '繧ｹ繧ｳ繧｢.png': 'スコア.png', '轤ｹ.png': '点.png', '繧ｹ繧ｿ繝ｼ繝医・繧ｿ繝ｳ1.png': 'スタートボタン1.png', '縺励ｅ縺・ｊ繧・≧縺ｼ縺溘ｓ (1).png': 'しゅうりょうぼたん (1).png', '繧ｿ繧､繝医Ν繧ｷ繝ｼ繝ｳ.png': 'タイトルシーン.png', '迚帙→UFO.png': '牛とUFO.png', '繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｼ縺溘ｓ.png': 'チュートリアルぼたん.png', '縺帙▲縺ｦ縺・⊂縺溘ｓ1.png': 'せっていぼたん1.png', '}CiX.png': 'マイナス.png', 'R{00.png': 'コンボ数00.png', 'R{01.png': 'コンボ数01.png', 'R{02.png': 'コンボ数02.png', 'R{03.png': 'コンボ数03.png', 'R{04.png': 'コンボ数04.png', 'R{05.png': 'コンボ数05.png', 'R{06.png': 'コンボ数06.png', 'R{07.png': 'コンボ数07.png', 'R{08.png': 'コンボ数08.png', 'R{09.png': 'コンボ数09.png', '繝ｪ繧ｶ繝ｫ繝・png': 'リザルト.png'}
for f in files:
    with open(f, 'r', encoding='utf-8-sig') as file:
        content = file.read()
    for old, new in replacements.items():
        content = content.replace(old, new)
    with open(f, 'w', encoding='utf-8-sig') as file:
        file.write(content)
