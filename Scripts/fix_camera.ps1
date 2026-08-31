$enc = [System.Text.Encoding]::GetEncoding("shift_jis")
$path = "Source\Camera.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'float center_x_', 'float center_x'
$content = $content -replace 'float center_y_', 'float center_y'
[System.IO.File]::WriteAllText($path, $content, $enc)
