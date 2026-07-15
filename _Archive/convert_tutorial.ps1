# BOMなしのファイルをSJISとして読んでUTF-8 BOM付きで書き直す
# （もしUTF-8なら UTF-8として読んでBOMを追加する）
$bom = [byte[]](0xEF, 0xBB, 0xBF)
$basePath = 'C:\Users\Admin\Desktop\progtram\Cow\Source'

# TutorialScene.cppは元々SJIS（CP932）の可能性が高い
$path = "$basePath\TutorialScene.cpp"
$raw = [System.IO.File]::ReadAllBytes($path)

# BOMがないのでSJISとして解釈してUTF-8文字列に変換
$sjis = [System.Text.Encoding]::GetEncoding(932)
$utf8 = [System.Text.Encoding]::UTF8

$str = $sjis.GetString($raw)
$utf8Bytes = $utf8.GetBytes($str)

# BOM付きで書き直す
$allBytes = New-Object byte[] ($bom.Length + $utf8Bytes.Length)
[System.Array]::Copy($bom, 0, $allBytes, 0, $bom.Length)
[System.Array]::Copy($utf8Bytes, 0, $allBytes, $bom.Length, $utf8Bytes.Length)
[System.IO.File]::WriteAllBytes($path, $allBytes)

Write-Host "TutorialScene.cpp: converted from SJIS to UTF-8 BOM"
Write-Host "Done."
