$files = "Source\Combo.cpp", "Source\GameTimer.cpp", "Source\Score.cpp", "Source\TutorialScene.cpp"
foreach ($f in $files) {
    $c = [System.IO.File]::ReadAllText($f, [System.Text.Encoding]::UTF8)
    
    # Replace Combo/Timer/Score numbers
    for ($i = 0; $i -le 9; $i++) {
        $c = $c -replace "LoadGraph\("Resource/[^"]+0$i\.png"\)", "LoadGraph("Resource/2D/コンボ数0$i.png")"
    }

    # Score minus image
    $c = $c -replace "LoadGraph\("Resource/[^"]+\.png"\); // minus", "LoadGraph("Resource/2D/マイナス.png");"

    # Tutorial
    $c = $c -replace ""Resource/Tutorial/[^"]+W[^"]+\.png"", ""Resource/3D/Tutorial/Wを押して移動しよう.png""
    $c = $c -replace ""Resource/Tutorial/[^"]+COMBO[^"]+\.png"", ""Resource/3D/Tutorial/COMBOをしてみよう.png""
    $c = $c -replace ""Resource/Tutorial/[^"]+FEVER[^"]+\.png"", ""Resource/3D/Tutorial/金の牛を吸ってFEVERにしてみよう.png""
    $c = $c -replace ""Resource/Tutorial/[^"]+X[^"]+\.png"", ""Resource/3D/Tutorial/レベルアップ！左クリックでスキル選択.png""
    $c = $c -replace ""Resource/Tutorial/[^"]+\.png"", ""Resource/3D/Tutorial/牛を吸ってみよう.png"" # Might match others, need to be careful

    [System.IO.File]::WriteAllText($f, $c, [System.Text.Encoding]::UTF8)
}
