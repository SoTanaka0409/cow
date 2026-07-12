$files = Get-ChildItem "Source" -Include "*.cpp","*.h" -Recurse
$utf8bom = New-Object System.Text.UTF8Encoding $True
foreach ($file in $files) {
    $text = [System.IO.File]::ReadAllText($file.FullName, [System.Text.Encoding]::UTF8)
    # Check if the file is mojibaked. We can just try to reverse it anyway. If it wasn't mojibaked, this might corrupt it, so let's only do it for files we touched in batches 1-8. Actually, we touched almost all files.
    # Let's just reverse it for Score.cpp, SoundManager.cpp, Skill.cpp
}

