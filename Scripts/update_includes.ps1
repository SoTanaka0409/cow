$vcxprojPath = "Project1.vcxproj"
$utf8Bom = New-Object System.Text.UTF8Encoding $true
$vcxprojContent = [System.IO.File]::ReadAllText($vcxprojPath, $utf8Bom)

$newIncludes = ";`$(ProjectDir)Source;`$(ProjectDir)Source\System;`$(ProjectDir)Source\Scene;`$(ProjectDir)Source\Object;`$(ProjectDir)Source\Actor;`$(ProjectDir)Source\Animal;`$(ProjectDir)Source\Stage;`$(ProjectDir)Source\Camera;`$(ProjectDir)Source\Collision;`$(ProjectDir)Source\Graphics;`$(ProjectDir)Source\UI_Input_Sound"

$vcxprojContent = [regex]::Replace($vcxprojContent, "(?i)(<AdditionalIncludeDirectories>)(.*?)(</AdditionalIncludeDirectories>)", "`${1}`$2$newIncludes`$3")

[System.IO.File]::WriteAllText($vcxprojPath, $vcxprojContent, $utf8Bom)
Write-Host "Include directories updated."
