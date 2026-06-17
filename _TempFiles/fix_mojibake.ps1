$replacements = @{
    '繧ｿ繧､繝医Ν逕ｻ髱｢' = 'タイトル画面'
    '繧ｹ繧ｿ繝ｼ繝医・繧ｿ繝ｳ1' = 'スタートボタン1'
    '繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｼ縺溘ｓ' = 'チュートリアルぼたん'
    '縺帙▲縺ｦ縺・⊂縺溘ｓ1' = 'せっていぼたん1'
    '縺帙▲縺ｦ縺・⊂縺溘ｓ' = 'せっていぼたん'
    '繧ｿ繧､繝医Ν繧ｷ繝ｼ繝ｳ' = 'タイトルシーン'
    '迚帙→UFO' = '牛とUFO'
    '1菴・' = '1位'
    '2菴・' = '2位'
    '3菴・' = '3位'
    '繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ' = 'ランキング'
    '縺励ｅ縺・ｊ繧・≧縺ｼ縺溘ｓ' = 'しゅうりょうぼたん'
    '繧ｹ繧ｳ繧｢' = 'スコア'
    '繝ｪ繧ｶ繝ｫ繝・' = 'リザルト'
}

$files = @(
    "TitleScene.cpp", "TutorialScene.cpp", "Rule.cpp", "ResultScene.cpp", "Scene3D.cpp", "Main.cpp", "Scene.cpp", "Score.cpp"
)

foreach ($file in $files) {
    $path = "c:\Users\Admin\Desktop\progtram\Project1-sss\Source\$file"
    if (Test-Path $path) {
        $content = Get-Content -Path $path -Encoding UTF8 -Raw
        foreach ($key in $replacements.Keys) {
            $content = $content.Replace($key, $replacements[$key])
        }
        Set-Content -Path $path -Value $content -Encoding Default
    }
}
