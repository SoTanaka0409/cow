$b = [System.IO.File]::ReadAllBytes('Source\TutorialScene.cpp')
$hex = $b[0].ToString('X2') + $b[1].ToString('X2') + $b[2].ToString('X2')
Write-Host ("Size: " + $b.Length + "  First3bytes: " + $hex)
if ($b[0] -eq 0xEF -and $b[1] -eq 0xBB -and $b[2] -eq 0xBF) {
    Write-Host "UTF-8 BOM: YES"
} else {
    Write-Host "UTF-8 BOM: NO (might be SJIS or BOM-less UTF-8)"
}
