$enc = [System.Text.Encoding]::GetEncoding("shift_jis")
$path = "Source\Camera.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace "`r`n`r`n", "`r`n"
[System.IO.File]::WriteAllText($path, $content, $enc)
