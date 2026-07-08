$bom = [byte[]](0xEF, 0xBB, 0xBF)
$basePath = 'C:\Users\Admin\Desktop\progtram\Cow\Source'
$targets = @(
    "$basePath\TutorialScene.cpp"
)

foreach ($path in $targets) {
    $raw = [System.IO.File]::ReadAllBytes($path)
    if ($raw.Length -ge 3 -and $raw[0] -eq 0xEF -and $raw[1] -eq 0xBB -and $raw[2] -eq 0xBF) {
        Write-Host ("Already BOM: " + (Split-Path $path -Leaf))
    } else {
        $newBytes = New-Object byte[] ($bom.Length + $raw.Length)
        [System.Array]::Copy($bom, 0, $newBytes, 0, $bom.Length)
        [System.Array]::Copy($raw, 0, $newBytes, $bom.Length, $raw.Length)
        [System.IO.File]::WriteAllBytes($path, $newBytes)
        Write-Host ("BOM added: " + (Split-Path $path -Leaf))
    }
}
Write-Host "Done."
