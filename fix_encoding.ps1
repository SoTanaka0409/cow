$files = Get-ChildItem -Path "Source" -Include *.cpp, *.h -Recurse

foreach ($file in $files) {
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    $hasBom = ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF)
    
    if ($hasBom) {
        # Read as Shift-JIS but skip the BOM
        $sjis = [System.Text.Encoding]::GetEncoding("shift_jis")
        $text = $sjis.GetString($bytes, 3, $bytes.Length - 3)
        
        # Check if it contains some Japanese characters to verify it's Shift-JIS
        # Actually, let's just save it as UTF-8 with BOM
        $utf8BOM = New-Object System.Text.UTF8Encoding($true)
        [System.IO.File]::WriteAllText($file.FullName, $text, $utf8BOM)
        Write-Host "Fixed BOM + Shift-JIS -> proper UTF-8 BOM: $($file.Name)"
    } else {
        # No BOM, probably Shift-JIS. Let's convert to UTF-8 with BOM to be safe.
        $sjis = [System.Text.Encoding]::GetEncoding("shift_jis")
        $text = $sjis.GetString($bytes)
        $utf8BOM = New-Object System.Text.UTF8Encoding($true)
        [System.IO.File]::WriteAllText($file.FullName, $text, $utf8BOM)
        Write-Host "Converted Shift-JIS -> UTF-8 BOM: $($file.Name)"
    }
}
