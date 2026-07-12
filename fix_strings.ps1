
$utf8bom = New-Object System.Text.UTF8Encoding $True

function Fix-File ($file) {
    $orig = git show origin/sou:$file
    if ($LASTEXITCODE -ne 0) { return }
    $origStr = [string]::Join("`n", $orig)
    $origMatches = [regex]::Matches($origStr, '"Resource/[^"]+"')
    
    $curr = [System.IO.File]::ReadAllText($file, [System.Text.Encoding]::UTF8)
    $currMatches = [regex]::Matches($curr, '"Resource/[^"]+"')
    
    if ($origMatches.Count -eq $currMatches.Count -and $origMatches.Count -gt 0) {
        for ($i = $origMatches.Count - 1; $i -ge 0; $i--) {
            $o = $origMatches[$i].Value
            $c = $currMatches[$i]
            $curr = $curr.Remove($c.Index, $c.Length).Insert($c.Index, $o)
        }
        [System.IO.File]::WriteAllText($file, $curr, $utf8bom)
        Write-Host "Fixed $file"
    } else {
        Write-Host "Mismatch in $file"
    }
}

Fix-File "Source/Score.cpp"
Fix-File "Source/SoundManager.cpp"
Fix-File "Source/Skill.cpp"
Fix-File "Source/TitleScene.cpp"
Fix-File "Source/TutorialScene.cpp"

