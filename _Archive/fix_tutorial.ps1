$path = 'Source\TutorialScene.cpp'
# In Windows PowerShell, Default encoding is ANSI (CP932 on Japanese OS)
$lines = Get-Content $path -Encoding Default
$text = $lines -join "`r`n"
$text = $text.Replace('GameConstants::COW_DEFAULT.modelPath', 'GameConstants::kCowDefault.model_path')

# In .NET Framework, UTF8Encoding(True) gives BOM
$utf8bom = New-Object System.Text.UTF8Encoding $True
[System.IO.File]::WriteAllText($path, $text, $utf8bom)
Write-Host "TutorialScene.cpp fixed and saved with UTF-8 BOM"

$p3dPath = 'Source\Player3D.cpp'
if (Test-Path $p3dPath) {
    $lines2 = Get-Content $p3dPath -Encoding Default
    $text2 = $lines2 -join "`r`n"
    $text2 = $text2.Replace('Utility::UI_VACUUM_W', 'Utility::kUiVacuumW')
    $text2 = $text2.Replace('Utility::UI_VACUUM_H', 'Utility::kUiVacuumH')
    $text2 = $text2.Replace('Utility::UI_BASE_X', 'Utility::kUiBaseX')
    $text2 = $text2.Replace('Utility::UI_VACUUM_Y', 'Utility::kUiVacuumY')
    $text2 = $text2.Replace('Utility::SCREEN_WIDTH', 'Utility::kScreenWidth')
    $text2 = $text2.Replace('Utility::SCREEN_HEIGHT', 'Utility::kScreenHeight')
    [System.IO.File]::WriteAllText($p3dPath, $text2, $utf8bom)
    Write-Host "Player3D.cpp fixed and saved with UTF-8 BOM"
}
