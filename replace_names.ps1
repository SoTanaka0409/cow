$files = @("Source\TitleScene.cpp", "Source\ResultScene.cpp", "Source\Score.cpp")
$replacements = @{
    "1?Epng" = "1位.png"
    "2?Epng" = "2位.png"
    "3?Epng" = "3位.png"
    "スタートEタン1.png" = "スタートぼたん1.png"
    "せってぁEたん1.png" = "せっていぼたん1.png"
    "しゅぁEめEぼたん (1).png" = "しゅうりょうぼたん (1).png"
    "リザルチEpng" = "リザルト.png"
    "コンボ数" = "コンボ"
}

foreach ($f in $files) {
    $text = [System.IO.File]::ReadAllText($f, [System.Text.Encoding]::UTF8)
    foreach ($k in $replacements.Keys) {
        $v = $replacements[$k]
        $text = $text.Replace($k, $v)
    }
    $utf8BOM = New-Object System.Text.UTF8Encoding($true)
    [System.IO.File]::WriteAllText($f, $text, $utf8BOM)
    Write-Host "Replaced strings in $f"
}
