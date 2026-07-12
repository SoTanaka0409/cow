$utf8bom = New-Object System.Text.UTF8Encoding $True

# Fix GameManager.h enum definition
$f1 = 'Source\GameManager.h'
$t1 = Get-Content $f1 -Encoding Default -Raw
$t1 = $t1 -replace '(?m)^\s*Normal,', "`t`tkNormal,"
$t1 = $t1 -replace '(?m)^\s*MassSpawn,', "`t`tkMassSpawn,"
$t1 = $t1 -replace '(?m)^\s*TornadoCrisis', "`t`tkTornadoCrisis"
[System.IO.File]::WriteAllText($f1, $t1, $utf8bom)

# Fix Skill.h method
$f2 = 'Source\Skill.h'
$t2 = Get-Content $f2 -Encoding Default -Raw
$t2 = $t2 -replace 'return tag;', 'return tag_;'
[System.IO.File]::WriteAllText($f2, $t2, $utf8bom)

# Fix Skill.cpp initializer list
$f3 = 'Source\Skill.cpp'
$t3 = Get-Content $f3 -Encoding Default -Raw
$t3 = $t3 -replace '\btag\(', 'tag_('
[System.IO.File]::WriteAllText($f3, $t3, $utf8bom)

Write-Host 'Fixed.'
