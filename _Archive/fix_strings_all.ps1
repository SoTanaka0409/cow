
$utf8bom = New-Object System.Text.UTF8Encoding $True

function Fix-File ($fname) {
    $file = "Source\" + $fname
    $orig = git show "origin/sou:Source/$fname"
    if ($LASTEXITCODE -ne 0) { return }
    $origStr = [string]::Join("`n", $orig)
    $origMatches = [regex]::Matches($origStr, '"Resource/[^"]+"')
    
    $curr = [System.IO.File]::ReadAllText($file, [System.Text.Encoding]::UTF8)
    $currMatches = [regex]::Matches($curr, '"Resource/[^"]+"')
    
    if ($origMatches.Count -eq $currMatches.Count -and $origMatches.Count -gt 0) {
        $changed = $false
        for ($i = $origMatches.Count - 1; $i -ge 0; $i--) {
            $o = $origMatches[$i].Value
            $c = $currMatches[$i]
            if ($o -ne $c.Value) {
                $curr = $curr.Remove($c.Index, $c.Length).Insert($c.Index, $o)
                $changed = $true
            }
        }
        if ($changed) {
            [System.IO.File]::WriteAllText($file, $curr, $utf8bom)
            Write-Host "Fixed $file"
        }
    }
}

$files = Get-ChildItem "Source" -Include "*.cpp","*.h" -Recurse | Select-Object -ExpandProperty Name
foreach ($f in $files) {
    Fix-File $f
}
Write-Host "Done!"

