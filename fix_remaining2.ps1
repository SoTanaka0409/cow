$files = "Source\Combo.cpp", "Source\GameTimer.cpp", "Source\Score.cpp", "Source\TutorialScene.cpp"
foreach ($f in $files) {
    $c = [System.IO.File]::ReadAllText($f, [System.Text.Encoding]::UTF8)
    
    # regex match anything inside the quotes starting with Resource/ that ends with 00.png
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+00\.png"\)', 'LoadGraph("Resource/2D/コンボ数00.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+01\.png"\)', 'LoadGraph("Resource/2D/コンボ数01.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+02\.png"\)', 'LoadGraph("Resource/2D/コンボ数02.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+03\.png"\)', 'LoadGraph("Resource/2D/コンボ数03.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+04\.png"\)', 'LoadGraph("Resource/2D/コンボ数04.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+05\.png"\)', 'LoadGraph("Resource/2D/コンボ数05.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+06\.png"\)', 'LoadGraph("Resource/2D/コンボ数06.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+07\.png"\)', 'LoadGraph("Resource/2D/コンボ数07.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+08\.png"\)', 'LoadGraph("Resource/2D/コンボ数08.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+09\.png"\)', 'LoadGraph("Resource/2D/コンボ数09.png")')

    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+0\.jpg"\)', 'LoadGraph("Resource/2D/コンボ数00.png")')
    $c = [regex]::Replace($c, 'LoadGraph\("Resource/[^"]+\.png"\);\s*//minus', 'LoadGraph("Resource/2D/マイナス.png"); //minus')

    $c = [regex]::Replace($c, '"Resource/Tutorial/[^"]+W[^"]+\.png"', '"Resource/3D/Tutorial/Wを押して移動しよう.png"')
    $c = [regex]::Replace($c, '"Resource/Tutorial/[^"]+COMBO[^"]+\.png"', '"Resource/3D/Tutorial/COMBOをしてみよう.png"')
    $c = [regex]::Replace($c, '"Resource/Tutorial/[^"]+FEVER[^"]+\.png"', '"Resource/3D/Tutorial/金の牛を吸ってFEVERにしてみよう.png"')
    $c = [regex]::Replace($c, '"Resource/Tutorial/[^"]+X[^"]+\.png"', '"Resource/3D/Tutorial/レベルアップ！左クリックでスキル選択.png"')
    $c = [regex]::Replace($c, '"Resource/Tutorial/[^"]+_[^"]+\.png"', '"Resource/3D/Tutorial/マウスを動かして視点を動かそう.png"')
    $c = [regex]::Replace($c, '"Resource/Tutorial/[^"]+z[^"]+\.png"', '"Resource/3D/Tutorial/牛を吸ってみよう.png"')

    [System.IO.File]::WriteAllText($f, $c, [System.Text.Encoding]::UTF8)
}
