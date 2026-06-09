$files = Get-ChildItem -Path Source -Include *.cpp,*.h -Recurse
$resourceFiles = Get-ChildItem -Path Resource -Recurse | Where-Object { -not $_.PSIsContainer }
$sjis = [System.Text.Encoding]::GetEncoding("shift_jis")

foreach ($file in $files) {
    $content = [System.IO.File]::ReadAllText($file.FullName, $sjis)
    $originalContent = $content

    $matches = [regex]::Matches($content, '"(Resource/[^"]+)"')
    $uniqueMatches = $matches | Foreach-Object { $_.Groups[1].Value } | Select-Object -Unique

    foreach ($oldPath in $uniqueMatches) {
        $parts = $oldPath -split '/'
        $basename = $parts[-1]
        $parentFolder = if ($parts.Length -gt 2) { $parts[-2] } else { "Resource" }
        
        $matchedFile = $null
        foreach ($rf in $resourceFiles) {
            if ($rf.Name -eq $basename) {
                $rfRel = $rf.FullName.Substring((Get-Location).Path.Length + 1) -replace '\\', '/'
                if ($parentFolder -eq "Resource") {
                    $matchedFile = $rfRel
                    break
                } elseif ($rfRel -match "/$parentFolder/") {
                    $matchedFile = $rfRel
                    break
                }
            }
        }

        if ($matchedFile -ne $null -and $oldPath -ne $matchedFile) {
            $content = $content.Replace('"' + $oldPath + '"', '"' + $matchedFile + '"')
        }
    }

    if ($content -cne $originalContent) {
        [System.IO.File]::WriteAllText($file.FullName, $content, $sjis)
        Write-Host "Updated $($file.Name)"
    }
}
