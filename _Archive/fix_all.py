import os
import re

def process_file(filepath):
    if not os.path.exists(filepath): return
    with open(filepath, 'rb') as f:
        data = f.read()
    try:
        text = data.decode('utf-8')
    except:
        text = data.decode('cp932', errors='ignore')

    # Fix garbled patterns based on common ones seen
    replacements = [
        (r'モチE', 'モデル'),
        (r'チEスチャ', 'テクスチャ'),
        (r'座樁E', '座標'),
        (r'Ȃ', 'なし'),
        (r'冁EE', '内の'),
        (r'生EE', '生成・'),
        (r'管琁E', '管理'),
        (r'取得すめE', '取得する'),
        (r'刁E替える', '切り替える'),
        (r'状慁E', '状態'),
        (r'伴ぁE', '伴う'),
        (r'亁E', '次の'),
        (r'スチEプ', 'ステップ'),
        (r'タイチE', 'タイプ'),
        (r'開姁E', '開始'),
        (r'Kvな', '必要な'),
        (r'演E', '演出'),
        (r'監視E', '監視・'),
        (r'タイマE', 'タイマー'),
        (r'実衁E', '実行'),
        (r'リスチE', 'リスト'),
        (r'基溁E', '基準'),
        (r'繝ｫ繝ｼ繝医ヵ繝ｬ繝ｼ繝縺檎ｧE蜍輔い繝九Γ繝ｼ繧E繝ｧ繝ｳ縺E蜍晄焔縺E蜍輔＞縺E縺壹E縺E縺EE縺E€√Ο繝ｼ繧E繝ｫ陦悟E繧貞崋螳壹☁EE', ''),
        (r'繧E繝九Γ繝ｼ繧E繝ｧ繝ｳ驕ｷ遘ｻ譎ゅE繝悶Ξ繝ｳ繝E紫繧E繝輔Ξ繝ｼ繝縺斐E縺E騾E陦後！E帙ａE', ''),
        (r'髱槭Ν繝ｼ繝励ぁE九Γ繝ｼ繧E繝ｧ繝ｳ縺檎ｵめEEE縺溷EE蜷医€∵EE縺E謖EE夐E遘ｻ蜈医′縺E縺代E縺E繧E繝九Γ繝ｼ繧E繝ｧ繝ｳ譎る俣繧呈ｭE繧√ａE', ''),
        (r'蜿E縺EぁE九Γ繝ｼ繧E繝ｧ繝ｳ縺E繝悶Ξ繝ｳ繝E紫繧剁E九￡縺E縺E″縲∵怙邨ら噪縺E繝輔ぉ繝ｼ繝峨ぁEE繝医EE帙ａE', ''),
        (r'譌｢縺E蜷後§繧E繝九Γ繝ｼ繧E繝ｧ繝ｳ縺碁E謚槭EE後※縺EE蝣E蜷医E辟｡鬧E↑繝Eち繝Eメ繝ｻ繧E繧E繝Eメ繧帝∩縺代EE', ''),
        (r'蜿E縺吶℁E九い繝九Γ繝ｼ繧E繝ｧ繝ｳ繝上Φ繝峨Ν縺梧E九▲縺E縺EE蝣E蜷医E螳悟E縺E蛻EE髮E縺E', ''),
        (r'繝悶Ξ繝ｳ繝峨E縺E縺EEE蜷医E蜊ｳ蠎ｧ縺E譁E隕上い繝九Γ繝ｼ繧E繝ｧ繝ｳ縺E螳悟E蛻EE譖ｿ縺医☁EE', '')
    ]

    for p, r in replacements:
        text = re.sub(p, r, text)

    # Convert known garbled characters
    text = text.replace('', '')

    with open(filepath, 'w', encoding='utf-8-sig') as f:
        f.write(text)

files = [
    'Floor.h', 'GameConstants.h', 'GameManager.cpp', 'GameManager.h',
    'GameTimer.cpp', 'GameTimer.h', 'GoldCow.cpp', 'GoldCow.h',
    'InputManager.cpp', 'InputManager.h', 'Level.cpp', 'Level.h',
    'LoadingScene.cpp', 'LoadingScene.h', 'Main.cpp', 'Master.h',
    'Model.cpp', 'Model.h', 'ModelAnimation.cpp', 'ModelAnimation.h',
    'ModelUtility.h'
]

for f in files:
    process_file(os.path.join('Source', f))
    print(f"Processed {f}")
