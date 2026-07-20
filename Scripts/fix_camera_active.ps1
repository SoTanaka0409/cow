$enc = [System.Text.Encoding]::GetEncoding("shift_jis")
$path = "Source\Camera.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'const float MOUSE_SENSITIVITY = 0\.05f;'
$replace = "const float MOUSE_SENSITIVITY = 0.05f;`r`n`tif (GetWindowActiveFlag() == 0) return;"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)
