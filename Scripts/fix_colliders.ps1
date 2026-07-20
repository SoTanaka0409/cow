$enc = [System.Text.Encoding]::GetEncoding("shift_jis")

# CapsuleCollider.cpp
$path = "Source\CapsuleCollider.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'void CapsuleCollider::Draw\(\)\s*\{\s*\}'
$replace = "void CapsuleCollider::Draw()`r`n{`r`n`tDrawCapsule3D(`r`n`t`tposition_,`r`n`t`tposition2_,`r`n`t`tradius_,`r`n`t`t8,`r`n`t`tGetColor(255, 0, 0),`r`n`t`tGetColor(255, 0, 0),`r`n`t`tfalse`r`n`t);`r`n}"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)

# SphereCollider.cpp
$path = "Source\SphereCollider.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'GetColor\(255, 255, 255\),\s*GetColor\(255, 255, 255\)'
$replace = "GetColor(255, 0, 0),`r`n`t`tGetColor(255, 0, 0)"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)

