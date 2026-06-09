$sourceDir = "c:\Users\Admin\Desktop\progtram\Project1-sss\Source"
$files = Get-ChildItem -Path $sourceDir -Include *.cpp, *.h -Recurse

foreach ($file in $files) {
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    $isUtf8 = $true
    try {
        $utf8Encoding = New-Object System.Text.UTF8Encoding $false, $true
        $null = $utf8Encoding.GetString($bytes)
    } catch {
        $isUtf8 = $false
    }
    
    if (-not $isUtf8) {
        # It's likely CP932
        $cp932 = [System.Text.Encoding]::GetEncoding(932)
        $text = $cp932.GetString($bytes)
        [System.IO.File]::WriteAllText($file.FullName, $text, [System.Text.Encoding]::UTF8)
        Write-Host "Converted $($file.FullName) from CP932 to UTF-8 BOM"
    } else {
        # It's already UTF-8. Let's just make sure it has a BOM.
        $text = [System.Text.Encoding]::UTF8.GetString($bytes)
        [System.IO.File]::WriteAllText($file.FullName, $text, [System.Text.Encoding]::UTF8)
        Write-Host "Ensured BOM for $($file.FullName)"
    }
}
