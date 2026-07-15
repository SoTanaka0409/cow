
$files = @("Source/Score.cpp", "Source/Rule.cpp", "Source/TutorialScene.cpp")
foreach ($file in $files) {
    $text = [System.IO.File]::ReadAllText($file)
    [System.IO.File]::WriteAllText($file, $text, (New-Object System.Text.UTF8Encoding $True))
    Write-Host "BOM added to $file"
}

