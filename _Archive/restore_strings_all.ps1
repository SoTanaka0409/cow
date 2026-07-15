$files = Get-ChildItem "Source" -Include "*.cpp","*.h" -Recurse | Select-Object -ExpandProperty Name
$utf8bom = New-Object System.Text.UTF8Encoding $True
foreach ($fname in $files) {
    $file = "Source\" + $fname
    # get original text from git
    $origText = git show origin/sou:Source/$fname
    if ($LASTEXITCODE -ne 0) { continue }
    $origTextStr = [string]::Join("`n", $origText)
    
    # regex to match "Resource/..."
    $regex = '"Resource/[^"]+"'
    $origMatches = [regex]::Matches($origTextStr, $regex)
    
    if ($origMatches.Count -gt 0) {
        $currText = [System.IO.File]::ReadAllText($file, [System.Text.Encoding]::UTF8)
        $currMatches = [regex]::Matches($currText, $regex)
        
        if ($origMatches.Count -eq $currMatches.Count) {
            $changed = $false
            for ($i = $origMatches.Count - 1; $i -ge 0; $i--) {
                $o = $origMatches[$i].Value
                $c = $currMatches[$i]
                if ($o -ne $c.Value) {
                    $currText = $currText.Remove($c.Index, $c.Length).Insert($c.Index, $o)
                    $changed = $true
                }
            }
            if ($changed) {
                [System.IO.File]::WriteAllText($file, $currText, $utf8bom)
                Write-Host ("Fixed strings in " + $fname)
            }
        } else {
            Write-Host ("Mismatch in " + $fname + " (" + $origMatches.Count + " vs " + $currMatches.Count + ")")
        }
    }
}
Write-Host "Done!"
