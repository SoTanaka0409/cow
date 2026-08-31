$enc = [System.Text.Encoding]::GetEncoding("shift_jis")
$path = "Source\Camera.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'int deltaX = mouse_x_ - center_x_;', 'int deltaX = mouse_x_ - center_x;'
[System.IO.File]::WriteAllText($path, $content, $enc)
